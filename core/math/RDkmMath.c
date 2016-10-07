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

#include <stdlib.h>
#include <math.h>

#include "RDkmMath.h"

// Returns the length of the vector.
GLfloat RDVec3Length(const RDVec3 *pIn)
{
    return sqrtf((pIn->x * pIn->x) + (pIn->y * pIn->y) + (pIn->z * pIn->z));
}

// Normalizes the vector to unit length, stores the result in pOut and returns the result.
RDVec3 *RDVec3Normalize(RDVec3 *pOut, const RDVec3 *pIn)
{
    GLfloat l = 1.0f / RDVec3Length(pIn);
    
    RDVec3 v;
    v.x = pIn->x * l;
    v.y = pIn->y * l;
    v.z = pIn->z * l;
    
    pOut->x = v.x;
    pOut->y = v.y;
    pOut->z = v.z;
    
    return pOut;
}

RDVec3 *RDMat4extractTranslation(RDVec3 *pOut, const RDMat4 *pIn)
{
    pOut->x = pIn->mat[12];
    pOut->y = pIn->mat[13];
    pOut->z = pIn->mat[14];
    return pOut;
}

// YXZ order for OpenGL default
/*
 |  cycz + sxsysz   czsxsy - cysz   cxsy  0 |
 M = |  cxsz            cxcz           -sx    0 |
 |  cysxsz - czsy   cyczsx + sysz   cxcy  0 |
 |  0               0               0     1 |
 
 where cA = cos(A), sA = sin(A) for A = x,y,z
 */
RDVec3 *RDMat4extractRotation(RDVec3 *pOut, const RDMat4 *pIn)
{
    GLfloat radX, radY, radZ;
    GLfloat cxsz = pIn->mat[1];
    GLfloat cxcz = pIn->mat[5];
    GLfloat sx = -pIn->mat[9];
    GLfloat cxsy = pIn->mat[8];
    GLfloat cxcy = pIn->mat[10];
    if (sx < +1.0) {
        if (sx > -1.0) {
            radX = asin(sx);
            radY = atan2(cxsy, cxcy);
            radZ = atan2(cxsz, cxcz);
        } else {
            // sx = -1. Not a unique solution: radZ + radY = atan2(-m01,m00).
            radX = -M_PI_2;
            radY = atan2(-pIn->mat[4], pIn->mat[0]);
            radZ = 0.0;
        }
    } else {
        // sx = +1. Not a unique solution: radZ - radY = atan2(-m01,m00).
        radX = +M_PI_2;
        radY = -atan2(-pIn->mat[4], pIn->mat[0]);
        radZ = 0.0;
    }
    
    pOut->x = RadiansToDegrees(radX);
    pOut->y = RadiansToDegrees(radY);
    pOut->z = RadiansToDegrees(radZ);
    return pOut;
}

static GLfloat determinant(const RDMat4 *mat4)
{
    const GLfloat *m = mat4->mat;
    GLfloat a0 = m[0] * m[5] - m[1] * m[4];
    GLfloat a1 = m[0] * m[6] - m[2] * m[4];
    GLfloat a2 = m[0] * m[7] - m[3] * m[4];
    GLfloat a3 = m[1] * m[6] - m[2] * m[5];
    GLfloat a4 = m[1] * m[7] - m[3] * m[5];
    GLfloat a5 = m[2] * m[7] - m[3] * m[6];
    GLfloat b0 = m[8] * m[13] - m[9] * m[12];
    GLfloat b1 = m[8] * m[14] - m[10] * m[12];
    GLfloat b2 = m[8] * m[15] - m[11] * m[12];
    GLfloat b3 = m[9] * m[14] - m[10] * m[13];
    GLfloat b4 = m[9] * m[15] - m[11] * m[13];
    GLfloat b5 = m[10] * m[15] - m[11] * m[14];
    
    // Calculate the determinant.
    return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

RDVec3 *RDMat4extractScale(RDVec3 *pOut, const RDMat4 *pIn)
{
    // Extract the scale.
    // This is simply the length of each axis (row/column) in the matrix.
    pOut->x = sqrt(pIn->mat[0] * pIn->mat[0] + pIn->mat[1] * pIn->mat[1] + pIn->mat[2] * pIn->mat[2]);
    pOut->y = sqrt(pIn->mat[4] * pIn->mat[4] + pIn->mat[5] * pIn->mat[5] + pIn->mat[6] * pIn->mat[6]);
    pOut->z = sqrt(pIn->mat[8] * pIn->mat[8] + pIn->mat[9] * pIn->mat[9] + pIn->mat[10] * pIn->mat[10]);
    
    // Determine if we have a negative scale (true if determinant is less than zero).
    // In this case, we simply negate a single axis of the scale.
    GLfloat det = determinant(pIn);
    if (det < 0) {
        pOut->z = -pOut->z;
    }
    
    return pOut;
}

// Multiplies pM1 with pM2, stores the result in pOut, returns pOut
RDMat4 *RDMat4Multiply(RDMat4 *pOut, const RDMat4 *pM1, const RDMat4 *pM2)
{
    const float *m1 = pM1->mat, *m2 = pM2->mat;
    GLfloat *m = pOut->mat;
    
    m[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    m[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    m[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    m[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
    
    m[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    m[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    m[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    m[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
    
    m[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    m[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    m[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    m[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
    
    m[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    m[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    m[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    m[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
    
    return pOut;
}

static void multiplyMatrix(const GLfloat* m, GLfloat scalar, GLfloat *dst)
{
    dst[0]  = m[0]  * scalar;
    dst[1]  = m[1]  * scalar;
    dst[2]  = m[2]  * scalar;
    dst[3]  = m[3]  * scalar;
    dst[4]  = m[4]  * scalar;
    dst[5]  = m[5]  * scalar;
    dst[6]  = m[6]  * scalar;
    dst[7]  = m[7]  * scalar;
    dst[8]  = m[8]  * scalar;
    dst[9]  = m[9]  * scalar;
    dst[10] = m[10] * scalar;
    dst[11] = m[11] * scalar;
    dst[12] = m[12] * scalar;
    dst[13] = m[13] * scalar;
    dst[14] = m[14] * scalar;
    dst[15] = m[15] * scalar;
}

#define MATH_TOLERANCE              2e-37f

RDMat4 *RDMat4Inversed(RDMat4 *pOut, const RDMat4 *pIn)
{
    const GLfloat *m = pIn->mat;
    GLfloat a0 = m[0] * m[5] - m[1] * m[4];
    GLfloat a1 = m[0] * m[6] - m[2] * m[4];
    GLfloat a2 = m[0] * m[7] - m[3] * m[4];
    GLfloat a3 = m[1] * m[6] - m[2] * m[5];
    GLfloat a4 = m[1] * m[7] - m[3] * m[5];
    GLfloat a5 = m[2] * m[7] - m[3] * m[6];
    GLfloat b0 = m[8] * m[13] - m[9] * m[12];
    GLfloat b1 = m[8] * m[14] - m[10] * m[12];
    GLfloat b2 = m[8] * m[15] - m[11] * m[12];
    GLfloat b3 = m[9] * m[14] - m[10] * m[13];
    GLfloat b4 = m[9] * m[15] - m[11] * m[13];
    GLfloat b5 = m[10] * m[15] - m[11] * m[14];
    
    // Calculate the determinant.
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
    if (fabs(det) <= MATH_TOLERANCE) {
        return NULL;
    }
    
    // Support the case where m == dst.
    RDMat4 inverse;
    inverse.mat[0]  = m[5] * b5 - m[6] * b4 + m[7] * b3;
    inverse.mat[1]  = -m[1] * b5 + m[2] * b4 - m[3] * b3;
    inverse.mat[2]  = m[13] * a5 - m[14] * a4 + m[15] * a3;
    inverse.mat[3]  = -m[9] * a5 + m[10] * a4 - m[11] * a3;
    
    inverse.mat[4]  = -m[4] * b5 + m[6] * b2 - m[7] * b1;
    inverse.mat[5]  = m[0] * b5 - m[2] * b2 + m[3] * b1;
    inverse.mat[6]  = -m[12] * a5 + m[14] * a2 - m[15] * a1;
    inverse.mat[7]  = m[8] * a5 - m[10] * a2 + m[11] * a1;
    
    inverse.mat[8]  = m[4] * b4 - m[5] * b2 + m[7] * b0;
    inverse.mat[9]  = -m[0] * b4 + m[1] * b2 - m[3] * b0;
    inverse.mat[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
    inverse.mat[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;
    
    inverse.mat[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
    inverse.mat[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
    inverse.mat[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
    inverse.mat[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;
    
    multiplyMatrix((const GLfloat *)&inverse, 1.0f / det, pOut->mat);
    
    return pOut;
}

RDVec3 *RDMat4TransformVec3(RDVec3 *outPoint, const RDMat4 *pIn, const RDVec3 *point)
{
    GLfloat w = 1.0f;
    outPoint->x = point->x * pIn->mat[0] + point->y * pIn->mat[4] + point->z * pIn->mat[8] + w * pIn->mat[12];
    outPoint->y = point->x * pIn->mat[1] + point->y * pIn->mat[5] + point->z * pIn->mat[9] + w * pIn->mat[13];
    outPoint->z = point->x * pIn->mat[2] + point->y * pIn->mat[6] + point->z * pIn->mat[10] + w * pIn->mat[14];
    return outPoint;
}

// Builds a rotation matrix that rotates around all three axes, y (yaw), x (pitch), z (roll),
// (equivalently to separate rotations, in that order), stores the result in pOut and returns the result.
RDMat4 *RDMat4RotationYXZ(RDMat4 *pOut, const GLfloat xRadians, const GLfloat yRadians, const GLfloat zRadians)
{
    /*
     |  cycz + sxsysz   czsxsy - cysz   cxsy  0 |
 M = |  cxsz            cxcz           -sx    0 |
     |  cysxsz - czsy   cyczsx + sysz   cxcy  0 |
     |  0               0               0     1 |
     
     where cA = cos(A), sA = sin(A) for A = x,y,z
     */
    GLfloat *m = pOut->mat;
    
    GLfloat cx = cosf(xRadians);
    GLfloat sx = sinf(xRadians);
    GLfloat cy = cosf(yRadians);
    GLfloat sy = sinf(yRadians);
    GLfloat cz = cosf(zRadians);
    GLfloat sz = sinf(zRadians);
    
    m[0] = (cy * cz) + (sx * sy * sz);
    m[1] = cx * sz;
    m[2] = (cy * sx * sz) - (cz * sy);
    m[3] = 0.0;
    
    m[4] = (cz * sx * sy) - (cy * sz);
    m[5] = cx * cz;
    m[6] = (cy * cz * sx) + (sy * sz);
    m[7] = 0.0;
    
    m[8] = cx * sy;
    m[9] = -sx;
    m[10] = cx * cy;
    m[11] = 0.0;
    
    m[12] = 0.0;
    m[13] = 0.0;
    m[14] = 0.0;
    m[15] = 1.0;
    
    return pOut;
}

// Builds a rotation matrix around the X-axis, stores the result in pOut and returns the result
RDMat4 *RDMat4RotationX(RDMat4 *pOut, const float radians)
{
    /*
     |  1  0       0       0 |
 M = |  0  cos(A) -sin(A)  0 |
     |  0  sin(A)  cos(A)  0 |
     |  0  0       0       1 |
     */
    GLfloat *m = pOut->mat;
    
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    
    m[4] = 0.0f;
    m[5] = cosf(radians);
    m[6] = sinf(radians);
    m[7] = 0.0f;
    
    m[8] = 0.0f;
    m[9] = -sinf(radians);
    m[10] = cosf(radians);
    m[11] = 0.0f;
    
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
    
    return pOut;
}

// Builds a rotation matrix around the Y-axis, stores the result in pOut and returns the result
RDMat4 *RDMat4RotationY(RDMat4 *pOut, const float radians)
{
    /*
     |  cos(A)  0   sin(A)  0 |
 M = |  0       1   0       0 |
     | -sin(A)  0   cos(A)  0 |
     |  0       0   0       1 |
     */
    GLfloat *m = pOut->mat;
    
    m[0] = cosf(radians);
    m[1] = 0.0f;
    m[2] = -sinf(radians);
    m[3] = 0.0f;
    
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    
    m[8] = sinf(radians);
    m[9] = 0.0f;
    m[10] = cosf(radians);
    m[11] = 0.0f;
    
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
    
    return pOut;
}

// Builds a rotation matrix around the Z-axis, stores the result in pOut and returns the result
RDMat4 *RDMat4RotationZ(RDMat4 *pOut, const float radians)
{
    /*
     |  cos(A)  -sin(A)   0   0 |
 M = |  sin(A)   cos(A)   0   0 |
     |  0        0        1   0 |
     |  0        0        0   1 |
     */
    GLfloat *m = pOut->mat;
    
    m[0] = cosf(radians);
    m[1] = sinf(radians);
    m[2] = 0.0f;
    m[3] = 0.0f;
    
    m[4] = -sinf(radians);;
    m[5] = cosf(radians);
    m[6] = 0.0f;
    m[7] = 0.0f;
    
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;
    
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
    
    return pOut;
}

// Build a rotation matrix from an axis and an angle, stores the result in pOut and returns the result.
RDMat4 *RDMat4RotationAxisAngle(RDMat4 *pOut, const RDVec3 *axis, GLfloat radians)
{
    /*
     |      									|
     | C + XX(1 - C)   -ZS + XY(1-C)  YS + ZX(1-C)   0 |
     |                                                 |
M =  | ZS + XY(1-C)    C + YY(1 - C)  -XS + YZ(1-C)  0 |
     |                                                 |
     | -YS + ZX(1-C)   XS + YZ(1-C)   C + ZZ(1 - C)  0 |
     |                                                 |
     |      0              0               0         1 |
     
     where X, Y, Z define axis of rotation and C = cos(A), S = sin(A) for A = angle of rotation
     */
    GLfloat ca = cosf(radians);
    GLfloat sa = sinf(radians);
    
    RDVec3 rax;
    RDVec3Normalize(&rax, axis);
    
    pOut->mat[0] = ca + rax.x * rax.x * (1 - ca);
    pOut->mat[1] = rax.z * sa + rax.y * rax.x * (1 - ca);
    pOut->mat[2] = -rax.y * sa + rax.z * rax.x * (1 - ca);
    pOut->mat[3] = 0.0f;
    
    pOut->mat[4] = -rax.z * sa + rax.x * rax.y * (1 - ca);
    pOut->mat[5] = ca + rax.y * rax.y * (1 - ca);
    pOut->mat[6] = rax.x * sa + rax.z * rax.y * (1 - ca);
    pOut->mat[7] = 0.0f;
    
    pOut->mat[8] = rax.y * sa + rax.x * rax.z * (1 - ca);
    pOut->mat[9] = -rax.x * sa + rax.y * rax.z * (1 - ca);
    pOut->mat[10] = ca + rax.z * rax.z * (1 - ca);
    pOut->mat[11] = 0.0f;
    
    pOut->mat[12] = 0.0f;
    pOut->mat[13] = 0.0f;
    pOut->mat[14] = 0.0f;
    pOut->mat[15] = 1.0f;
    
    return pOut;
}

// Builds a rotation matrix from a quaternion to a rotation matrix,
// stores the result in pOut and returns the result
RDMat4 *RDMat4RotationQuaternion(RDMat4 *pOut, const RDQuaternion *pQ)
{
    /*
     |       2     2									|
     | 1 - 2Y  - 2Z    2XY + 2ZW      2XZ - 2YW		 0	|
     |													|
     |                       2     2					|
 M = | 2XY - 2ZW       1 - 2X  - 2Z   2YZ + 2XW		 0	|
     |													|
     |                                      2     2		|
     | 2XZ + 2YW       2YZ - 2XW      1 - 2X  - 2Y	 0	|
     |													|
     |     0			   0			  0          1  |
     */
    GLfloat *m = pOut->mat;
    
    GLfloat twoXX = 2.0f * pQ->x * pQ->x;
    GLfloat twoXY = 2.0f * pQ->x * pQ->y;
    GLfloat twoXZ = 2.0f * pQ->x * pQ->z;
    GLfloat twoXW = 2.0f * pQ->x * pQ->w;
    
    GLfloat twoYY = 2.0f * pQ->y * pQ->y;
    GLfloat twoYZ = 2.0f * pQ->y * pQ->z;
    GLfloat twoYW = 2.0f * pQ->y * pQ->w;
    
    GLfloat twoZZ = 2.0f * pQ->z * pQ->z;
    GLfloat twoZW = 2.0f * pQ->z * pQ->w;
    
    m[0] = 1.0f - twoYY - twoZZ;
    m[1] = twoXY - twoZW;
    m[2] = twoXZ + twoYW;
    m[3] = 0.0f;
    
    m[4] = twoXY + twoZW;
    m[5] = 1.0f - twoXX - twoZZ;
    m[6] = twoYZ - twoXW;
    m[7] = 0.0f;
    
    m[8] = twoXZ - twoYW;
    m[9] = twoYZ + twoXW;
    m[10] = 1.0f - twoXX - twoYY;
    m[11] = 0.0f;
    
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
    
    return pOut;
}

// Extracts a quaternion from a rotation matrix, stores the result in quat and returns the result.
// This implementation is actually taken from the Quaternions article in Jeff LaMarche's excellent
// series on OpenGL programming for the iOS. Jeff's original source and explanation can be found here:
// http://iphonedevelopment.blogspot.com/2010/04/opengl-es-from-ground-up-9-intermission.html
// It has been adapted here for this library.
RDQuaternion *RDQuaternionRotationMatrix(RDQuaternion *quat, const RDMat4 *pIn)
{
#define QUATERNION_TRACE_ZERO_TOLERANCE 0.0001f
    GLfloat trace, s;
    const GLfloat *m = pIn->mat;
    
    trace = m[0] + m[5] + m[10];
    if (trace > 0.0f) {
        s = sqrtf(trace + 1.0f);
        quat->w = s * 0.5f;
        s = 0.5f / s;
        
        quat->x = (m[9] - m[6]) * s;
        quat->y = (m[2] - m[8]) * s;
        quat->z = (m[4] - m[1]) * s;
    } else {
        enum {A,E,I} biggest;
        if (m[0] > m[5])
            if (m[10] > m[0])
                biggest = I;
            else
                biggest = A;
            else
                if (m[10] > m[0])
                    biggest = I;
                else
                    biggest = E;
        
        switch (biggest) {
            case A:
                s = sqrtf(m[0] - (m[5] + m[10]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->x = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[9] - m[6]) * s;
                    quat->y = (m[1] + m[4]) * s;
                    quat->z = (m[2] + m[8]) * s;
                    break;
                }
                s = sqrtf(m[10] - (m[0] + m[5]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->z = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[4] - m[1]) * s;
                    quat->x = (m[8] + m[2]) * s;
                    quat->y = (m[9] + m[6]) * s;
                    break;
                }
                s = sqrtf(m[5] - (m[10] + m[0]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->y = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[2] - m[8]) * s;
                    quat->z = (m[6] + m[9]) * s;
                    quat->x = (m[4] + m[1]) * s;
                    break;
                }
                break;
                
            case E:
                s = sqrtf(m[5] - (m[10] + m[0]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->y = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[2] - m[8]) * s;
                    quat->z = (m[6] + m[9]) * s;
                    quat->x = (m[4] + m[1]) * s;
                    break;
                }
                s = sqrtf(m[10] - (m[0] + m[5]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->z = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[4] - m[1]) * s;
                    quat->x = (m[8] + m[2]) * s;
                    quat->y = (m[9] + m[6]) * s;
                    break;
                }
                s = sqrtf(m[0] - (m[5] + m[10]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->x = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[9] - m[6]) * s;
                    quat->y = (m[1] + m[4]) * s;
                    quat->z = (m[2] + m[8]) * s;
                    break;
                }
                break;
                
            case I:
                s = sqrtf(m[10] - (m[0] + m[5]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->z = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[4] - m[1]) * s;
                    quat->x = (m[8] + m[2]) * s;
                    quat->y = (m[9] + m[6]) * s;
                    break;
                }
                s = sqrtf(m[0] - (m[5] + m[10]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->x = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[9] - m[6]) * s;
                    quat->y = (m[1] + m[4]) * s;
                    quat->z = (m[2] + m[8]) * s;
                    break;
                }
                s = sqrtf(m[5] - (m[10] + m[0]) + 1.0f);
                if (s > QUATERNION_TRACE_ZERO_TOLERANCE) {
                    quat->y = s * 0.5f;
                    s = 0.5f / s;
                    quat->w = (m[2] - m[8]) * s;
                    quat->z = (m[6] + m[9]) * s;
                    quat->x = (m[4] + m[1]) * s;
                    break;
                }
                break;
                
            default:
                break;
        }
    }
    return quat;
#undef QUATERNION_TRACE_ZERO_TOLERANCE
}

// Builds a transformation matrix that translates, rotates and scales according to the specified vectors,
// stores the result in pOut and returns the result
RDMat4 *RDMat4Transformation(RDMat4 *pOut, RDVec3 *translation, RDVec3 *rotation, RDVec3 *scale)
{
    /*
     |  gxR0  gyR4  gzR8   tx |
 M = |  gxR1  gyR5  gzR9   ty |
     |  gxR2  gyR6  gzR10  tz |
     |  0     0     0      1  |
     
     where Rn is an element of the rotation matrix (R0 - R15).
     where tx = translation.x, ty = translation.y, tz = translation.z
     where gx = scale.x, gy = scale.y, gz = scale.z
     */
    
    // Start with basic rotation matrix
    RDMat4RotationYXZ(pOut, rotation->x, rotation->y, rotation->z);
    
    // Adjust for scale and translation
    GLfloat *m = pOut->mat;
    
    m[0] *= scale->x;
    m[1] *= scale->x;
    m[2] *= scale->x;
    m[3] = 0.0;
    
    m[4] *= scale->y;
    m[5] *= scale->y;
    m[6] *= scale->y;
    m[7] = 0.0;
    
    m[8] *= scale->z;
    m[9] *= scale->z;
    m[10] *= scale->z;
    m[11] = 0.0;
    
    
    m[12] = translation->x;
    m[13] = translation->y;
    m[14] = translation->z;
    m[15] = 1.0;
    
    return pOut;
}

GLfloat RDMatGet(const RDMat4 *pIn, int row, int col)
{
    return pIn->mat[row + 4 * col];
}

void RDMatSet(RDMat4 *pIn, int row, int col, GLfloat value)
{
    pIn->mat[row + 4 * col] = value;
}

void RDMatSwap(RDMat4 *pIn, int r1, int c1, int r2, int c2)
{
    float tmp = RDMatGet(pIn,r1,c1);
    RDMatSet(pIn,r1,c1,RDMatGet(pIn,r2,c2));
    RDMatSet(pIn,r2,c2, tmp);
}

// Returns an upper and a lower triangular matrix which are L and R in the Gauss algorithm
int RDGaussJordan(RDMat4 *a, RDMat4 *b)
{
#define INDEX_SIZE (4)
	int i, icol = 0, irow = 0, j, k, l, ll, n = INDEX_SIZE, m = INDEX_SIZE;
    float big, dum, pivinv;
	int indxc[INDEX_SIZE];
	int indxr[INDEX_SIZE];
	int ipiv[INDEX_SIZE];
#undef INDEX_SIZE
    
    for (j = 0; j < n; j++) {
        ipiv[j] = 0;
    }
    
    for (i = 0; i < n; i++) {
        big = 0.0f;
        for (j = 0; j < n; j++) {
            if (ipiv[j] != 1) {
                for (k = 0; k < n; k++) {
                    if (ipiv[k] == 0) {
                        if (fabsf(RDMatGet(a,j, k)) >= big) {
                            big = fabsf(RDMatGet(a,j, k));
                            irow = j;
                            icol = k;
                        }
                    }
                }
            }
        }
        ++(ipiv[icol]);
        if (irow != icol) {
            for (l = 0; l < n; l++) {
                RDMatSwap(a,irow, l, icol, l);
            }
            for (l = 0; l < m; l++) {
                RDMatSwap(b,irow, l, icol, l);
            }
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (RDMatGet(a,icol, icol) == 0.0) {
            return 0;//false
        }
        pivinv = 1.0f / RDMatGet(a,icol, icol);
        RDMatSet(a,icol, icol, 1.0f);
        for (l = 0; l < n; l++) {
            RDMatSet(a,icol, l, RDMatGet(a,icol, l) * pivinv);
        }
        for (l = 0; l < m; l++) {
            RDMatSet(b,icol, l, RDMatGet(b,icol, l) * pivinv);
        }
        
        for (ll = 0; ll < n; ll++) {
            if (ll != icol) {
                dum = RDMatGet(a,ll, icol);
                RDMatSet(a,ll, icol, 0.0f);
                for (l = 0; l < n; l++) {
                    RDMatSet(a,ll, l, RDMatGet(a,ll, l) - RDMatGet(a,icol, l) * dum);
                }
                for (l = 0; l < m; l++) {
                    RDMatSet(b,ll, l, RDMatGet(a,ll, l) - RDMatGet(b,icol, l) * dum);
                }
            }
        }
    }
    // This is the end of the main loop over columns of the reduction. It only remains to unscram-
    // ble the solution in view of the column interchanges. We do this by interchanging pairs of
    // columns in the reverse order that the permutation was built up.
    for (l = n - 1; l >= 0; l--) {
        if (indxr[l] != indxc[l]) {
            for (k = 0; k < n; k++) {
                RDMatSwap(a,k, indxr[l], k, indxc[l]);
            }
        }
    }
    return 1;//true
}
