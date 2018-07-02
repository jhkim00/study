#include "WinMutexObject.h"

static void __Lock(MutexObject* self);
static void __Unlock(MutexObject* self);

static MutexObjectVTable s_vTable = {
    __Lock,
    __Unlock
};

static void __Lock(MutexObject* self)
{
    WaitForSingleObject(((WinMutexObject*)self)->handle, INFINITE);
}

static void __Unlock(MutexObject* self)
{
    ReleaseMutex(((WinMutexObject*)self)->handle);
}


void WinMutexObject_Init(WinMutexObject* self)
{
    MutexObject_Init((MutexObject*)self);

    self->super.vTbl = &s_vTable;
    self->handle = CreateMutex(NULL, FALSE, NULL);
}

void WinMutexObject_Deinit(WinMutexObject* self)
{
    CloseHandle(self->handle);
    MutexObject_Deinit((MutexObject*)self);
}
