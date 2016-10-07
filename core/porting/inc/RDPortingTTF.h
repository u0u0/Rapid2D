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

#ifndef __RDPortingTTF_h__
#define __RDPortingTTF_h__

#include "RDTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    RD_uchar *buffer;// r,g,b,a
    int width;
    int height;
    bool isPremultipliedAlpha;
} RD_Bitmap32;

bool RDPortingTTF_getBitmap32(const char *utf8_text,
                            const char *fontName,
                            float fontSize,
                            RDColor *fontColor,// 0~255
                            int align,// 0 left, 1 center, 2 right
                            RD_Bitmap32 *outMap);

#ifdef __cplusplus
}
#endif


#endif /* __RDPortingTTF_h__ */
