#include <stdio.h>
#include <stdlib.h>
#include "VcardParser.h"

#define __CASE(x) case x: return #x

static VcardParser s_vcardParser;

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

int main()
{
    freopen("dbg.txt", "w", stdout);

    printf("== start ==\n");

    VcardParser_Init(&s_vcardParser);
    VcardParser_StartParsing(&s_vcardParser, VCARD_PB, "pbDump.txt", __vcardParserEventCallback);

    printf("== finish ==\n");
    return 0;
}

/* TODO

1. handling value len over limit.
2. handling parsing error state.
3. decoding quoted printable.

*/
