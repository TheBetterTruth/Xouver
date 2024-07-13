#ifndef XI_H
#define XI_H

/*
    This is the Xouver-Integrity header.

    Use this header to create and manage instances of the xouver virtual machine
    as well as to develop native libraries

*/

// for precise integer definitions to use for xint, xshort, xlong
#include <stdint.h>

typedef unsigned char _xchar;
typedef int16_t _xshort;
typedef int32_t _xint;
typedef float _xfloat;
typedef int64_t _xlong;
typedef double _xdouble;
typedef struct _xobject {};
typedef struct _xstring : _xobject {};

typedef _xchar* xchar;
typedef _xshort* xshort;
typedef _xint* xint;
typedef _xfloat* xfloat;
typedef _xlong* xlong;
typedef _xdouble* xdouble;
typedef _xobject* xobject;
typedef _xstring* xstring;

typedef union xval {
    xchar c;
    xshort s;
    xint i;
    xfloat f;
    xlong l;
    xdouble d;
    xobject o;
};

#endif