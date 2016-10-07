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

#import <Foundation/Foundation.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "RDPortingFile.h"

RD_uchar *RDPortingFile_getData(const char *fullPath, bool isTextMode, size_t *outSize)
{
    RD_uchar *buffer = NULL;
    *outSize = 0;
    
    FILE *fp = fopen(fullPath, "rb");
    if (NULL == fp) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    *outSize = ftell(fp);
    fseek(fp,0,SEEK_SET);

    size_t mallocSize = *outSize;
    if (isTextMode) {
        mallocSize += 1;
    }
    buffer = (RD_uchar *)malloc(sizeof(RD_uchar) * mallocSize);
    if (NULL == buffer) {
        fclose(fp);
        return NULL;
    }
    memset(buffer, 0, mallocSize);
    fread(buffer, sizeof(RD_uchar), *outSize, fp);
    fclose(fp);
    return buffer;
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
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    const char *utf8Dir = [documentsDirectory UTF8String];
    return strdup(utf8Dir);
}

char *RDPortingFile_getBundlePath()
{
    NSString *dir = [[NSBundle mainBundle] resourcePath];
    return strdup([dir UTF8String]);
}
