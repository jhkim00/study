#ifndef __WINEVENTOBJECT_H__
#define __WINEVENTOBJECT_H__

#include <windows.h>
#include "EventObject.h"

typedef struct _WinEventObject
{
    EventObject super;
    HANDLE      handle;

} WinEventObject;

extern void WinEventObject_Init(WinEventObject* self, unsigned char bManualReset, unsigned char initialState);
extern void WinEventObject_Deinit(WinEventObject* self);

#endif
