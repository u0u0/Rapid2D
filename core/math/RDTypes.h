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

#ifndef __RDTypes_H__
#define __RDTypes_H__

#include "RDPortingTypes.h"
#include "RDPortingGL.h"

typedef struct {
    GLfloat x;
    GLfloat y;
} RDPoint;

typedef struct {
    GLfloat width;
    GLfloat height;
} RDSize;

typedef struct {
    RDPoint origin;
    RDSize size;
} RDRect;

/* A three-dimensional vector. OpenGL coordinate */
typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} RDVec3;

/* Rapid2D color, [0, 255] */
typedef struct {
    RD_uchar r;
    RD_uchar g;
    RD_uchar b;
    RD_uchar a;
} RDColor;

/* OpenGL color, [0, 1] */
typedef struct {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
} RDGLColor;

/* OpenGL Texture coordinate */
typedef struct {
    GLfloat s;
    GLfloat t;
} RDTexCoord;

/* A rotational quaternion */
typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} RDQuaternion;

/* A standard 4x4 matrix */
typedef struct {
    GLfloat mat[16];
} RDMat4;

typedef struct {
    RDVec3 position;
    RDGLColor color;
} RDBasicVertex;

typedef struct {
    RDVec3 position;
    RDGLColor color;
    RDTexCoord texcoord;
} RDVertex;


#endif // __RDTypes_H__
