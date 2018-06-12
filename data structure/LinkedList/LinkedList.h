#ifndef __MTX_LINKED_LIST_H__
#define __MTX_LINKED_LIST_H__

#include "Common.h"

typedef void* MtxLinkedListData;
typedef struct _MtxLinkedListNode MtxLinkedListNode;
typedef struct _MtxLinkedList MtxLinkedList;
typedef BOOL (*MtxLinkedListAction)(MtxLinkedListData data);
typedef BOOL (*MtxLinkedListConditionFunc)(MtxLinkedListData data1, MtxLinkedListData data2);

struct _MtxLinkedListNode
{
	MtxLinkedListData  data;
	MtxLinkedListNode* next;
	MtxLinkedListNode* prev;
};

struct _MtxLinkedList
{
    MtxLinkedListNode*  head;
    MtxLinkedListNode*  tail;
    MtxLinkedListNode*  current;
    int                 currentIndex;
    int                 count;
};

extern MtxLinkedList* MtxLinkedList_Create(void);
extern void MtxLinkedList_Destroy(MtxLinkedList* self);
extern void MtxLinkedList_Init(MtxLinkedList* self);
extern void MtxLinkedList_InsertAt(MtxLinkedList* self, const MtxLinkedListData data, const int index);
extern void MtxLinkedList_InsertAtHead(MtxLinkedList* self, const MtxLinkedListData data);
extern void MtxLinkedList_InsertAtTail(MtxLinkedList* self, const MtxLinkedListData data);
extern void MtxLinkedList_RemoveAt(MtxLinkedList* self, const int index);
extern void MtxLinkedList_RemoveAtHead(MtxLinkedList* self);
extern void MtxLinkedList_RemoveAtTail(MtxLinkedList* self);
extern void MtxLinkedList_RemoveByData(MtxLinkedList* self, const MtxLinkedListData data, MtxLinkedListConditionFunc conditionFunc, BOOL bRemoveAll);
extern void MtxLinkedList_Clear(MtxLinkedList* self);
extern void MtxLinkedList_ClearWithData(MtxLinkedList* self);
extern void MtxLinkedList_Traverse(MtxLinkedList* self, MtxLinkedListAction action);
extern MtxLinkedListNode* MtxLinkedList_FindNodeByIndex(MtxLinkedList* self, const int index);
extern MtxLinkedListData MtxLinkedList_GetDataByIndex(MtxLinkedList* self, const int index);
extern void MtxLinkedList_SetCurrentIndex(MtxLinkedList* self, const int index);
extern BOOL MtxLinkedList_IncreaseCurrentIndex(MtxLinkedList* self);
extern BOOL MtxLinkedList_DecreaseCurrentIndex(MtxLinkedList* self);
extern MtxLinkedListData MtxLinkedList_GetDataOfCurrentIndex(MtxLinkedList* self);
extern BOOL MtxLinkedList_isExist(MtxLinkedList* self, const MtxLinkedListData data);
extern BOOL MtxLinkedList_isFoundByConditionFunc(MtxLinkedList* self, const MtxLinkedListData data, MtxLinkedListConditionFunc conditionFunc);
extern void MtxLinkedList_Print(MtxLinkedList* self, MtxLinkedListAction printMethod);

#endif
