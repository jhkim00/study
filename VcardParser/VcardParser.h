#ifndef VCARD_PARSER_H__
#define VCARD_PARSER_H__

#include <stdlib.h>
#include <stdio.h>


#define VCARD_NAME_LEN_MAX          128
#define VCARD_NUMBER_LEN_MAX        32
#define VCARD_PROPERTY_LEN_MAX      32
#define VCARD_PARAM_LEN_MAX         32
#define VCARD_PARAMVAL_LEN_MAX      32
#define VCARD_PARAM_COUNT_MAX       8
#define VCARD_VAL_LEN_MAX           128
#define VCARD_VAL_COUNT_MAX         8
#define VCARD_PROPERTY_COUNT_MAX    16


typedef enum _VCARD_TEL_TYPE
{
    VCARD_TEL_CELL,
    VCARD_TEL_HOME,
    VCARD_TEL_WORK,
    VCARD_TEL_ETC,

    VCARD_TEL_TYPE_MAX,

} VCARD_TEL_TYPE;

typedef enum _VCARD_TYPE
{
    VCARD_PB,
    VCARD_CL_IN,
    VCARD_CL_OUT,
    VCARD_CL_MISSED,

    VCARD_TYPE_MAX,

} VCARD_TYPE;

typedef enum _VCARD_PARSER_EVENT
{
    VCARD_PARSER_STARTED,
    VCARD_PARSER_COUNT_UPDATED,
    VCARD_PARSER_FINISHED,

} VCARD_PARSER_EVENT;

typedef enum _VCARD_PARSER_MAIN_STATUS
{
    VCARD_PARSER_MAIN_READY,
    VCARD_PARSER_MAIN_BEGIN,
    VCARD_PARSER_MAIN_VERSION,
    VCARD_PARSER_MAIN_VERSION_SKIP,
    VCARD_PARSER_MAIN_CONTENT,
    VCARD_PARSER_MAIN_ERR,

} VCARD_PARSER_MAIN_STATUS;

typedef enum _VCARD_PARSER_CONTENT_STATUS
{
    VCARD_PARSER_CONTENT_READY,
    VCARD_PARSER_CONTENT_PROP,
    VCARD_PARSER_CONTENT_PRAM,
    VCARD_PARSER_CONTENT_PRAMVAL,
    VCARD_PARSER_CONTENT_VAL,
    VCARD_PARSER_CONTENT_ESC,
    VCARD_PARSER_CONTENT_CRLF,
    VCARD_PARSER_CONTENT_LINESTART,
    VCARD_PARSER_CONTENT_SKIP,
    VCARD_PARSER_CONTENT_END,

} VCARD_PARSER_CONTENT_STATUS;


typedef struct _VcardPropertyParamItem
{
    unsigned int    paramLen;
    unsigned int    paramValLen;
    char            param[VCARD_PARAM_LEN_MAX];
    char            paramVal[VCARD_PARAMVAL_LEN_MAX];

} VcardPropertyParam;

typedef struct _VcardPropertyValItem
{
    unsigned int    valLen;
    char            val[VCARD_VAL_LEN_MAX];

} VcardPropertyVal;

typedef struct _VcardPropertyItem
{
    unsigned int       propLen;
    unsigned int       paramCount;
    unsigned int       valCount;
    char               name[VCARD_PROPERTY_LEN_MAX];
    VcardPropertyParam param[VCARD_PARAM_COUNT_MAX];
    VcardPropertyVal   val[VCARD_VAL_COUNT_MAX];

} VcardProperty;

typedef struct _VcardParsingItem
{
    unsigned int        propCount;
    VcardProperty       prop[VCARD_PROPERTY_COUNT_MAX];
} VcardProperties;

typedef struct _VcardParsingContext
{
    VCARD_PARSER_MAIN_STATUS    mainState;
    VCARD_PARSER_CONTENT_STATUS contentState;
    VcardProperties             props;
    char                        readBuffer[1024];
    char*                       pParsing;
    unsigned int                readCount;
    unsigned int                parsedCount;
    unsigned char               propSkipFlag;
    unsigned char               dataInsufficientFlag;
    unsigned char               bParsingFinished;
} VcardParsingContext;

typedef struct _VcardNumber
{
    VCARD_TEL_TYPE type;
    char number[VCARD_NUMBER_LEN_MAX];

} VcardNumber;

typedef struct _VcardItem
{
    VCARD_TYPE          type;
    char                name[VCARD_NAME_LEN_MAX];
    struct _VcardItem*  next;

} VcardItem;

typedef struct _VcardPbItem
{
    VcardItem       super;
    VcardNumber     number[VCARD_TEL_TYPE_MAX];
    unsigned int    numberCount;

} VcardPbItem;

typedef struct _VcardClItem
{
    VcardItem   super;
    VcardNumber number;

} VcardClItem;

typedef void(*VcardParserEventCallback)(VCARD_PARSER_EVENT event, void* param);

typedef struct _VcardParser
{
    FILE*                       srcFile;
    VCARD_TYPE                  type;
    VcardItem*                  items;
    VcardItem*                  currentItem;
    VcardParserEventCallback    callback;
    VcardParsingContext         ctxt;

} VcardParser;


extern void VcardParser_Init(VcardParser* self);
extern void VcardParser_StartParsing(VcardParser* self, VCARD_TYPE type, const char* srcFileName, VcardParserEventCallback callback);

#endif // VCARD_PARSER_H__
