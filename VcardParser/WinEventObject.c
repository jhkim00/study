#include "WinEventObject.h"

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
    SetEvent(((WinEventObject*)self)->handle);
}

static void __ResetEvent(EventObject* self)
{
    ResetEvent(((WinEventObject*)self)->handle);
}

static void __Wait(EventObject* self, unsigned int time)
{
    WaitForSingleObject(((WinEventObject*)self)->handle, time == -1 ? INFINITE : time);
}


void WinEventObject_Init(WinEventObject* self, unsigned char bManualReset, unsigned char initialState)
{
    EventObject_Init((EventObject*)self, bManualReset, initialState);

    self->super.vTbl = &s_vTable;
    self->handle = CreateEvent(NULL,    // default security attributes
		bManualReset ? TRUE : FALSE,    // manual-reset event
		initialState ? TRUE : FALSE,    // initial state is nonsignaled
		NULL                            // object name
	);
}

void WinEventObject_Deinit(WinEventObject* self)
{
    CloseHandle(self->handle);
    EventObject_Deinit((EventObject*)self);
}
