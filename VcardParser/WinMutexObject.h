#ifndef __WINMUTEXOBJECT_H__
#define __WINMUTEXOBJECT_H__

#include <windows.h>
#include "MutexObject.h"

typedef struct _WinMutexObject
{
    MutexObject super;
    HANDLE      handle;

} WinMutexObject;

extern void WinMutexObject_Init(WinMutexObject* self);
extern void WinMutexObject_Deinit(WinMutexObject* self);

#endif
