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

#ifndef __RDProgram_H__
#define __RDProgram_H__

#include "RDPortingGL.h"
#include "RDData.h"

// GLSL Loader

class RDProgram
{
public:
    RDProgram();
    ~RDProgram();
    static RDProgram *create(const char *vshPath, const char *fshPath);
    static RDProgram *create(RDData *vshData, RDData *fshData);
    
    GLuint getProgram(void) {return _program;};
    GLint getPositionSlot(void) {return _positionSlot;};
    GLint getColorSlot(void) {return _colorSlot;};
    GLint getProjectionUniform(void) {return _projectionUniform;};
    GLint getModelviewUniform(void) {return _modelViewUniform;};
    GLint getTexCoordSlot(void) {return _texCoordSlot;};
    GLint getTextureUniform(void) {return _textureUniform;};
    GLint getAlphaUniform(void) {return _aplhaUniform;};
    GLint getPointSizeUniform(void) {return _pointSizeUniform;};
private:
    GLuint _program;
    GLint _positionSlot;// -1 for not set
    GLint _colorSlot;// -1 for not set
    GLint _projectionUniform;// -1 for not set
    GLint _modelViewUniform;// -1 for not set
    GLint _texCoordSlot;// -1 for not set
    GLint _textureUniform;// -1 for not set
    GLint _aplhaUniform;// -1 for not set
    GLint _pointSizeUniform;// -1 for not set
    
    bool init(RDData *vshData, RDData *fshData);
    bool compileShader(GLuint *shader, GLenum type, RDData *data);
    bool linkProgram(GLuint prog);
};

#endif // __RDProgram_H__
