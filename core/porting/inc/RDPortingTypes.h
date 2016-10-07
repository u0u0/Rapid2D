// Copyright 2016 KeNan Liu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __RDPortingTypes_h__
#define __RDPortingTypes_h__

/****** For iOS, Mac ********/
#if defined(RD_PLATFORM_IOS) || defined(RD_PLATFORM_MAC) || defined(RD_PLATFORM_ANDROID)
#include <inttypes.h>
#include <sys/types.h>

typedef unsigned char RD_uchar;
typedef int16_t RD_int16;
typedef uint16_t RD_uint16;
typedef int32_t RD_int32;
typedef uint32_t RD_uint32;
typedef int64_t RD_int64;
typedef uint64_t RD_uint64;

#endif

/****** end of For iOS, Mac ********/

#if defined(RD_PLATFORM_WIN32)
typedef unsigned char RD_uchar;
typedef short RD_int16;
typedef unsigned short RD_uint16;
typedef int RD_int32;
typedef unsigned int RD_uint32;
typedef long long RD_int64;
typedef unsigned long long RD_uint64;

#include <crtdefs.h> // define size_t

#if _MSC_VER < 1900
// make VS happy
#define strdup _strdup
#define strcasecmp _stricmp
#define snprintf _snprintf
#endif

#define strcasecmp _stricmp

#define M_PI (3.14159265358)
#define M_PI_2 (1.57079632679)

#endif

#endif /* __RDPortingTypes_h__ */
