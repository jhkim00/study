#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "RingBuffer.h"

void RingBuffer_Init(RingBuffer* self, const int size)
{
    self->size = size;
    self->used = 0;

    self->buffer = (unsigned char*)malloc(size);
    if (self->buffer == NULL)
    {
        free(self);
        self = NULL;
    }
    else
    {
        memset(self->buffer, 0x00, size);
        self->head = self->buffer;
        self->tail = self->buffer;

        WinEventObject_Init(&self->eventObj, 1, 0);
        WinMutexObject_Init(&self->mutexObj);
    }
}

void RingBuffer_Deinit(RingBuffer* self)
{
    WinEventObject_Deinit(&self->eventObj);
    WinMutexObject_Deinit(&self->mutexObj);
    free(self->buffer);
}

int RingBuffer_Push(RingBuffer* self, unsigned char* in_data, const int size)
{
    self->mutexObj.super.vTbl->lock((MutexObject*)&self->mutexObj);

    if (size > 0 && size <= self->size - self->used)
    {
        const int firstCopiableSize = (self->buffer + self->size) - self->tail;

        if (firstCopiableSize == size)
        {
            memcpy(self->tail, in_data, size);
            self->tail = self->buffer;
        }
        else if (firstCopiableSize > size)
        {
            memcpy(self->tail, in_data, size);
            self->tail += size;
        }
        else
        {
            memcpy(self->tail, in_data, firstCopiableSize);
            memcpy(self->buffer, in_data + firstCopiableSize, size - firstCopiableSize);
            self->tail = self->buffer + size - firstCopiableSize;
        }

        self->used += size;

        self->eventObj.super.vTbl->setEvent((EventObject*)&self->eventObj);
        self->mutexObj.super.vTbl->unlock((MutexObject*)&self->mutexObj);

        return size;
    }

    self->mutexObj.super.vTbl->unlock((MutexObject*)&self->mutexObj);

    return 0;
}

int RingBuffer_Pop(RingBuffer* self, unsigned char* out_data, const int size)
{
    self->mutexObj.super.vTbl->lock((MutexObject*)&self->mutexObj);

    if (size > 0 && size <= self->used)
    {
        const int firstCopiableSize = (self->buffer + self->size) - self->head;

        if (firstCopiableSize == size)
        {
            memcpy(out_data, self->head, size);
            self->head = self->buffer;
        }
        else if (firstCopiableSize > size)
        {
            memcpy(out_data, self->head, size);
            self->head += size;
        }
        else
        {
            memcpy(out_data, self->head, firstCopiableSize);
            memcpy(out_data + firstCopiableSize, self->buffer, size - firstCopiableSize);
            self->head = self->buffer + size - firstCopiableSize;
        }

        self->used -= size;

        if (self->used == 0)
        {
            self->eventObj.super.vTbl->resetEvent((EventObject*)&self->eventObj);
        }

        self->mutexObj.super.vTbl->unlock((MutexObject*)&self->mutexObj);

        return size;
    }

    return 0;
}

void RingBuffer_Wait(RingBuffer* self)
{
    self->eventObj.super.vTbl->wait((EventObject*)&self->eventObj, -1);
}

int RingBuffer_IsEmpty(RingBuffer* self)
{
    return self->used == 0 ? 1 : 0;
}

int RingBuffer_GetUsableSize(RingBuffer* self)
{
    return self->size - self->used;
}

void RingBuffer_Clear(RingBuffer* self)
{
    self->mutexObj.super.vTbl->lock((MutexObject*)&self->mutexObj);

    self->used = 0;
    self->head = self->buffer;
    self->tail = self->buffer;

    self->eventObj.super.vTbl->resetEvent((EventObject*)&self->eventObj);
    self->mutexObj.super.vTbl->unlock((MutexObject*)&self->mutexObj);
}

