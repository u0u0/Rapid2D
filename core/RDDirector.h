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

#ifndef __RDDirector_H__
#define __RDDirector_H__

#include "RDPortingTime.h"
#include "RDTypes.h"

typedef enum {
    RD_Projection_2D,
}RD_ProjectionType;

class RDDirector
{
public:
    RDDirector();
    ~RDDirector();
    
    static RDDirector *getInstance(void);
    void start(void);
    void stop(void);
    // drawScene is call by mainLoop
    void drawScene(void);
    RDMat4 *getProjection(RD_ProjectionType type);
    
private:
    static RDDirector *_instance;
    static RDMat4 _projection2D;
    float _deltaTime;
    struct timeval _lastUpdate;
    
    void initGL(void);
    /** calculates delta time since last time it was called */
    void calculateDeltaTime(void);
};

#endif // __RDDirector_H__
