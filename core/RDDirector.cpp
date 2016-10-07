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

#include "RDPortingMainLoop.h"
#include "RDMacros.h"
#include "RDDirector.h"
#include "RDResolution.h"
#include "RDProgramCache.h"
#include "RDMath.h"
#include "RDLua.h"

/* local C function */
static void portingMainLoop(void)
{
    RDDirector::getInstance()->drawScene();
}

RDDirector *RDDirector::_instance = nullptr;
RDMat4 RDDirector::_projection2D = {0};

RDDirector::RDDirector()
:_deltaTime(0.0f)
,_lastUpdate({0})
{
}

RDDirector::~RDDirector()
{
}

RDDirector *RDDirector::getInstance(void)
{
    if (!_instance) {
        _instance = new RDDirector();
        // init 2D projection
        RDSize size = RDResolution::getInstance()->getResolutionSize();
        RDMath_OrthographicProjection(&_projection2D,
                                      -size.width / 2.0f,
                                      size.width / 2.0f,
                                      -size.height / 2.0f,
                                      size.height / 2.0f,
                                      -1024.0f,
                                      1024.0f);
    }
    return _instance;
}

void RDDirector::drawScene(void)
{
    // calculate "global" dt
    calculateDeltaTime();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // call lua to draw the scene
    RDLuaDraw(_deltaTime);
}

void RDDirector::calculateDeltaTime(void)
{
    struct timeval now;
    
    if (gettimeofday(&now, nullptr) != 0) {
        RDLog("[Error]: while calling gettimeofday\n");
        _deltaTime = 0;
        return;
    }
    
    if (_lastUpdate.tv_sec == 0) {
        _deltaTime = 0;
    } else {
        _deltaTime = (now.tv_sec - _lastUpdate.tv_sec) + (now.tv_usec - _lastUpdate.tv_usec) / 1000000.0f;
    }
    
    _lastUpdate = now;
}

/*
 * set OpenGL default values
 */
void RDDirector::initGL(void)
{
    // loading OpenGL programs
    RDProgramCache::getInstance();
    
    // close for 2D rendering, make alpha blend happy for two sprite
    glDisable(GL_DEPTH_TEST);
    
    // viewport for fullscreen
    RDSize frameSize = RDResolution::getInstance()->getFrameSize();
    glViewport(0, 0, frameSize.width, frameSize.height);
    
    RD_CHECK_GL_ERROR();
}

void RDDirector::start(void)
{
    initGL();
    RDPortingMainLoop_start(portingMainLoop);
}

void RDDirector::stop(void)
{
    RDPortingMainLoop_stop();
}

RDMat4 *RDDirector::getProjection(RD_ProjectionType type)
{
    switch (type) {
        case RD_Projection_2D:
            return &_projection2D;
            break;
            
        default:
            return NULL;
            break;
    }
}
