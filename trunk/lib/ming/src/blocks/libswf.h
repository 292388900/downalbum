/*
    Ming, an SWF output library
    Copyright (C) 2001  Opaque Industries - http://www.opaque.net/

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* libswf.h - global typedefs, etc. */

/* $Id: libswf.h,v 1.7 2002/08/07 23:37:55 opaque_ndst Exp $ */

#ifndef LIBSWF_H_INCLUDED
#define LIBSWF_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "error.h"

typedef unsigned char BOOL;

#ifndef TRUE
  #define TRUE ((BOOL)1)
#endif

#ifndef FALSE
  #define FALSE ((BOOL)0)
#endif

#ifndef M_PI
  #define M_PI 3.14159265358979f
#endif

#ifndef min
  #define min(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef max
  #define max(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef rint
  #define rint(d) (floor((d)+0.5))
#endif

typedef unsigned char byte;

#endif /* LIBSWF_H_INCLUDED */
