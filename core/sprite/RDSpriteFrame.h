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

#ifndef __RDSpriteFrame_h__
#define __RDSpriteFrame_h__

#include "RDTypes.h"
#include "RDTexture.h"

typedef struct {
    char *name;
    bool rotated;// direction clockwise, The amount of rotation is always 90°.
    bool trimmed;// frameSize is small than soruceSize
    // frame.origin is left up corner
    RDRect frame;// coordinates of original picture in texture
    RDRect source;// coordinate of trimmed pic in original picture
    RDSize sourceSize;// original picture size
    RDTexture *texture;// Ref only
} RDSpriteFrame;

#endif /* __RDSpriteFrame_h__ */
