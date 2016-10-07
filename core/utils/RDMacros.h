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

#ifndef __RDMacros_H__
#define __RDMacros_H__

#include <stdlib.h>
#include "RDLog.h"

#define RD_MIN(x,y) (((x) > (y)) ? (y) : (x))
#define RD_MAX(x,y) (((x) < (y)) ? (y) : (x))

#if !defined(RD_DEBUG)
    #define RD_CHECK_GL_ERROR()
#else
    #define RD_CHECK_GL_ERROR() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            RDLog("[OpenGL Error]: 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
#endif

#if !defined(RD_DEBUG)
    #define RD_ASSERT(cond, msg)
#else
    #define RD_ASSERT(cond, msg) \
    do {                         \
        if (!(cond)) {           \
            RDLog("[Assert failed]: %s\n", msg);\
        } \
    } while (0)
#endif

#define RDToString(a) #a

//#define RD_SAFE_FREE_NULL(p) do { if(p) { free(p); (p) = 0; } } while(0)

#endif // __RDMacros_H__
