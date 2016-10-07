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

#ifndef __RDFileSystem_H__
#define __RDFileSystem_H__

#include "RDData.h"

typedef enum {
    RD_FILETYPE_UNKNOWN = -1,
    
    // texture file type
    RD_FILETYPE_TEXTURE_START,
    RD_FILETYPE_PNG,
    RD_FILETYPE_TEXTURE_END,
    // end of texture type
    
    // music type
    RD_FILETYPE_MUSIC_START,
    RD_FILETYPE_OGG,
    RD_FILETYPE_MUSIC_END,
}RD_FileType;

typedef enum {
    // texture file type
    RD_FILEDIR_RES, // place picture,font etc.
    RD_FILEDIR_LUA, // place lua source file.
    RD_FILEDIR_DATA,// place writeable data.
    RD_FILEDIR_FULLPATH, // no need to convert
    // end of texture type
}RD_FileDir;

class RDFileSystem
{
public:
    RDFileSystem();
    ~RDFileSystem();
    static RDFileSystem *getInstance(void);
    
    // get data from res dir, delete data by caller
    RDData *getData(RD_FileDir type, const char *path, bool isTextMode = false);
    // write data to documentPath
    bool putData(const char *path, const char *buffer, size_t size);
    // auto insert '/' into path1 and path2
    char *appendPath(const char *path1, const char *path2);
    RD_FileType getFileType(const char *fileName);
	char *getFullPath(RD_FileDir type, const char *path);
private:
    static RDFileSystem *_instance;
    
    char *_documentsPath;
    char *_bundlePath;
    char *_luaPath;
    char *_resPath;
};

#endif // __RDFileSystem_H__
