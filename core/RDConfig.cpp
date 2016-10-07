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

#include <string>

#include "RDConfig.h"
#include "RDMacros.h"
#include "RDLog.h"
#include "RDResolution.h"

// Avoid to include Rapid2D.h
extern "C" const char * Rapid2D_Version();

RDConfig *RDConfig::_instance = nullptr;

RDConfig::RDConfig()
:_maxTextureSize(0)
,_maxTextureUnits(0)
,_supportsBGRA8888(false)
,_supportsDiscardFramebuffer(false)
,_supportsNPOT(false)
,_supportsPVRTC(false)
,_supportsVAO(false)
{
}

RDConfig::~RDConfig()
{
}

RDConfig *RDConfig::getInstance(void)
{
    if (!_instance)
    {
        _instance = new RDConfig();
        _instance->getGPUInfo();
    }
    return _instance;
}

void RDConfig::getGPUInfo()
{
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &_maxTextureUnits);
    
    _supportsPVRTC = checkGLExt("GL_IMG_texture_compression_pvrtc");
    _supportsNPOT = true;
    _supportsBGRA8888 = checkGLExt("GL_IMG_texture_format_BGRA888");
    _supportsDiscardFramebuffer = checkGLExt("GL_EXT_discard_framebuffer");
    _supportsVAO = checkGLExt("vertex_array_object");
    
    RD_CHECK_GL_ERROR();
}

bool RDConfig::checkGLExt(const char *searchName)
{
    bool bRet = false;
    
    if (strstr((char *)glGetString(GL_EXTENSIONS), searchName)) {
        bRet = true;
    }
    
    return bRet;
}

void RDConfig::dumpInfo()
{
    RDLog("======= Rapid2D Config =======\n");
    
    RDLog("Rapid2D Version:%s\n", Rapid2D_Version());
    RDSize size = RDResolution::getInstance()->getFrameSize();
    RDLog("Screen Size %f x %f\n", size.width, size.height);
    RDLog("GL_VENDOR:%s\n", (char *)glGetString(GL_VENDOR));
    RDLog("GL_RENDERER:%s\n", (char *)glGetString(GL_RENDERER));
    RDLog("GL_VERSION:%s\n", (char *)glGetString(GL_VERSION));
    RDLog("GL_MAX_TEXTURE_SIZE:%d\n", _maxTextureSize);
    RDLog("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:%d\n", _maxTextureUnits);
    RDLog("supportsPVRTC:%d\n", _supportsPVRTC);
    RDLog("supportsNPOT:%d\n", _supportsNPOT);
    RDLog("supportsBGRA8888:%d\n", _supportsBGRA8888);
    RDLog("supportsDiscardFramebuffer:%d\n", _supportsDiscardFramebuffer);
    RDLog("supportsVAO:%d\n", _supportsVAO);
#if defined(RD_DEBUG)
    RDLog("Rapid2D Debug mode\n");
#else
    RDLog("Rapid2D Release mode\n");
#endif
    
    RDLog("============ end =============\n");
    
    RD_CHECK_GL_ERROR();
}
