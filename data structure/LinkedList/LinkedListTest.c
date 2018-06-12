#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include "LinkedList.h"
#include "LinkedListTest.h"

static BOOL __PrintData(MtxLinkedListData data)
{
    printf("[%s] data:%d\n", __func__, *((int*)data));

    return TRUE;
}

static BOOL __CompData(MtxLinkedListData data1, MtxLinkedListData data2)
{
    if (*(int*)data1 == *(int*)data2)
    {
        return TRUE;
    }
    return FALSE;
}

void mtxLinkedList_Test(void)
{
    MtxLinkedList* list = NULL;
    int testVal[] = { 1, 30, 4, 55, 8, 61, 92, 12, 43, 27 };
    int* pTestVal[10] = {0,};
    int i;

    freopen("linkedlisttest.txt", "w", stdout);

    list = MtxLinkedList_Create();
    assert(list);

    for (i = 0; i < 10; ++i)
    {
        pTestVal[i] = &testVal[i];
    }

    printf("[%s] testVals\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        printf("[%s] testVal[%d]:%d\n", __func__, i, *pTestVal[i]);
    }
    printf("\n");

    printf("[%s] insert testVals at 0\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        MtxLinkedList_InsertAt(list, pTestVal[i], 0);
        //MtxLinkedList_Print(list, __PrintData);
        //printf("\n");
    }
    MtxLinkedList_Print(list, __PrintData);
    printf("\n");

    printf("[%s] clear\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        MtxLinkedList_Clear(list);        
    }
    MtxLinkedList_Print(list, __PrintData);
    printf("\n");

    printf("[%s] insert testVals at tail\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        MtxLinkedList_InsertAtTail(list, pTestVal[i]);
        //MtxLinkedList_Print(list, __PrintData);
        //printf("\n");
    }
    MtxLinkedList_Print(list, __PrintData);
    printf("\n");

    printf("[%s] remove at 0\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        MtxLinkedList_RemoveAt(list, 0);
        MtxLinkedList_Print(list, __PrintData);
        printf("\n");
    }

    printf("[%s] insert testVals at head\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        MtxLinkedList_InsertAtHead(list, pTestVal[i]);        
    }
    MtxLinkedList_Print(list, __PrintData);
    printf("\n");

    printf("[%s] remove at 1\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        MtxLinkedList_RemoveAt(list, 1);
        MtxLinkedList_Print(list, __PrintData);
        printf("\n");
    }

    printf("[%s] insert testVals at count\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        MtxLinkedList_InsertAt(list, pTestVal[i], list->count);        
    }
    MtxLinkedList_Print(list, __PrintData);
    printf("\n");

    printf("[%s] removeByData\n", __func__);
    for (i = 0; i < 10; ++i)
    {
        MtxLinkedList_RemoveByData(list, pTestVal[i], __CompData);
        MtxLinkedList_Print(list, __PrintData);
        printf("\n");
    }    

    fclose(stdout);
}
