/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2002             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: #ifdef jail to whip a few platforms into the UNIX ideal.
 last mod: $Id: os_types.h 19098 2014-02-26 19:06:45Z giles $

 ********************************************************************/
#ifndef _OS_TYPES_H
#define _OS_TYPES_H

#include "RDPortingTypes.h"

/* make it easy on the folks that want to compile the libs with a
   different malloc than stdlib */
#define _ogg_malloc  malloc
#define _ogg_calloc  calloc
#define _ogg_realloc realloc
#define _ogg_free    free

typedef RD_int16 ogg_int16_t;
typedef RD_uint16 ogg_uint16_t;
typedef RD_int32 ogg_int32_t;
typedef RD_uint32 ogg_uint32_t;
typedef RD_int64 ogg_int64_t;
typedef RD_uint64 ogg_uint64_t;

#endif  /* _OS_TYPES_H */
