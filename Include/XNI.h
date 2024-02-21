#ifndef XNI_H
#define XNI_H
#include <stdint.h>

typedef uint32_t xint;
typedef float xfloat;
typedef char xchar;
typedef bool xbool;

typedef struct _xobject {};
typedef _xobject* xobject;

typedef union xvalue {
    xint i;
    xfloat f;
    xchar c;
    xbool b;
    xobject o;
};

#endif