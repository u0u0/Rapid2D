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

#ifndef __RDPortingFile_h__
#define __RDPortingFile_h__

#include "RDPortingTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* return file buffer, need release by core.
 * if isTextMode, return a buffer terminal by \0
 */
RD_uchar *RDPortingFile_getData(const char *fullPath, bool isTextMode, size_t *outSize);

/* write buffer to file, binary mode 
 * return wrote size
 */
size_t RDPortingFile_putData(const char *fullPath, RD_uchar *buffer, size_t size);

/* a Writable directory in sandbox for mobile app.
 * return file path in UTF8 encoding, need release by core.
 */
char *RDPortingFile_getDocumentsPath();

/* a ReadOnly directory in sandbox for mobile app.
 * return file path in UTF8 encoding, need release by core.
 */
char *RDPortingFile_getBundlePath();

#ifdef __cplusplus
}
#endif


#endif /* __RDPortingFile_h__ */
