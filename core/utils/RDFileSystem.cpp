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

#include <string.h>
#include <stdlib.h>

#include "RDFileSystem.h"
#include "RDPortingFile.h"
#include "RDLog.h"

RDFileSystem *RDFileSystem::_instance = nullptr;

RDFileSystem::RDFileSystem()
:_documentsPath(NULL)
,_bundlePath(NULL)
{
    _documentsPath = RDPortingFile_getDocumentsPath();
    _bundlePath = RDPortingFile_getBundlePath();
    _luaPath =  appendPath(_bundlePath, "lua");
    _resPath =  appendPath(_bundlePath, "res");
}

RDFileSystem::~RDFileSystem()
{
    if (_documentsPath) {
        free(_documentsPath);
    }
    if (_bundlePath) {
        free(_bundlePath);
    }
    if (_luaPath) {
        free(_luaPath);
    }
    if (_resPath) {
        free(_resPath);
    }
}

RDFileSystem *RDFileSystem::getInstance(void)
{
    if (!_instance) {
        _instance = new RDFileSystem();
    }
    return _instance;
}

RDData *RDFileSystem::getData(RD_FileDir type, const char *path,  bool isTextMode)
{
    const char *fullPath = NULL;
    if (type == RD_FILEDIR_FULLPATH) {
        fullPath = path;
    } else {
        fullPath = getFullPath(type, path);
    }
    
	if (!fullPath) {
		return NULL;
	}
    
    size_t size = 0;
    RD_uchar *buffer = RDPortingFile_getData(fullPath, isTextMode, &size);
    if (type != RD_FILEDIR_FULLPATH) {
        free((void *)fullPath);
    }
    
    if (buffer) {
        RDData *data = new RDData(buffer, size);
		if (isTextMode && 239 == buffer[0] && 187 == buffer[1] && 191 == buffer[2])
		{
			data->setBOM(true);
		}
        return data;
    }
#ifdef RD_DEBUG
    RDLog("[Error]: RDFileSystem::getData read fail for: %s\n", path);
#endif
    return NULL;
}

bool RDFileSystem::putData(const char *path, const char *buffer, size_t size)
{
    char *fullPath = getFullPath(RD_FILEDIR_DATA, path);
    if (!fullPath) {
        return false;
    }
    
    size_t putSize = RDPortingFile_putData(fullPath, (RD_uchar *)buffer, size);
    if (putSize != size) {
        RDLog("[Error]: RDFileSystem::putData write fail: %s\n", path);
        return false;
    }
    
#ifdef RD_DEBUG
    RDLog("RDFileSystem::putData write at: %s\n", fullPath);
#endif
    return true;
}

char *RDFileSystem::appendPath(const char *path1, const char *path2)
{
    size_t size1 = strlen(path1);
    size_t size2 = strlen(path2);
    // one for '/'. one for '0'
    size_t mallocSize = size1 + size2 + 2;
    char *rtnStr = (char *)malloc(mallocSize);
    memset(rtnStr, 0, mallocSize);
    
    strncpy(rtnStr, path1, size1);
    size_t endCharIndex = size1 - 1;
    if (path1[endCharIndex] != '/') {
        // insert '/' into paths
        rtnStr[++endCharIndex] = '/';
    }
    strncpy(rtnStr + endCharIndex + 1, path2, size2);
    return rtnStr;
}

RD_FileType RDFileSystem::getFileType(const char *fileName)
{
    RD_FileType type = RD_FILETYPE_UNKNOWN;
    
    // get extension
    const char *pDot = strrchr(fileName, '.');
    if (NULL == pDot) {
        return type;
    }
    
    if(0 == strcasecmp(pDot, ".png")) {
        return RD_FILETYPE_PNG;
    }
    if(0 == strcasecmp(pDot, ".ogg")) {
        return RD_FILETYPE_OGG;
    }
    // add more type here

    return type;
}

char *RDFileSystem::getFullPath(RD_FileDir type, const char *path)
{
	if (!path || strlen(path) == 0) {
		return NULL;
	}

	// convert to fullPath
	char *dirPath = NULL;
	switch (type) {
	case RD_FILEDIR_RES:
		dirPath = _resPath;
		break;
	case RD_FILEDIR_LUA:
		dirPath = _luaPath;
		break;
	case RD_FILEDIR_DATA:
		dirPath = _documentsPath;
		break;
	default:
		return NULL;
	}
	
	return appendPath(dirPath, path);
}
