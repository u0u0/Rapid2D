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

#ifndef __RDAudio_H__
#define __RDAudio_H__

#include "RDTypes.h"

/****** For iOS ********/
#ifdef RD_PLATFORM_IOS
#include <OpenAL/OpenAL.h>
#endif // end of RD_PLATFORM_IOS

/****** For Mac ********/
#ifdef RD_PLATFORM_MAC
#include <OpenAL/OpenAL.h>
#endif // end of RD_PLATFORM_MAC

/****** For Win32 ********/
#ifdef RD_PLATFORM_WIN32
#include "alext.h"
#endif // end of RD_PLATFORM_WIN32

/****** For Android ********/
#ifdef RD_PLATFORM_ANDROID
#include "alext.h"
#endif // end of RD_PLATFORM_ANDROID

typedef struct RDPcmData {
    RD_uchar *data;
    RD_int32 channels;
    RD_int32 rate;
    size_t size;
} RDPcmData;

#ifdef __cplusplus
extern "C" {
#endif

void RDAudio_init(void);
/* return bufferID, if 0 fail */
ALuint RDAudio_LoadFile(const char *path);

#ifdef __cplusplus
}
#endif

#endif // __RDAudio_H__
