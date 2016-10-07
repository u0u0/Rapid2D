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

#include "RDIndexBuffer.h"
#include "RDLog.h"

RDIndexBuffer *RDIndexBuffer::_instance = nullptr;

RDIndexBuffer::RDIndexBuffer()
{
}

RDIndexBuffer::~RDIndexBuffer()
{
}

RDIndexBuffer *RDIndexBuffer::getInstance(void)
{
    if (!_instance) {
        _instance = new RDIndexBuffer();
        _instance->init();
    }
    return _instance;
}

void RDIndexBuffer::init(void)
{
    // vertex index buffer for sprite
    const GLubyte indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    _dict.insert(std::make_pair("sprite", buffer));
}

GLuint RDIndexBuffer::getVIB(const char *name)
{
    for (auto &pair : _dict) {
        if (0 == strcmp(pair.first, name)) {
            return pair.second;
        }
    }
    
    RDLog("[Error]: fail to getVIB:%s\n", name);
    return -1;
}
