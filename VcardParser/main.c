#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include "VcardParser.h"

#define __CASE(x) case x: return #x

static VcardParser s_vcardParser;
static RingBuffer s_vcardBuffer;
BOOL g_bSrcProduceComplete;

static const char* __Str_VCARD_TEL_TYPE(VCARD_TEL_TYPE type)
{
    switch (type)
    {
    __CASE(VCARD_TEL_CELL);
    __CASE(VCARD_TEL_HOME);
    __CASE(VCARD_TEL_WORK);
    __CASE(VCARD_TEL_ETC);

    default: return "unknown";
    }
}

static const char* __Str_VCARD_TYPE(VCARD_TYPE type)
{
    switch (type)
    {
    __CASE(VCARD_PB);
    __CASE(VCARD_CL_IN);
    __CASE(VCARD_CL_OUT);
    __CASE(VCARD_CL_MISSED);

    default: return "unknown";
    }
}

void __vcardParserEventCallback(VCARD_PARSER_EVENT event, void* param)
{
    switch (event)
    {
    case VCARD_PARSER_STARTED:
        break;

    case VCARD_PARSER_COUNT_UPDATED:
        printf("    count:%d\n", s_vcardParser.ctxt.parsedCount);
        {
            VcardItem* item = (VcardItem*)param;
            if (item)
            {
                switch (s_vcardParser.type)
                {
                case VCARD_PB:
                    printf("    name:%s\n", item->name);
                    {
                        VcardPbItem* pb = (VcardPbItem*)item;

                        int i;
                        for (i = 0; i < pb->numberCount; ++i)
                        {
                            printf("    number[%d]:(%s:%s)\n", i, __Str_VCARD_TEL_TYPE(pb->number[i].type), pb->number[i].number);
                        }
                    }
                    printf("\n");
                    break;
                case VCARD_CL_IN:
                    break;
                case VCARD_CL_OUT:
                    break;
                case VCARD_CL_MISSED:
                    break;

                default:
                    break;
                }
            }
        }
        break;

    case VCARD_PARSER_FINISHED:
        break;
    }
}


HANDLE s_thread;
unsigned int s_threadId;
volatile BOOL s_bThreadRun;
static unsigned int _stdcall __VcardProduce(void* arg)
{
    FILE* srcFile = fopen("pbDump.txt", "rb");

    if (srcFile == NULL)
    {
        return 0;
    }

    s_bThreadRun = TRUE;
    g_bSrcProduceComplete = FALSE;

    Sleep(100);

    while (1)
    {
        if (!s_bThreadRun)
        {
            break;
        }

        {
            unsigned int usableSize = RingBuffer_GetUsableSize(&s_vcardBuffer);

            if (usableSize > 0)
            {
                unsigned char buffer[1024] = {0,};
                unsigned int readSize = 0;

                readSize = fread(buffer, sizeof(char), usableSize, srcFile);
                if (readSize == 0)
                {
                    s_bThreadRun = FALSE;
                    g_bSrcProduceComplete = TRUE;
                    break;
                }
                RingBuffer_Push(&s_vcardBuffer, buffer, readSize);
            }

            Sleep(1);
        }
    }

    fclose(srcFile);

	printf("[%s] __VcardProduce ended\n", __func__);

	return 0;
}


int main()
{
    freopen("dbg.txt", "w", stdout);

    printf("== start ==\n");

    RingBuffer_Init(&s_vcardBuffer, 1024);

    s_thread = (HANDLE)_beginthreadex(NULL, 0, __VcardProduce, NULL, 0, &s_threadId);

    VcardParser_Init(&s_vcardParser);
    VcardParser_StartParsing(&s_vcardParser, VCARD_PB, &s_vcardBuffer, __vcardParserEventCallback);

    s_bThreadRun = FALSE;
    WaitForSingleObject(s_thread, INFINITE);
    CloseHandle(s_thread);
    s_thread = NULL;
    s_threadId = 0;

    RingBuffer_Deinit(&s_vcardBuffer);

    printf("== finish ==\n");
    return 0;
}

/* TODO

1. handling value len over limit. (O)
2. handling parsing error state.
3. decoding quoted printable.

*/
