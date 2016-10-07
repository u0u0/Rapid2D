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

#ifndef __RDSprite_H__
#define __RDSprite_H__

#include "RDTypes.h"
#include "RDSpriteFrame.h"

#define RD_SPRITE_VERTEX_COUNT (4)

class RDProgram;

class RDSprite
{
public:
    RDSprite();
    ~RDSprite();
    static RDSprite *create(RDSpriteFrame *frame);
    
    virtual void draw(RDMat4 *modelView);
    
    void setAnchorPoint(RDPoint *point);
    RDPoint *getAnchorPoint(void) { return &_anchorPoint; };
    
    // set all 4 vertexs color to the same value
    void setColor(RDColor *color);
    RDColor *getColor(void) { return &_color[0]; };
    
    void setFlip(bool flipx, bool flipy);
    bool getFlipX(void) { return _flipX; };
    bool getFlipY(void) { return _flipY; };
    
    void setOpacity(RD_uchar opacity);
    RD_uchar getOpacity(void) { return _opacity; };
    
    void setFrame(RDSpriteFrame *frame);
private:
    // ref only, manage by cache.
    GLuint _indexBuffer;
    // ref only, manage by cache.
    RDProgram *_program;
    // ref only, manage by lua cache.
    RDSpriteFrame *_spriteFrame;
    
    // sprite have 4 vertexs
    RDVertex _vertex[RD_SPRITE_VERTEX_COUNT];
    bool _isVertexDirty;
    // OpenGL VBO for the sprite
    GLuint _vertexBuffer;
    // AnchorPoint
    RDPoint _anchorPoint;
    // vertexColor
    RDColor _color[RD_SPRITE_VERTEX_COUNT];
    // flip
    bool _flipX;
    bool _flipY;
    RD_uchar _opacity;
    
    bool init(RDSpriteFrame *frame);
    void updateTexcoord(void);
};

#endif // __RDSprite_H__
