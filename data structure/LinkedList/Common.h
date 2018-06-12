#ifndef __COMMON_H__
#define __MTX_LINKED_LIST_H__

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 1
#endif

#define ARRAY_LENGTH(a) sizeof(a)/sizeof(a[0])
#define MTX_Malloc      malloc
#define MTX_Free        free
#define MTX_MemSet      memset

#endif
