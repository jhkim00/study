//#include "SDK/TM_API_Common.h"
//#include "Common_def.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <windows.h>

#include "LinkedList.h"

MtxLinkedListNode* __MtxLinkedList_CreateNewNode(MtxLinkedListData data)
{
	static int cnt = 0;
    static MtxLinkedListNode preCreateObj[470];

	MtxLinkedListNode* newNode = NULL;

	if (cnt < ARRAY_LENGTH(preCreateObj)) {
        newNode = &preCreateObj[cnt++];
    }
    else {
		newNode = (MtxLinkedListNode*)MTX_Malloc(sizeof(MtxLinkedListNode));
	}

	newNode->data = data;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

void __MtxLinkedList_RemoveNode(MtxLinkedList* self, MtxLinkedListNode* node)
{
    if (node->next)
    {
        node->next->prev = node->prev;
    }
    if (node->prev)
    {
        node->prev->next = node->next;
    }
    if (node == self->head)
    {
        self->head = node->next;
    }
    if (node == self->tail)
    {
        self->tail = node->prev;
    }
    --self->count;
    MTX_Free(node);
}

MtxLinkedList* MtxLinkedList_Create(void)
{
    MtxLinkedList* pInst = (MtxLinkedList*)MTX_Malloc(sizeof(MtxLinkedList));

    MtxLinkedList_Init(pInst);

    return pInst;
}

void MtxLinkedList_Destroy(MtxLinkedList* self)
{
    MtxLinkedList_Clear(self);
    MTX_Free(self);
}

void MtxLinkedList_Init(MtxLinkedList* self)
{
    MTX_MemSet(self, 0x00, sizeof(MtxLinkedList));
}

void MtxLinkedList_InsertAt(MtxLinkedList* self, const MtxLinkedListData data, const int index)
{
    if (index <= self->count)
    {
        MtxLinkedListNode* newNode = __MtxLinkedList_CreateNewNode(data);
        MtxLinkedListNode* prevNode = MtxLinkedList_FindNodeByIndex(self, index - 1);

        if (prevNode)
        {
            if (prevNode->next)
            {
                prevNode->next->prev = newNode;
            }

            newNode->next = prevNode->next;
            newNode->prev = prevNode;
            prevNode->next = newNode;

            if (newNode->next == NULL)
            {
                self->tail = newNode;
            }
        }
        else
        {
            if (self->count == 0)
            {
                self->tail = newNode;
            }
            else
            {
                newNode->next = self->head;
                self->head->prev = newNode;
            }

            self->head = newNode;
        }

        ++self->count;
    }
}

void MtxLinkedList_InsertAtHead(MtxLinkedList* self, const MtxLinkedListData data)
{
	MtxLinkedListNode* newNode = __MtxLinkedList_CreateNewNode(data);

    if (self->count == 0)
    {
        self->tail = newNode;
    }
    else
    {
        self->head->prev = newNode;
        newNode->next = self->head;
    }

	self->head = newNode;
    ++self->count;
}

void MtxLinkedList_InsertAtTail(MtxLinkedList* self, const MtxLinkedListData data)
{
	MtxLinkedListNode* newNode = __MtxLinkedList_CreateNewNode(data);

    if (self->count == 0)
    {
        self->head = newNode;
    }
    else
    {
        self->tail->next = newNode;
        newNode->prev = self->tail;
    }
    self->tail = newNode;
    ++self->count;
}

void MtxLinkedList_RemoveAt(MtxLinkedList* self, const int index)
{
    MtxLinkedListNode* removingNode = MtxLinkedList_FindNodeByIndex(self, index);

    if (removingNode)
    {
        if (removingNode->prev)
        {
            removingNode->prev->next = removingNode->next;
        }
        if (removingNode->next)
        {
            removingNode->next->prev = removingNode->prev;
        }

        if (self->head == removingNode)
        {
            self->head = removingNode->next;
        }
        if (self->tail == removingNode)
        {
            self->tail = removingNode->prev;
        }

        --self->count;

        MTX_Free(removingNode);
    }
}

void MtxLinkedList_RemoveAtHead(MtxLinkedList* self)
{
    MtxLinkedList_RemoveAt(self, 0);
}

void MtxLinkedList_RemoveAtTail(MtxLinkedList* self)
{
    MtxLinkedList_RemoveAt(self, self->count - 1);
}

void MtxLinkedList_RemoveByData(MtxLinkedList* self, const MtxLinkedListData data, MtxLinkedListConditionFunc conditionFunc, BOOL bRemoveAll)
{
    MtxLinkedListNode* node = self->head;
    MtxLinkedListNode* nodeNext = NULL;
    while (node != NULL)
    {
        nodeNext = node->next;

        if (conditionFunc(node->data, data))
        {
            __MtxLinkedList_RemoveNode(self, node);
            if (bRemoveAll == FALSE)
            {
                return;
            }
        }

        node = nodeNext;
    }
}

void MtxLinkedList_Clear(MtxLinkedList* self)
{
    MtxLinkedListNode* node = self->head;
    MtxLinkedListNode* nodeNext = NULL;
    while (node != NULL)
    {
        nodeNext = node->next;
        MTX_Free(node);
        node = nodeNext;
    }
    MtxLinkedList_Init(self);
}

void MtxLinkedList_ClearWithData(MtxLinkedList* self)
{
    MtxLinkedListNode* node = self->head;
    MtxLinkedListNode* nodeNext = NULL;
    while (node != NULL)
    {
        nodeNext = node->next;
        if (node->data != NULL)
        {
            MTX_Free(node->data);
        }
        MTX_Free(node);
        node = nodeNext;
    }
    MtxLinkedList_Init(self);
}

void MtxLinkedList_Traverse(MtxLinkedList* self, MtxLinkedListAction action)
{
    if (action != NULL)
    {
        MtxLinkedListNode* node = self->head;
        MtxLinkedListNode* nodeNext = NULL;
        BOOL actionResult = TRUE;
        while (node != NULL && actionResult != FALSE)
        {
            nodeNext = node->next;
            actionResult = action(node->data);
            node = nodeNext;
        }
    }
}

MtxLinkedListNode* MtxLinkedList_FindNodeByIndex(MtxLinkedList* self, const int index)
{
    MtxLinkedListNode* node = NULL;

    if (index >= 0 && index < self->count)
    {
        const int prevIndex = self->currentIndex;
        unsigned char bMoveUp;
        int moveCount;
        int i = 0;

        if (self->current == NULL)
        {
            if (index < (self->count - 1) - index)
            {
                node = self->head;
                bMoveUp = TRUE;
                moveCount = index;
            }
            else
            {
                node = self->tail;
                bMoveUp = FALSE;
                moveCount = (self->count - 1) - index;
            }
        }
        else
        {
            if (index < prevIndex)
            {
                if (prevIndex - index < index)
                {
                    node = self->current;
                    bMoveUp = FALSE;
                    moveCount = prevIndex - index;
                }
                else
                {
                    node = self->head;
                    bMoveUp = TRUE;
                    moveCount = index;
                }
            }
            else
            {
                if (index - prevIndex < (self->count - 1) - prevIndex)
                {
                    node = self->current;
                    bMoveUp = TRUE;
                    moveCount = index - prevIndex;
                }
                else
                {
                    node = self->tail;
                    bMoveUp = FALSE;
                    moveCount = (self->count - 1) - index;
                }
            }
        }

        if (bMoveUp)
        {
            for (; i < moveCount; ++i)
            {
                node = node->next;
            }
        }
        else
        {
            for (; i < moveCount; ++i)
            {
                node = node->prev;
            }
        }
    }

    return node;
}

MtxLinkedListData MtxLinkedList_GetDataByIndex(MtxLinkedList* self, const int index)
{
    MtxLinkedListNode* node = MtxLinkedList_FindNodeByIndex(self, index);
    if (node != NULL)
    {
        return node->data;
    }
    return NULL;
}

void MtxLinkedList_SetCurrentIndex(MtxLinkedList* self, const int index)
{
    MtxLinkedListNode* node = MtxLinkedList_FindNodeByIndex(self, index);
    if (node != NULL)
    {
        self->current = node;
        self->currentIndex = index;
    }
}

BOOL MtxLinkedList_IncreaseCurrentIndex(MtxLinkedList* self)
{
    if (self->currentIndex < self->count - 1)
    {
        ++self->currentIndex;
        self->current = self->current->next;

        return TRUE;
    }
    return FALSE;
}

BOOL MtxLinkedList_DecreaseCurrentIndex(MtxLinkedList* self)
{
    if (self->currentIndex > 0)
    {
        --self->currentIndex;
        self->current = self->current->prev;

        return TRUE;
    }
    return FALSE;
}

MtxLinkedListData MtxLinkedList_GetDataOfCurrentIndex(MtxLinkedList* self)
{
    if (self->current != NULL)
    {
        return self->current->data;
    }
    return NULL;
}

BOOL MtxLinkedList_isExist(MtxLinkedList* self, const MtxLinkedListData data)
{
    MtxLinkedListNode* node = self->head;
    while (node != NULL)
    {
        if (node->data == data)
        {
            return TRUE;
        }
        node = node->next;
    }
    return FALSE;
}

BOOL MtxLinkedList_isFoundByConditionFunc(MtxLinkedList* self, const MtxLinkedListData data, MtxLinkedListConditionFunc conditionFunc)
{
    MtxLinkedListNode* node = self->head;
    while (node != NULL)
    {
        if (conditionFunc(node->data, data))
        {
            return TRUE;
        }
        node = node->next;
    }
    return FALSE;
}

void MtxLinkedList_Print(MtxLinkedList* self, MtxLinkedListAction printMethod)
{
    printf("[%s]\n", __func__);
    printf("[%s] count:%d\n", __func__, self->count);
    printf("[%s] head:0x%08x\n", __func__, (unsigned int)(self->head));
    printf("[%s] tail:0x%08x\n", __func__, (unsigned int)(self->tail));
    printf("[%s] current:0x%08x\n", __func__, (unsigned int)(self->current));

    MtxLinkedList_Traverse(self, printMethod);
}
