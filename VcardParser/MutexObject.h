#ifndef __MUTEXOBJECT_H__
#define __MUTEXOBJECT_H__

typedef struct _MutexObject MutexObject;

typedef struct _MutexObjectVTable
{
    void(*lock)(MutexObject*);
    void(*unlock)(MutexObject*);

} MutexObjectVTable;

struct _MutexObject
{
    MutexObjectVTable*  vTbl;

};

extern void MutexObject_Init(MutexObject* self);
extern void MutexObject_Deinit(MutexObject* self);

#endif
