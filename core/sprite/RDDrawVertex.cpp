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

#include <stdlib.h>

#include "RDDrawVertex.h"
#include "RDProgramCache.h"
#include "RDProgram.h"
#include "RDDirector.h"
#include "RDMacros.h"
#include "RDResolution.h"

RDDrawVertex::RDDrawVertex()
:_program(NULL)
{
    _program = RDProgramCache::getInstance()->getProgram("vertex");
    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_indexBuffer);
}

RDDrawVertex::~RDDrawVertex()
{
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_indexBuffer);
}

RDDrawVertex *RDDrawVertex::create(RDBasicVertex *vertex,
                                   size_t count,
                                   int drawType,
                                   float pointSize)
{
    RDDrawVertex *drawVertex = new RDDrawVertex;
    if (drawVertex->init(vertex, count, drawType, pointSize)) {
        return drawVertex;
    }
    delete drawVertex;
    return NULL;
}

bool RDDrawVertex::init(RDBasicVertex *vertex,
                        size_t count,
                        int drawType,
                        float pointSize)
{
    if (!vertex) {
        return false;
    }
    _count = count;
    _pointSize = pointSize;
    switch (drawType) {
        case 0:
            _drawMode = GL_POINTS;
            break;
        case 1:
            _drawMode = GL_LINES;
            break;
        case 2:
            _drawMode = GL_LINE_LOOP;
            break;
        case 3:
            _drawMode = GL_LINE_STRIP;
            break;
        case 4:
            _drawMode = GL_TRIANGLES;
            break;
        case 5:
            _drawMode = GL_TRIANGLE_STRIP;
            break;
        case 6:
            _drawMode = GL_TRIANGLE_FAN;
            break;
        default:
            return false;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RDBasicVertex) * _count, vertex, GL_STATIC_DRAW);
    
    // vertex draw order
    GLubyte *indices = (GLubyte *)malloc(sizeof(GLubyte) * _count);
    for (GLubyte i = 0; i < _count; i++) {
        indices[i] = i;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * _count, indices, GL_STATIC_DRAW);
    free(indices);
    return true;
}

void RDDrawVertex::draw(RDMat4 *modelView)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(_program->getProgram());
    
    // use projection
    RDMat4 *projection = RDDirector::getInstance()->getProjection(RD_Projection_2D);
    glUniformMatrix4fv(_program->getProjectionUniform(), 1, GL_FALSE, projection->mat);
    // ModelView
    glUniformMatrix4fv(_program->getModelviewUniform(), 1, GL_FALSE, modelView->mat);
    // position
    glVertexAttribPointer(_program->getPositionSlot(), 3, GL_FLOAT, GL_FALSE,
                          sizeof(RDBasicVertex), 0);
    // color
    glVertexAttribPointer(_program->getColorSlot(), 4, GL_FLOAT, GL_FALSE,
                          sizeof(RDBasicVertex), (GLvoid *)offsetof(RDBasicVertex, color));
    
    // point size
    glUniform1f(_program->getPointSizeUniform(),
                _pointSize * RDResolution::getInstance()->getScale());
    
    glDrawElements(_drawMode, _count, GL_UNSIGNED_BYTE, 0);
    
    RD_CHECK_GL_ERROR();
}
