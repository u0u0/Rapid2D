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

#ifndef __RDkmMath_H__
#define __RDkmMath_H__

// wrap from kazmath

#include "RDTypes.h"

/** Conversion between degrees and radians. */
#define DegreesToRadiansFactor  0.017453292519943f			// PI / 180
#define RadiansToDegreesFactor  57.29577951308232f			// 180 / PI
#define DegreesToRadians(D) ((D) * DegreesToRadiansFactor)
#define RadiansToDegrees(R) ((R) * RadiansToDegreesFactor)

#ifdef __cplusplus
extern "C" {
#endif

/* Returns the length of the vector. */
GLfloat RDVec3Length(const RDVec3 *pIn);

/* Normalizes the vector to unit length, stores the result in pOut and returns the result. */
RDVec3 *RDVec3Normalize(RDVec3 *pOut, const RDVec3 *pIn);

/* get translation vec3 from Mat4 */
RDVec3 *RDMat4extractTranslation(RDVec3 *pOut, const RDMat4 *pIn);

/* get Rotation vec3 from Mat4 */
RDVec3 *RDMat4extractRotation(RDVec3 *pOut, const RDMat4 *pIn);

/* get Scale vec3 from Mat4 */
RDVec3 *RDMat4extractScale(RDVec3 *pOut, const RDMat4 *pIn);

/* Multiplies pM1 with pM2, stores the result in pOut, returns pOut. */
RDMat4 *RDMat4Multiply(RDMat4 *pOut, const RDMat4 *pM1, const RDMat4 *pM2);

/* get Inversed Mat4 */
RDMat4 *RDMat4Inversed(RDMat4 *pOut, const RDMat4 *pIn);

/* transform a RDVec3 Point by mat4 */
RDVec3 *RDMat4TransformVec3(RDVec3 *outPoint, const RDMat4 *pIn, const RDVec3 *point);

/*
 * Builds a rotation matrix that rotates around all three axes, y (yaw), x (pitch) and z (roll),
 * in that order, stores the result in pOut and returns the result.
 * This algorithm matches up along the positive Y axis, which is the OpenGL ES default.
 */
RDMat4 *RDMat4RotationYXZ(RDMat4 *pOut, const GLfloat xRadians, const GLfloat yRadians, const GLfloat zRadians);

/* Builds a rotation matrix around the X-axis, stores the result in pOut and returns the result */
RDMat4 *RDMat4RotationX(RDMat4 *pOut, const float radians);

/* Builds a rotation matrix around the Y-axis, stores the result in pOut and returns the result */
RDMat4 *RDMat4RotationY(RDMat4 *pOut, const float radians);

/* Builds a rotation matrix around the Z-axis, stores the result in pOut and returns the result */
RDMat4 *RDMat4RotationZ(RDMat4 *pOut, const float radians);

/*
 * Build a rotation matrix from an axis and an angle, 
 * stores the result in pOut and returns the result.
 */
RDMat4 *RDMat4RotationAxisAngle(RDMat4 *pOut, const RDVec3 *axis, GLfloat radians);

/*
 * Builds a rotation matrix from a quaternion to a rotation matrix,
 * stores the result in pOut and returns the result.
 */
RDMat4* RDMat4RotationQuaternion(RDMat4 *pOut, const RDQuaternion *pQ);

/** Extracts a quaternion from a rotation matrix, stores the result in quat and returns the result */
RDQuaternion *RDQuaternionRotationMatrix(RDQuaternion *quat, const RDMat4 *pIn);

/*
 * Builds a transformation matrix that translates, rotates and scales according to the specified vectors,
 * stores the result in pOut and returns the result.
 */
RDMat4 *RDMat4Transformation(RDMat4 *pOut, RDVec3 *translation, RDVec3 *rotation, RDVec3 *scale);

/* Gauss-Jordan matrix inversion function */
int RDGaussJordan(RDMat4 *a, RDMat4 *b);

/* Get the value from the matrix at the specfied row and column. */
GLfloat RDMatGet(const RDMat4 *pIn, int row, int col);

/* Set the value into the matrix at the specfied row and column. */
void RDMatSet(RDMat4 *pIn, int row, int col, GLfloat value);

/* Swap the elements in the matrix at the specfied row and column coordinates. */
void RDMatSwap(RDMat4 *pIn, int r1, int c1, int r2, int c2);

#ifdef __cplusplus
}
#endif

#endif // __RDkmMath_H__
