#ifndef __EVENTOBJECT_H__
#define __EVENTOBJECT_H__

typedef struct _EventObject EventObject;

typedef struct _EventObjectVTable
{
    void(*setEvent)(EventObject*);
    void(*resetEvent)(EventObject*);
    void(*wait)(EventObject*, unsigned int);

} EventObjectVTable;

struct _EventObject
{
    unsigned char       bManualReset:1; // 1: manual reset, 0: auto reset
    unsigned char       initialState:1; // 1: signaled, 0: none signaled
    EventObjectVTable*  vTbl;
};

extern void EventObject_Init(EventObject* self, unsigned char bManualReset, unsigned char initialState);
extern void EventObject_Deinit(EventObject* self);

#endif
