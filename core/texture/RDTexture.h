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

#ifndef __RDTexture_H__
#define __RDTexture_H__

#include "RDPortingGL.h"
#include "RDTypes.h"
#include "RDData.h"

typedef enum {
    RD_TEXTURE_FORMAT_NULL = -1,
    RD_TEXTURE_FORMAT_RGBA32,
    RD_TEXTURE_FORMAT_RGBA24,
    RD_TEXTURE_FORMAT_RGB565,
    RD_TEXTURE_FORMAT_A8,
    RD_TEXTURE_FORMAT_I8,
    RD_TEXTURE_FORMAT_AI88,
    RD_TEXTURE_FORMAT_RGBA4444,
    RD_TEXTURE_FORMAT_RGB5A1,
    RD_TEXTURE_FORMAT_PVRTC4,
    RD_TEXTURE_FORMAT_PVRTC2,
}RD_TextureFormat;

class RDTexture
{
public:
    RDTexture();
    ~RDTexture();
    static RDTexture *create(const char *fileName);
    static RDTexture *createWithTTF(const char *utf8_text,
                                    const char *font,
                                    float fontSize,
                                    RDColor *fontColor,
                                    int align);
    
    RD_uint32 getWidth(void) {return _width;};
    RD_uint32 getHeight(void) {return _height;};
    GLuint getTextureID(void) {return _texture;};
    bool hasPremultipliedAlpha(void) {return _isPremultipliedAlpha;};
private:
    RD_TextureFormat _format;
    RD_uint32 _width;
    RD_uint32 _height;
    RD_uchar *_data;
    size_t _dataLen;
    bool _isPremultipliedAlpha;// R.B.G has multiplied Alpha value
    GLuint _texture;
    
    bool init(const char *fileName);
    bool initWithTTF(const char *utf8_text,
                     const char *font,
                     float fontSize,
                     RDColor *fontColor,
                     int align);
    void createGLTexture(void);
    
    bool decodePng(RDData *data);
    void premultipliedAlpha(void);
};

#endif // __RDTexture_H__
