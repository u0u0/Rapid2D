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

#ifndef __RDConfig_H__
#define __RDConfig_H__

#include "RDPortingGL.h"

// engine config, GPU info, etc...

class RDConfig
{
public:
    RDConfig();
    ~RDConfig();
    
    static RDConfig *getInstance(void);
    void dumpInfo();
private:
    static RDConfig *_instance;
    // GL properties
    GLint _maxTextureSize;
    GLint _maxTextureUnits;
    bool _supportsPVRTC;
    bool _supportsNPOT;
    bool _supportsBGRA8888;
    bool _supportsDiscardFramebuffer;
    bool _supportsVAO;
    
    void getGPUInfo();
    bool checkGLExt(const char *searchName);
};

#endif // __RDConfig_H__
