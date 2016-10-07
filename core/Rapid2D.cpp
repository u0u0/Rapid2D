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

#include "Rapid2D.h"
#include "RDResolution.h"
#include "RDDirector.h"
#include "RDLog.h"
#include "RDFileSystem.h"
#include "RDLua.h"
#include "RDConfig.h"
#include "RDAudio.h"

const char * Rapid2D_Version()
{
    return "Rapid2D 0.1";
}

void Rapid2D_setFrameSize(float width, float height)
{
    RDResolution::getInstance()->setFrameSize(width, height);
}

void Rapid2D_setResolutionModeSize(unsigned int mode, float width, float height)
{
    if (mode >= RD_Resolution_UnKnown) {
        RDLog("[Error]:Resolution mode must be 0 or 1\n");
        return;
    }
    RDResolution::getInstance()->setResolutionModeSize((RD_ResolutionMode)mode, width, height);
}

void Rapid2D_start(const char *name, const char *key)
{
    // init OpenAL
    RDAudio_init();
    // dump GPU info
    RDConfig::getInstance()->dumpInfo();
    // init lua binding
    RDLuaInit(name, key);
    // start main loop
    RDDirector::getInstance()->start();
}

void Rapid2D_stop(void)
{
    RDDirector::getInstance()->stop();
}

// IMPORTANT: make sure this is call fram MAIN THREAD
void Rapid2D_touchEvent(char *type, int count, intptr_t ids[], float xs[], float ys[])
{
    RDLuaTouchEvent(type, count, ids, xs, ys);
}

// IMPORTANT: make sure this is call fram MAIN THREAD
void Rapid2D_keyBoardEvent(char *type, int keyCode)
{
    RDLuaKeyBoardEvent(type, keyCode);
}
