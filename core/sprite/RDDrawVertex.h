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

#ifndef __RDDrawVertex_H__
#define __RDDrawVertex_H__

#include "RDTypes.h"
#include "RDPortingGL.h"

class RDProgram;

class RDDrawVertex
{
public:
    RDDrawVertex();
    ~RDDrawVertex();
    static RDDrawVertex *create(RDBasicVertex *vertex,
                                size_t count,
                                int drawType,
                                float pointSize);
    
    virtual void draw(RDMat4 *modelView);
private:
    // ref only, manage by cache.
    RDProgram *_program;
    
    // OpenGL GL_ELEMENT_ARRAY_BUFFER
    GLuint _indexBuffer;
    // OpenGL VBO
    GLuint _vertexBuffer;
    // how many vertexs
    size_t _count;
    // GL_POINTS need this
    float _pointSize;
    // glDrawElements mode
    GLenum _drawMode;
    
    bool init(RDBasicVertex *vertex,
              size_t count,
              int drawType,
              float pointSize);
};

#endif // __RDDrawVertex_H__
