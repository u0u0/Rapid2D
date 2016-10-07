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

#ifndef __RDProgramCache_H__
#define __RDProgramCache_H__

#include <map>

class RDProgram;

class RDProgramCache
{
public:
    RDProgramCache();
    ~RDProgramCache();
    static RDProgramCache *getInstance(void);
    
    // return a ref, must not be free by caller
    RDProgram *getProgram(const char * name);
private:
    static RDProgramCache *_instance;
    std::map<const char *, RDProgram *> _dict;
    
    void init(void);
};

#endif // __RDProgramCache_H__
