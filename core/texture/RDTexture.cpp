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

#include "RDTexture.h"
#include "RDFileSystem.h"
#include "RDPortingTTF.h"
#include "RDLog.h"

RDTexture::RDTexture()
:_format(RD_TEXTURE_FORMAT_NULL)
,_data(NULL)
,_dataLen(0)
,_isPremultipliedAlpha(false)
,_height(0)
,_width(0)
,_texture(0)
{
}

RDTexture::~RDTexture()
{
    glDeleteTextures(1, &_texture);
    if (_data) {
        free(_data);
    }
}

#define RD_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
(RD_uint32)(((RD_uint32)((RD_uchar)(vr) * ((RD_uchar)(va) + 1)) >> 8) | \
((RD_uint32)((RD_uchar)(vg) * ((RD_uchar)(va) + 1) >> 8) << 8) | \
((RD_uint32)((RD_uchar)(vb) * ((RD_uchar)(va) + 1) >> 8) << 16) | \
((RD_uint32)(RD_uchar)(va) << 24))

void RDTexture::premultipliedAlpha(void)
{
    RD_ASSERT(_format == RD_TEXTURE_FORMAT_RGBA32, "premultipliedAlpha should be RGBA32!");
    
    RD_uint32 *fourBytes = (RD_uint32 *)_data;
    for (RD_uint32 i = 0; i < _width * _height; i++) {
        RD_uchar *p = _data + i * 4;
        fourBytes[i] = RD_RGB_PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
    }
    
    _isPremultipliedAlpha = true;
}
#undef RD_RGB_PREMULTIPLY_ALPHA

bool RDTexture::init(const char *fileName)
{
    RD_FileType type = RDFileSystem::getInstance()->getFileType(fileName);
    if (type <= RD_FILETYPE_TEXTURE_START || type >= RD_FILETYPE_TEXTURE_END) {
        RDLog("[Error]: RDTexture unknown texture file: %s\n", fileName);
        return false;
    }
    
    RDData *data = RDFileSystem::getInstance()->getData(RD_FILEDIR_RES, fileName);
    if (!data) {
        return false;
    }
    
    bool rtn = false;
    switch (type) {
        case RD_FILETYPE_PNG:
        {
            rtn = decodePng(data);
            break;
        }
        default:
            break;
    }
    delete data;
    
    if (!rtn) {
        return false;
    }
    
    createGLTexture();
    return true;
}

// init OpenGL texture and attach image data to it
void RDTexture::createGLTexture(void)
{
    // gen texture
    glGenTextures(1, &_texture);
    // bind texture
    glBindTexture(GL_TEXTURE_2D, _texture);
    // texture paramater
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // OpenGL ES 2.0 NPOT setting
    // If the texture size is NPOT (non power of 2),
    // then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // attach image data
    switch (_format) {
        case RD_TEXTURE_FORMAT_RGBA32:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)_width, (GLsizei)_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
            break;
        case RD_TEXTURE_FORMAT_RGBA24:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)_width, (GLsizei)_height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
            break;
        case RD_TEXTURE_FORMAT_RGBA4444:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)_width, (GLsizei)_height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, _data);
            break;
        case RD_TEXTURE_FORMAT_RGB5A1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)_width, (GLsizei)_height, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, _data);
            break;
        case RD_TEXTURE_FORMAT_RGB565:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)_width, (GLsizei)_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, _data);
            break;
        case RD_TEXTURE_FORMAT_AI88:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, (GLsizei)_width, (GLsizei)_height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, _data);
            break;
        case RD_TEXTURE_FORMAT_A8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei)_width, (GLsizei)_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, _data);
            break;
        case RD_TEXTURE_FORMAT_I8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, (GLsizei)_width, (GLsizei)_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, _data);
            break;
        default:
            RD_ASSERT(0, "Unknown texture format");
    }
    
    if (_data) {
        free(_data);
        _data = NULL;
    }
    
    RD_CHECK_GL_ERROR();
}

bool RDTexture::initWithTTF(const char *utf8_text,
                            const char *font,
                            float fontSize,
                            RDColor *fontColor,
                            int align)
{
    RD_Bitmap32 bitmap;
#if defined(RD_DEBUG)
    if (align < 0 || align > 2) {
        RDLog("Error: initWithTTF fail for wrong align");
        return false;
    }
#endif
    if (!RDPortingTTF_getBitmap32(utf8_text, font, fontSize, fontColor, align, &bitmap)) {
        return false;
    }
    
    _format = RD_TEXTURE_FORMAT_RGBA32;
    _data = bitmap.buffer;
    _width = bitmap.width;
    _height = bitmap.height;
    _isPremultipliedAlpha = bitmap.isPremultipliedAlpha;
    
    createGLTexture();
    return true;
}

RDTexture *RDTexture::create(const char *fileName)
{
    RDTexture *texture = new RDTexture;
    bool rtn = texture->init(fileName);
    if (rtn) {
        return texture;
    }
    
    RDLog("[Error]: RDTexture init failed:%d!!\n", fileName);
    delete texture;
    return NULL;
}

RDTexture *RDTexture::createWithTTF(const char *utf8_text,
                                    const char *font,
                                    float fontSize,
                                    RDColor *fontColor,
                                    int align)
{
    RDTexture *texture = new RDTexture;
    bool rtn = texture->initWithTTF(utf8_text, font, fontSize, fontColor, align);
    if (rtn) {
        return texture;
    }
    
    RDLog("[Error]: RDTexture initTTF failed!!\n");
    delete texture;
    return NULL;
}
