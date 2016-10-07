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

#include "RDProgramCache.h"
#include "RDProgram.h"

RDProgramCache *RDProgramCache::_instance = nullptr;

RDProgramCache::RDProgramCache()
{
}

RDProgramCache::~RDProgramCache()
{
}

RDProgramCache *RDProgramCache::getInstance(void)
{
    if (!_instance) {
        _instance = new RDProgramCache();
        _instance->init();
    }
    return _instance;
}

// engine shader header files
#include "RDShaderSprite.h"
#include "RDShaderVertex.h"
void RDProgramCache::init(void)
{
    // itme type
    struct shaderItem {
        const char *vsh;
        const char *fsh;
        const char *cacheName;
    };
    // items
    struct shaderItem shaderTable[] = {
        { _spriteShaderVSH, _spriteShaderFSH, "sprite" },
        { _vertexShaderVSH, _vertexShaderFSH, "vertex"}
    };
    // init programs used by engine
    for (int i = 0; i < sizeof(shaderTable) / sizeof(shaderItem); i++) {
        RDData *vsh = new RDData((RD_uchar *)strdup(shaderTable[i].vsh), strlen(shaderTable[i].vsh));
        RDData *fsh = new RDData((RD_uchar *)strdup(shaderTable[i].fsh), strlen(shaderTable[i].fsh));
        RDProgram *program = RDProgram::create(vsh, fsh);
        if (program) {
            _dict.insert(std::make_pair(shaderTable[i].cacheName, program));
        }
        delete vsh;
        delete fsh;
    }
}

RDProgram *RDProgramCache::getProgram(const char * name)
{
    for (auto &pair : _dict) {
        if (0 == strcmp(pair.first, name)) {
            return pair.second;
        }
    }
    
    RDLog("[Error]: fail to getProgram:%s\n", name);
    return NULL;
}
