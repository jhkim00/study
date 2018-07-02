#include "EventObject.h"

static void __SetEvent(EventObject* self);
static void __ResetEvent(EventObject* self);
static void __Wait(EventObject* self, unsigned int time);

static EventObjectVTable s_vTable = {
    __SetEvent,
    __ResetEvent,
    __Wait
};

static void __SetEvent(EventObject* self)
{
}

static void __ResetEvent(EventObject* self)
{
}

static void __Wait(EventObject* self, unsigned int time)
{
}


void EventObject_Init(EventObject* self, unsigned char bManualReset, unsigned char initialState)
{
    self->bManualReset = bManualReset;
    self->initialState = initialState;
    self->vTbl = &s_vTable;
}

void EventObject_Deinit(EventObject* self)
{
}
