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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

// for native asset manager
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "RDPortingFile.h"

// work for data files (writable)
static RD_uchar *getDocData(const char *fullPath, bool isTextMode, size_t *outSize)
{
	RD_uchar *buffer = NULL;
	*outSize = 0;

	const char* mode = NULL;
	if (isTextMode) {
		mode = "rt";
	}
	else {
		mode = "rb";
	}

	FILE *fp = fopen(fullPath, mode);
	if (NULL == fp) {
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	*outSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	size_t mallocSize = *outSize;
	if (isTextMode) {
		mallocSize += 1;
	}
	buffer = (RD_uchar *)malloc(sizeof(RD_uchar)* mallocSize);
	if (NULL == buffer) {
		fclose(fp);
		return NULL;
	}
	memset(buffer, 0, mallocSize);
	fread(buffer, sizeof(RD_uchar), *outSize, fp);
	fclose(fp);
	return buffer;
}

// saved value from java->ndk
extern const char *g_androidFilesDir;
extern AAssetManager *g_AssetMgr;
// use to delteck where the file come from
#define PREFIX_LENGTH (6)
static const char *boundlePrefix = "apk://";

RD_uchar *RDPortingFile_getData(const char *fullPath, bool isTextMode, size_t *outSize)
{
	if (0 == strncmp(fullPath, boundlePrefix, PREFIX_LENGTH)) {
		// open file
		AAsset *pAsset = AAssetManager_open(g_AssetMgr,
				fullPath + PREFIX_LENGTH,
				AASSET_MODE_UNKNOWN);
		if (pAsset == NULL) {
			return NULL;
		}
		// get file size
		*outSize = AAsset_getLength(pAsset);
		size_t mallocSize = *outSize;
		if (isTextMode) {
			mallocSize += 1;
		}
		// malloc
		RD_uchar *buffer = (RD_uchar *)malloc(sizeof(RD_uchar)* mallocSize);
		if (NULL == buffer) {
			AAsset_close(pAsset);
			return NULL;
		}
		memset(buffer, 0, mallocSize);
		// read file
		int ret = AAsset_read(pAsset, buffer, *outSize);
		if(ret <= 0) {
			free(buffer);
			buffer = NULL;
		}
		// close file
		AAsset_close(pAsset);
		return buffer;
	} else {
		// XXX: not test yet
		return getDocData(fullPath, isTextMode, outSize);
	}
}

size_t RDPortingFile_putData(const char *fullPath, RD_uchar *buffer, size_t size)
{
    FILE *fp = fopen(fullPath, "wb");
    if (NULL == fp) {
        return 0;
    }
    
    size_t wroteBlocks = fwrite(buffer, size, 1, fp);
    fclose(fp);
    return wroteBlocks == 1 ? size : 0;
}

char *RDPortingFile_getDocumentsPath()
{
	char * temp = strdup(g_androidFilesDir);
	free((void *)g_androidFilesDir);
	g_androidFilesDir = NULL;
	return temp;
}

char *RDPortingFile_getBundlePath()
{
	return strdup(boundlePrefix);
}
