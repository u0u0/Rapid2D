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

#ifndef __Rapid2D_H__
#define __Rapid2D_H__

// API for startup in project directory
// hide engine detail

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

const char * Rapid2D_Version();
void Rapid2D_setFrameSize(float width, float height);
// mode:
//      0  , FixedHeight
//      1  , FixedWidth
void Rapid2D_setResolutionModeSize(unsigned int mode, float width, float height);
/* name: game chunks, locate in RD_FILEDIR_RES top level
 *      if NULL, load from discrete files
 * key: is for xxtea decrypt
 */
void Rapid2D_start(const char *name, const char *key);
void Rapid2D_stop(void);

/**************** for porting ******************/

/* type
 *  "began"
 *  "moved"
 *  "ended"
 *  IMPORTANT: make sure this is call fram MAIN THREAD
 */
void Rapid2D_touchEvent(char *type, int count, intptr_t ids[], float xs[], float ys[]);

/* type
 *  "down"
 *  "up"
 *  IMPORTANT: make sure this is call fram MAIN THREAD
 *  keyCode see "glfw3.h"
 */
void Rapid2D_keyBoardEvent(char *type, int keyCode);

#ifdef __cplusplus
}
#endif

#endif // __Rapid2D_H__
