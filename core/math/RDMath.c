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

#include "RDMath.h"

void RDMath_OrthographicProjection(RDMat4 *mat4,
                                   GLfloat left,
                                   GLfloat right,
                                   GLfloat bottom,
                                   GLfloat top,
                                   GLfloat near,
                                   GLfloat far)
{
    mat4->mat[0]  = 2.0f / (right - left);
    mat4->mat[1]  = 0.0f;
    mat4->mat[2]  = 0.0f;
    mat4->mat[3] = 0.0f;
    
    mat4->mat[4]  = 0.0f;
    mat4->mat[5]  = 2.0f / (top - bottom);
    mat4->mat[6]  = 0.0f;
    mat4->mat[7] = 0.0f;
    
    mat4->mat[8]  = 0.0f;
    mat4->mat[9]  = 0.0f;
    mat4->mat[10]  = -2.0f / (far - near);
    mat4->mat[11] = 0.0f;
    
    mat4->mat[12]  = -(right + left) / (right - left);
    mat4->mat[13]  = -(top + bottom) / (top - bottom);
    mat4->mat[14] = -(far + near) / (far - near);
    mat4->mat[15] = 1.0f;
}

void RDMath_PerspectiveProjection(RDMat4 *mat4,
                                  GLfloat left,
                                  GLfloat right,
                                  GLfloat bottom,
                                  GLfloat top,
                                  GLfloat near,
                                  GLfloat far)
{
    mat4->mat[0]  = (2.0f * near) / (right - left);
    mat4->mat[1]  = 0.0f;
    mat4->mat[2]  = 0.0f;
    mat4->mat[3] = 0.0f;
    
    mat4->mat[4]  = 0.0f;
    mat4->mat[5]  = (2.0f * near) / (top - bottom);
    mat4->mat[6]  = 0.0f;
    mat4->mat[7] = 0.0f;
    
    mat4->mat[8]  = (right + left) / (right - left);
    mat4->mat[9]  = (top + bottom) / (top - bottom);
    mat4->mat[10] = -(far + near) / (far - near);
    mat4->mat[11] = -1.0f;
    
    mat4->mat[12]  = 0.0f;
    mat4->mat[13]  = 0.0f;
    mat4->mat[14] = -(2.0f * far * near) / (far - near);
    mat4->mat[15] = 0.0f;
}

// use for modelview Multiply, in OpenGL ES 1.0, is glLoadIdentity()
static RDMat4 s_identityMat4 = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

void RDMath_modelView(RDMat4 *pOut, RDVec3 *translation, RDVec3 *rotation, RDVec3 *scale)
{
    // TODO :check translation and rotation and scale are not zero???
    
    RDVec3 rdRotation = RDVec3Make(DegreesToRadians(rotation->x),
                                   DegreesToRadians(rotation->y),
                                   DegreesToRadians(rotation->z));
    RDMat4 outTransformation;
    RDMat4Transformation(&outTransformation, translation, &rdRotation, scale);
    RDMat4Multiply(pOut, &s_identityMat4, &outTransformation);
}

void RDColorInit(RDColor *color, RD_uchar r, RD_uchar g, RD_uchar b, RD_uchar a)
{
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}

void RDColorToGL(RDColor *rdcolor, RDGLColor *glColor)
{
    glColor->r = rdcolor->r / 255.0f;
    glColor->g = rdcolor->g / 255.0f;
    glColor->b = rdcolor->b / 255.0f;
    glColor->a = rdcolor->a / 255.0f;
}

RDVec3 RDVec3Make(GLfloat x, GLfloat y, GLfloat z)
{
    RDVec3 vec3;
    vec3.x = x;
    vec3.y = y;
    vec3.z = z;
    return vec3;
}
