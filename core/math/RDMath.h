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

#ifndef __RDMath_H__
#define __RDMath_H__

// wrap from cocos3d

#include "RDkmMath.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =================== below for projection  ===================== */
// fill mat4 with a parallel projection matrix.
void RDMath_OrthographicProjection(RDMat4 *mat4,
                                GLfloat left,
                                GLfloat right,
                                GLfloat bottom,
                                GLfloat top,
                                GLfloat near,
                                GLfloat far);
// fill mat4 with a perspective projection matrix.
void RDMath_PerspectiveProjection(RDMat4 *mat4,
                                GLfloat left,
                                GLfloat right,
                                GLfloat bottom,
                                GLfloat top,
                                GLfloat near,
                                GLfloat far);

/* =================== below for ModelView  ===================== */
void RDMath_modelView(RDMat4 *pOut, RDVec3 *translation, RDVec3 *rotation, RDVec3 *scale);
    
/* ===== color ====== */
// init color by RGBA
void RDColorInit(RDColor *color, RD_uchar r, RD_uchar g, RD_uchar b, RD_uchar a);
// rdcolor Range in [0, 255], glcolor Range in [0, 1]
void RDColorToGL(RDColor *rdcolor, RDGLColor *glColor);

/* ===== RDVec3 ====== */
RDVec3 RDVec3Make(GLfloat x, GLfloat y, GLfloat z);


#ifdef __cplusplus
}
#endif

#endif // __RDMath_H__
