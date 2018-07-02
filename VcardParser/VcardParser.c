#include <windows.h>
#include <assert.h>
#include "VcardParser.h"


static void __tranMainState(VcardParser* self, VCARD_PARSER_MAIN_STATUS state)
{
    //printf("mainState:%d\n", self->ctxt.mainState);
    self->ctxt.mainState = state;
}

static void __tranConentState(VcardParser* self, VCARD_PARSER_CONTENT_STATUS state)
{
    //printf("contentState:%d\n", self->ctxt.contentState);
    self->ctxt.contentState = state;
}

static int __IsSkipProperty(const char* strProperty)
{
    if (strstr(strProperty, "FN")
        || strstr(strProperty, "TEL")
        || strstr(strProperty, "X-IRMC-CALL-DATETIME"))
    {
        return 0;
    }
    return 1;
}

static int __IseEndProperty(const char* strProperty)
{
    if (strstr(strProperty, "END"))
    {
        return 1;
    }
    return 0;
}

static void __PrintProperty(VcardProperty* property)
{
    int i;

    //printf("    [%s]\n", __func__);

    printf("    name:%s\n", property->name);
    printf("    paramCount:%d\n", property->paramCount);

    for (i = 0; i < property->paramCount; ++i)
    {
        printf("    param[%d]:%s, paramVal[%d]:%s\n",
            i, property->param[i].param, i, property->param[i].paramVal);
    }

    for (i = 0; i < property->valCount; ++i)
    {
        printf("    val[%d]:%s\n", i, property->val[i].val);
    }
}

static void __PrintProperties(VcardProperties* props)
{
    int i;

    printf("[%s] propCount:%d\n", __func__, props->propCount);

    for (i = 0; i < props->propCount; ++i)
    {
        printf("===[%d]===\n", i);
        __PrintProperty(&props->prop[i]);
    }
}

static VcardItem* __CreateVcardItem(VcardParser* self)
{
    VcardItem* retItem = NULL;

    if (self->type == VCARD_PB)
    {
        VcardPbItem* item = NULL;
        VcardProperties* props = &self->ctxt.props;
        VcardProperty* prop = NULL;
        int i;

        item = (VcardPbItem*)malloc(sizeof(VcardPbItem));
        if (item == NULL) {
            return NULL;
        }
        retItem = (VcardItem*)item;
        memset(item, 0x00, sizeof(VcardPbItem));
        item->super.type = self->type;

        for (i = 0; i < props->propCount; ++i)
        {
            prop = &props->prop[i];

            // fill name
            if (strcmp(props->prop[i].name, "FN") == 0)
            {
                strncpy(item->super.name, prop->val[0].val, prop->val[0].valLen);
                item->super.name[prop->val[0].valLen] = '\0';
            }
            // fill number
            else if (strcmp(props->prop[i].name, "TEL") == 0)
            {
                VcardPropertyParam* param = NULL;
                VcardNumber* vNumber = &item->number[item->numberCount];
                int j;

                for (j = 0; j < prop->paramCount; ++j)
                {
                    param = &prop->param[j];

                    if (strcmp(param->param, "TYPE") == 0)
                    {
                        if (strstr(param->paramVal, "CELL"))
                        {
                            vNumber->type = VCARD_TEL_CELL;
                        }
                        else if (strstr(param->paramVal, "HOME"))
                        {
                            vNumber->type = VCARD_TEL_HOME;
                        }
                        else if (strstr(param->paramVal, "WORK"))
                        {
                            vNumber->type = VCARD_TEL_WORK;
                        }
                        else
                        {
                            vNumber->type = VCARD_TEL_ETC;
                        }
                        break;
                    }
                }

                strncpy(vNumber->number, prop->val[0].val, prop->val[0].valLen);
                vNumber->number[prop->val[0].valLen] = '\0';
                ++item->numberCount;
            }
        }
    }
    else
    {
        VcardClItem* item = NULL;

        item = (VcardClItem*)malloc(sizeof(VcardClItem));
        if (item == NULL) {
            return NULL;
        }
        retItem = (VcardItem*)item;
        memset(item, 0x00, sizeof(VcardPbItem));
        item->super.type = self->type;


    }

    return retItem;
}

static int __contentStateMachine(VcardParser* self)
{
    int bFinish = 0;

    switch (self->ctxt.contentState)
    {
    case VCARD_PARSER_CONTENT_READY:
        self->ctxt.propSkipFlag = 0;
        self->ctxt.props.propCount++;
        __tranConentState(self, VCARD_PARSER_CONTENT_PROP);
        break;

    case VCARD_PARSER_CONTENT_PROP:
        while (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount)
        {
            VcardProperty* prop = &self->ctxt.props.prop[self->ctxt.props.propCount - 1];

            if (*self->ctxt.pParsing == ';')
            {
                if (__IsSkipProperty(prop->name))
                {
                    self->ctxt.propSkipFlag = 1;
                    self->ctxt.props.propCount--;
                    memset(prop->name, 0x00, VCARD_PROPERTY_LEN_MAX);
                    prop->propLen = 0;
                    self->ctxt.pParsing++;
                    __tranConentState(self, VCARD_PARSER_CONTENT_SKIP);
                }
                else
                {
                    prop->paramCount++;
                    self->ctxt.pParsing++;
                    __tranConentState(self, VCARD_PARSER_CONTENT_PRAM);
                }
                break;
            }

            if (*self->ctxt.pParsing == ':')
            {
                if (__IseEndProperty(prop->name))
                {
                    memset(prop->name, 0x00, VCARD_PROPERTY_LEN_MAX);
                    prop->propLen = 0;
                    self->ctxt.pParsing++;
                    __tranConentState(self, VCARD_PARSER_CONTENT_END);
                }
                else if (__IsSkipProperty(prop->name))
                {
                    self->ctxt.propSkipFlag = 1;
                    self->ctxt.props.propCount--;
                    memset(prop->name, 0x00, VCARD_PROPERTY_LEN_MAX);
                    prop->propLen = 0;
                    self->ctxt.pParsing++;
                    __tranConentState(self, VCARD_PARSER_CONTENT_SKIP);
                }
                else
                {
                    prop->valCount++;
                    self->ctxt.pParsing++;
                    __tranConentState(self, VCARD_PARSER_CONTENT_VAL);
                }
                break;
            }

            prop->name[prop->propLen++] = *self->ctxt.pParsing++;
        }
        break;

    case VCARD_PARSER_CONTENT_PRAM:
        while (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount)
        {
            VcardProperty* prop = &self->ctxt.props.prop[self->ctxt.props.propCount - 1];
            VcardPropertyParam* param = &prop->param[prop->paramCount - 1];

            if (*self->ctxt.pParsing == '=')
            {
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_PRAMVAL);
                break;
            }

            param->param[param->paramLen++] = *self->ctxt.pParsing++;
        }
        break;

    case VCARD_PARSER_CONTENT_PRAMVAL:
        while (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount)
        {
            VcardProperty* prop = &self->ctxt.props.prop[self->ctxt.props.propCount - 1];
            VcardPropertyParam* param = &prop->param[prop->paramCount - 1];

            if (*self->ctxt.pParsing == ';')
            {
                prop->paramCount++;
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_PRAM);
                break;
            }

            if (*self->ctxt.pParsing == ':')
            {
                prop->valCount++;
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_VAL);
                break;
            }

            param->paramVal[param->paramValLen++] = *self->ctxt.pParsing++;
        }
        break;

    case VCARD_PARSER_CONTENT_VAL:
        while (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount)
        {
            VcardProperty* prop = &self->ctxt.props.prop[self->ctxt.props.propCount - 1];
            VcardPropertyVal* val = &prop->val[prop->valCount - 1];

            if (*self->ctxt.pParsing == '\r')
            {
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_CRLF);
                break;
            }

            if (*self->ctxt.pParsing == '\\')
            {
                val->val[val->valLen++] = *self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_ESC);
                break;
            }

            if (*self->ctxt.pParsing == ',' || *self->ctxt.pParsing == ';')
            {
                prop->valCount++;
                self->ctxt.pParsing++;
                break;
            }

            val->val[val->valLen++] = *self->ctxt.pParsing++;

            if (val->valLen >= VCARD_VAL_LEN_MAX - 1)
            {
                self->ctxt.propSkipFlag = 1;
                __tranConentState(self, VCARD_PARSER_CONTENT_SKIP);
                break;
            }
        }
        break;

    case VCARD_PARSER_CONTENT_ESC:
        if (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount)
        {
            if (self->ctxt.propSkipFlag)
            {
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_SKIP);
            }
            else
            {
                VcardProperty* prop = &self->ctxt.props.prop[self->ctxt.props.propCount - 1];
                VcardPropertyVal* val = &prop->val[prop->valCount - 1];

                if (*self->ctxt.pParsing == ','
                    || *self->ctxt.pParsing == '\\'
                    || *self->ctxt.pParsing == ';')
                {
                    val->val[--val->valLen] = '\0';
                }
                else if (*self->ctxt.pParsing == 'n'
                        || *self->ctxt.pParsing == 'N') // '\\' + ('n' or 'N') --> '\n'
                {
                    val->val[--val->valLen] = '\n';
                }

                if (val->valLen >= VCARD_VAL_LEN_MAX - 1)
                {
                    self->ctxt.propSkipFlag = 1;
                    __tranConentState(self, VCARD_PARSER_CONTENT_SKIP);
                    break;
                }

                val->val[val->valLen++] = *self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_VAL);
            }
        }
        break;

    case VCARD_PARSER_CONTENT_CRLF:
        if (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount
            && *self->ctxt.pParsing == '\n')
        {
            self->ctxt.pParsing++;
            __tranConentState(self, VCARD_PARSER_CONTENT_LINESTART);
        }
        break;

    case VCARD_PARSER_CONTENT_LINESTART:
        if (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount
            && (*self->ctxt.pParsing == 0x09 || *self->ctxt.pParsing == 0x20))
        {
            if (self->ctxt.propSkipFlag)
            {
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_SKIP);
            }
            else
            {
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_VAL);
            }
        }
        else
        {
            __tranConentState(self, VCARD_PARSER_CONTENT_READY);
        }
        break;

    case VCARD_PARSER_CONTENT_SKIP:
        while (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount)
        {
            if (*self->ctxt.pParsing == '\\')
            {
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_ESC);
                break;
            }

            if (*self->ctxt.pParsing == '\r')
            {
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_CRLF);
                break;
            }

            self->ctxt.pParsing++;
        }
        break;

    case VCARD_PARSER_CONTENT_END:
        while (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount)
        {
            if (*self->ctxt.pParsing == '\n')
            {
                bFinish = 1;
                //__PrintProperties(&self->ctxt.props);
                self->ctxt.pParsing++;
                __tranConentState(self, VCARD_PARSER_CONTENT_READY);
                break;
            }
            self->ctxt.pParsing++;
        }
        break;
    }

    return bFinish;
}

static void __mainStateMachine(VcardParser* self)
{
    switch (self->ctxt.mainState)
    {
    case VCARD_PARSER_MAIN_READY:
        memset(&self->ctxt.props, 0x00, sizeof(self->ctxt.props));
        __tranMainState(self, VCARD_PARSER_MAIN_BEGIN);
        break;

    case VCARD_PARSER_MAIN_BEGIN:
        if (self->ctxt.readCount - (self->ctxt.pParsing - self->ctxt.readBuffer) < strlen("BEGIN:VCARD"))
        {
            self->ctxt.dataInsufficientFlag = 1;
        }
        else
        {
            char* p = strstr(self->ctxt.pParsing, "BEGIN:VCARD");
            if (p)
            {
                self->ctxt.pParsing = p + strlen("BEGIN:VCARD");
                __tranMainState(self, VCARD_PARSER_MAIN_VERSION);
            }
            else
            {
                __tranMainState(self, VCARD_PARSER_MAIN_ERR);
            }
        }
        break;

    case VCARD_PARSER_MAIN_VERSION:
        {
            char* p = strstr(self->ctxt.pParsing, "VERSION:");
            if (p)
            {

                self->ctxt.pParsing = p + strlen("VERSION:");
                __tranMainState(self, VCARD_PARSER_MAIN_VERSION_SKIP);
            }
            else
            {
                self->ctxt.dataInsufficientFlag = 1;
            }
        }
        break;

    case VCARD_PARSER_MAIN_VERSION_SKIP:
        {
            char* p = self->ctxt.pParsing;
            int bFound = 0;

            while (p != self->ctxt.readBuffer + self->ctxt.readCount)
            {
                if (*p == 0x0d && *(p + 1) == 0x0a)
                {
                    bFound = 1;
                    break;
                }
                p++;
            }

            if (bFound)
            {
                self->ctxt.pParsing = p + 2;
                __tranMainState(self, VCARD_PARSER_MAIN_CONTENT);
            }
            else
            {
                self->ctxt.dataInsufficientFlag = 1;
            }
        }
        break;

    case VCARD_PARSER_MAIN_CONTENT:
        {
            int bFinish = 0;

            while (self->ctxt.pParsing != self->ctxt.readBuffer + self->ctxt.readCount)
            {
                bFinish = __contentStateMachine(self);
                if (bFinish)
                {
                    break;
                }
            }

            if (!bFinish || self->ctxt.pParsing == self->ctxt.readBuffer + self->ctxt.readCount)
            {
                self->ctxt.dataInsufficientFlag = 1;
            }

            if (bFinish)
            {
                if (self->ctxt.parsedCount == 0)
                {
                    self->items = __CreateVcardItem(self);
                    self->currentItem = self->items;
                }
                else
                {
                    self->currentItem->next = __CreateVcardItem(self);
                    self->currentItem = self->currentItem->next;
                }
                ++self->ctxt.parsedCount;

                if (self->callback) {
                    self->callback(VCARD_PARSER_COUNT_UPDATED, self->currentItem);
                }
                __tranMainState(self, VCARD_PARSER_MAIN_READY);
            }
        }
        break;

    case VCARD_PARSER_MAIN_ERR:

        break;
    }
}

extern BOOL g_bSrcProduceComplete;

static unsigned int __ReadData(VcardParser* self,
        char* dstBuffer,
        unsigned int dstSize,
        unsigned int prevReadSize,
        unsigned int usedSize)
{
    unsigned int readSize = 0;

    if (RingBuffer_IsEmpty(self->srcBuffer) && g_bSrcProduceComplete)
    {
        return 0;
    }

    if (prevReadSize != usedSize)
    {
        char remainBuffer[1024] = {0,};

        memcpy(remainBuffer, dstBuffer + usedSize, dstSize - usedSize);
        memcpy(dstBuffer, remainBuffer, dstSize - usedSize);

        readSize = dstSize - usedSize;
    }

    RingBuffer_Wait(self->srcBuffer);
    {
        const int sizeToRead = (self->srcBuffer->used > dstSize - readSize) ? dstSize - readSize : self->srcBuffer->used;
        readSize += RingBuffer_Pop(self->srcBuffer, (unsigned char*)(dstBuffer + readSize), sizeToRead);
    }

    return readSize;
}

static void __InitCtxt(VcardParser* self)
{
    memset(&self->ctxt, 0x00, sizeof(self->ctxt));
    self->ctxt.pParsing = self->ctxt.readBuffer;
}


void VcardParser_Init(VcardParser* self)
{
    self->srcBuffer             = NULL;
    self->type                  = VCARD_PB;
    self->items                 = NULL;
    self->currentItem           = NULL;
    self->callback              = NULL;

    __InitCtxt(self);
}


void VcardParser_StartParsing(VcardParser* self, VCARD_TYPE type, RingBuffer* srcBuffer, VcardParserEventCallback callback)
{
    self->type      = type;
    self->srcBuffer   = srcBuffer;
    self->callback  = callback;

    __InitCtxt(self);

    self->ctxt.bParsing = 1;

    while (self->ctxt.bParsing)
    {
        self->ctxt.readCount = __ReadData(self,
                                        self->ctxt.readBuffer,
                                        sizeof(self->ctxt.readBuffer),
                                        self->ctxt.readCount,
                                        self->ctxt.pParsing - self->ctxt.readBuffer);

        self->ctxt.pParsing             = self->ctxt.readBuffer;
        self->ctxt.dataInsufficientFlag = 0;

        if (self->ctxt.readCount == 0) {
            break;
        }

        while (1)
        {
            __mainStateMachine(self);
            if (self->ctxt.dataInsufficientFlag) {
                break;
            }

            Sleep(1);
        }
    }
}

void VcardParser_Stop(VcardParser* self)
{
    self->ctxt.bParsing = 0;
}
