#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include "WinEventObject.h"
#include "WinMutexObject.h"

typedef struct _RingBuffer
{
    int size;
    int used;
    unsigned char* buffer;
    unsigned char* head;
    unsigned char* tail;
    WinEventObject eventObj;
    WinMutexObject mutexObj;

} RingBuffer;

extern void         RingBuffer_Init(RingBuffer* self, const int size);
extern void         RingBuffer_Deinit(RingBuffer* self);
extern int          RingBuffer_Push(RingBuffer* self, unsigned char* in_data, const int size);
extern int          RingBuffer_Pop(RingBuffer* self, unsigned char* out_data, const int size);
extern void         RingBuffer_Wait(RingBuffer* self);
extern int          RingBuffer_IsEmpty(RingBuffer* self);
extern int          RingBuffer_GetUsableSize(RingBuffer* self);
extern void         RingBuffer_Clear(RingBuffer* self);

#endif
