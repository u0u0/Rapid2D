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

#include "RDSprite.h"
#include "RDProgramCache.h"
#include "RDTexture.h"
#include "RDProgram.h"
#include "RDMacros.h"
#include "RDMath.h"
#include "RDResolution.h"
#include "RDIndexBuffer.h"
#include "RDDirector.h"

RDSprite::RDSprite()
:_program(NULL)
,_spriteFrame(NULL)
,_vertexBuffer(0)
,_anchorPoint({0.5, 0.5})
,_opacity(255)
,_flipX(false)
,_flipY(false)
,_isVertexDirty(true)
{
    _program = RDProgramCache::getInstance()->getProgram("sprite");
    _indexBuffer = RDIndexBuffer::getInstance()->getVIB("sprite");
    for (int i = 0; i < sizeof(_vertex) / sizeof(RDVertex); i++) {
        // init to white color
        RDColorInit(&_color[i], 255.0, 255.0, 255.0, 255.0);
    }
    glGenBuffers(1, &_vertexBuffer);
}

RDSprite::~RDSprite()
{
    glDeleteBuffers(1, &_vertexBuffer);
}

RDSprite *RDSprite::create(RDSpriteFrame *frame)
{
    RDSprite *sprite = new RDSprite;
    if (sprite->init(frame)) {
        return sprite;
    }
    delete sprite;
    return NULL;
}

bool RDSprite::init(RDSpriteFrame *frame)
{
    if (!frame) {
        return false;
    }
    _spriteFrame = frame;
    
    // init default vertex position
    setAnchorPoint(&_anchorPoint);
    for (int i = 0; i < sizeof(_vertex) / sizeof(RDVertex); i++) {
        _vertex[i].position.z = 0;//default z value
    }
    setColor(&_color[0]);
    updateTexcoord();
    
    return true;
}

void RDSprite::updateTexcoord(void)
{
    /* _spriteFrame->frame.origin is screen coordinate
     * which Y is from top to buttom.
     * frame relative to texture coordinate
     */
    GLfloat textureW = _spriteFrame->texture->getWidth();
    GLfloat textureH = _spriteFrame->texture->getHeight();
    
    GLfloat left;
    GLfloat right;
    GLfloat top;
    GLfloat bottom;

    if (_spriteFrame->rotated) {
        left = _spriteFrame->frame.origin.x / textureW;
        right = (_spriteFrame->frame.origin.x + _spriteFrame->frame.size.height) / textureW;
        top = (textureH - _spriteFrame->frame.origin.y) / textureH;
        bottom = (textureH - _spriteFrame->frame.origin.y - _spriteFrame->frame.size.width) / textureH;
        
        if (_flipX) {
            std::swap(top, bottom);
        }
        if (_flipY) {
            std::swap(left, right);
        }
        
        // RightBottom
        _vertex[0].texcoord.s = left;
        _vertex[0].texcoord.t = bottom;
        
        // RightTop
        _vertex[1].texcoord.s = right;
        _vertex[1].texcoord.t = bottom;
        
        // LeftTop
        _vertex[2].texcoord.s = right;
        _vertex[2].texcoord.t = top;
        
        // LeftBottom
        _vertex[3].texcoord.s = left;
        _vertex[3].texcoord.t = top;
    } else {
        left = (_spriteFrame->frame.origin.x + _spriteFrame->frame.size.width) / textureW;
        right = _spriteFrame->frame.origin.x / textureW;
        top = (textureH - _spriteFrame->frame.origin.y - _spriteFrame->frame.size.height) / textureH;
        bottom = (textureH - _spriteFrame->frame.origin.y) / textureH;
        
        if (_flipX) {
            std::swap(left, right);
        }
        if (_flipY) {
            std::swap(top, bottom);
        }
        
        // RightBottom
        _vertex[0].texcoord.s = left;
        _vertex[0].texcoord.t = top;
        
        // RightTop
        _vertex[1].texcoord.s = left;
        _vertex[1].texcoord.t = bottom;
        
        // LeftTop
        _vertex[2].texcoord.s = right;
        _vertex[2].texcoord.t = bottom;
        
        // LeftBottom
        _vertex[3].texcoord.s = right;
        _vertex[3].texcoord.t = top;
    }
    
    _isVertexDirty = true;
}

void RDSprite::setColor(RDColor *color)
{
    for (int i = 0; i < sizeof(_vertex) / sizeof(RDVertex); i++) {
        // save for interface
        RDColorInit(&_color[i], color->r, color->g, color->b, color->a);
        // opacity deal
        RDColor opacityedColor;
        RDColorInit(&opacityedColor,
                    _color[i].r * _opacity / 255.0,
                    _color[i].g * _opacity / 255.0,
                    _color[i].b * _opacity / 255.0,
                    _color[i].a * _opacity / 255.0);
        // update OpenGL vertex
        RDColorToGL(&opacityedColor, &_vertex[i].color);
    }
    
    _isVertexDirty = true;
}

void RDSprite::setAnchorPoint(RDPoint *point)
{
#if defined(RD_DEBUG)
    if (point->x < 0.0 || point->x > 1.0
        || point->y < 0.0 || point->y > 1.0) {
        RDLog("[Error]: setAnchorPoint for wrong value!\n");
        return;
    }
#endif
    _anchorPoint.x = point->x;
    _anchorPoint.y = point->y;
    
    /* _spriteFrame->source.origin is screen coordinate
     * which Y is from top to buttom.
     * anchorPoint is relative to sourceSize
     */
    GLfloat leftTopx = -(_spriteFrame->sourceSize.width * _anchorPoint.x);
    GLfloat leftTopy = _spriteFrame->sourceSize.height * (1.0f - _anchorPoint.y);
    
    /* convert source vertex to frame vertex, make texturepacker trimmed happy */
    // LeftTop
    _vertex[2].position.x = leftTopx + _spriteFrame->source.origin.x;
    _vertex[2].position.y = leftTopy - _spriteFrame->source.origin.y;
    // LeftBottom
    _vertex[3].position.x = _vertex[2].position.x;
    _vertex[3].position.y = _vertex[2].position.y - _spriteFrame->source.size.height;
    // RightBottom
    _vertex[0].position.x = _vertex[3].position.x + _spriteFrame->source.size.width;
    _vertex[0].position.y = _vertex[3].position.y;
    // RightTop
    _vertex[1].position.x = _vertex[0].position.x;
    _vertex[1].position.y = _vertex[2].position.y;
    
    _isVertexDirty = true;
}

void RDSprite::setFlip(bool flipx, bool flipy)
{
    _flipX = flipx;
    _flipY = flipy;
    updateTexcoord();
}

void RDSprite::setOpacity(RD_uchar opacity)
{
    _opacity = opacity;
    
    for (int i = 0; i < sizeof(_vertex) / sizeof(RDVertex); i++) {
        // opacity deal
        RDColor opacityedColor;
        RDColorInit(&opacityedColor,
                    _color[i].r * _opacity / 255.0,
                    _color[i].g * _opacity / 255.0,
                    _color[i].b * _opacity / 255.0,
                    _color[i].a * _opacity / 255.0);
        // update OpenGL vertex
        RDColorToGL(&opacityedColor, &_vertex[i].color);
    }
    
    _isVertexDirty = true;
}

void RDSprite::setFrame(RDSpriteFrame *frame)
{
    if (!frame) {
        return;
    }
    _spriteFrame = frame;
    
    // update vertex
    setAnchorPoint(&_anchorPoint);
    updateTexcoord();
}

void RDSprite::draw(RDMat4 *modelView)
{
    RD_ASSERT(modelView, "sprite draw fail, must have a modelView");
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    if (_isVertexDirty) {
        _isVertexDirty = false;
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertex), _vertex, GL_DYNAMIC_DRAW);
    }
    
    // alpha blend setting
    glEnable(GL_BLEND);
    if (_spriteFrame->texture->hasPremultipliedAlpha()) {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glUseProgram(_program->getProgram());
    
    // use projection
    RDMat4 *projection = RDDirector::getInstance()->getProjection(RD_Projection_2D);
    glUniformMatrix4fv(_program->getProjectionUniform(), 1, GL_FALSE, projection->mat);
    // ModelView
    glUniformMatrix4fv(_program->getModelviewUniform(), 1, GL_FALSE, modelView->mat);
    
    // position
    glVertexAttribPointer(_program->getPositionSlot(), 3, GL_FLOAT, GL_FALSE,
                          sizeof(RDVertex), 0);
    // color
    glVertexAttribPointer(_program->getColorSlot(), 4, GL_FLOAT, GL_FALSE,
                          sizeof(RDVertex), (GLvoid *)offsetof(RDVertex, color));
    // texture coord
    glVertexAttribPointer(_program->getTexCoordSlot(), 2, GL_FLOAT, GL_FALSE,
                          sizeof(RDVertex), (GLvoid *)offsetof(RDVertex, texcoord));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _spriteFrame->texture->getTextureID());
    glUniform1i(_program->getTextureUniform(), 0);// texture uniform
    
    // alpha test
    glUniform1f(_program->getAlphaUniform(), 0.0);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    
    RD_CHECK_GL_ERROR();
}
