#include "MutexObject.h"

static void __Lock(MutexObject* self);
static void __Unlock(MutexObject* self);

static MutexObjectVTable s_vTable = {
    __Lock,
    __Unlock
};

static void __Lock(MutexObject* self)
{
}

static void __Unlock(MutexObject* self)
{
}


void MutexObject_Init(MutexObject* self)
{
    self->vTbl = &s_vTable;
}

void MutexObject_Deinit(MutexObject* self)
{
}
