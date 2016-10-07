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

#include "RDProgram.h"
#include "RDFileSystem.h"
#include "RDLog.h"


RDProgram::RDProgram()
:_program(0)
,_positionSlot(-1)
,_colorSlot(-1)
,_projectionUniform(-1)
,_modelViewUniform(-1)
,_texCoordSlot(-1)
,_textureUniform(-1)
,_aplhaUniform(-1)
,_pointSizeUniform(-1)
{
}

RDProgram::~RDProgram()
{
    if (_program) {
        glDeleteProgram(_program);
    }
}

RDProgram *RDProgram::create(const char *vshPath, const char *fshPath)
{
    RDProgram *pro = new RDProgram;
    // FIXME: only read from RD_FILEDIR_RES ? this may be move to Lua code.
    RDData *vshdata = RDFileSystem::getInstance()->getData(RD_FILEDIR_RES, vshPath, true);
    RDData *fshdata = RDFileSystem::getInstance()->getData(RD_FILEDIR_RES, fshPath, true);
    
    bool rtn = false;
    if (vshdata && fshdata) {
        rtn = pro->init(vshdata, fshdata);
    }
    
    if (vshdata) {
        delete vshdata;
    }
    if (fshdata) {
        delete fshdata;
    }
    
    if (rtn) {
        return pro;
    }
    delete pro;
    return NULL;
}

RDProgram *RDProgram::create(RDData *vshdata, RDData *fshdata)
{
    RDProgram *pro = new RDProgram;
    
    bool rtn = false;
    if (vshdata && fshdata) {
        rtn = pro->init(vshdata, fshdata);
    }
    
    if (rtn) {
        return pro;
    }
    delete pro;
    return NULL;
}

bool RDProgram::init(RDData *vshData, RDData *fshData)
{
    GLuint vertShader;
    GLuint fragShader;
    
    // Create and compile vertex shader.
    if (!compileShader(&vertShader, GL_VERTEX_SHADER, vshData)) {
        return false;
    }
    // Create and compile fragment shader.
    if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, fshData)) {
        glDeleteShader(vertShader);
        return false;
    }
    
    // Create shader program.
    _program = glCreateProgram();
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Link program.
    if (!linkProgram(_program)) {
        RDLog("[Error]: Failed to link program: %d\n", _program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_program) {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return false;
    }
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(_program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
    
    // FIXME:depend on shader GLSL code
    _positionSlot = glGetAttribLocation(_program, "Position");
    if (_positionSlot >= 0) {
        glEnableVertexAttribArray(_positionSlot);
    }
    
    _colorSlot = glGetAttribLocation(_program, "SourceColor");
    if (_colorSlot >= 0) {
        glEnableVertexAttribArray(_colorSlot);
    }
    
    _projectionUniform = glGetUniformLocation(_program, "Projection");
    _modelViewUniform = glGetUniformLocation(_program, "Modelview");
    
    _texCoordSlot = glGetAttribLocation(_program, "TexCoordIn");
    if (_texCoordSlot >= 0) {
        glEnableVertexAttribArray(_texCoordSlot);
        _textureUniform = glGetUniformLocation(_program, "Texture");
    }
    
    _aplhaUniform = glGetUniformLocation(_program, "AlphaValue");
    _pointSizeUniform = glGetUniformLocation(_program, "PointSize");
    
    RD_CHECK_GL_ERROR();
    return true;
}

bool RDProgram::compileShader(GLuint *shader, GLenum type, RDData *data)
{
    GLint status;
    const GLchar *source = (GLchar *)data->getBuffer();
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(RD_DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        RDLog("[Log]: Shader compile info:\n%s\n", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
}

bool RDProgram::linkProgram(GLuint prog)
{
    GLint status;
    glLinkProgram(prog);
    
#if defined(RD_DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        RDLog("[Log]: Program link info:\n%s\n", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}
