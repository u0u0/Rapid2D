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

#include <string.h>
#include <stdlib.h>

#include "RDTexture.h"

#include "png.h"

typedef struct {
    png_bytep data;
    size_t size;
    size_t offset;
}PngImageSource;

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    PngImageSource *isource = (PngImageSource*)png_get_io_ptr(png_ptr);
    
    if((isource->offset + length) <= isource->size)
    {
        memcpy(data, isource->data+isource->offset, length);
        isource->offset += length;
    } else {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

bool RDTexture::decodePng(RDData *data)
{
    png_structp png_ptr = 0;
    png_infop info_ptr = 0;

#define PNG_SIG_SIZE 8
    if (data->getSize() <= PNG_SIG_SIZE) {
        return false;
    }

    // check the data is png or not
    if (png_sig_cmp(data->getBuffer(), 0, PNG_SIG_SIZE)) {
        return false;
    }
#undef PNG_SIG_SIZE
    
    // init png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr) {
        return false;
    }

    // init png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, 0, 0);
        return false;
    }
    
    // set the read call back function
    PngImageSource imageSource;
    imageSource.data = data->getBuffer();
    imageSource.size = data->getSize();
    imageSource.offset = 0;
    png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

    // read png file info, and fix for OpenGL rendering
    png_read_info(png_ptr, info_ptr);
    _width = png_get_image_width(png_ptr, info_ptr);
    _height = png_get_image_height(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);
    
    // force palette images to be expanded to 24-bit RGB
    //it may include alpha channel
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    
    // low-bit-depth grayscale images are to be expanded to 8 bits
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        bit_depth = 8;
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    
    // expand any tRNS chunk data into a full alpha channel
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }

    // reduce images with 16-bit samples to 8 bits
    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }

    // Expanded earlier for grayscale, now take care of palette and rgb
    if (bit_depth < 8) {
        png_set_packing(png_ptr);
    }

    // update info
    png_read_update_info(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:
            _format = RD_TEXTURE_FORMAT_I8;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            _format = RD_TEXTURE_FORMAT_AI88;
            break;
        case PNG_COLOR_TYPE_RGB:
            _format = RD_TEXTURE_FORMAT_RGBA24;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            _format = RD_TEXTURE_FORMAT_RGBA32;
            break;
        default:
            break;
    }
    
    // read png data
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * _height);
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return false;
    }
    
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    _dataLen = rowbytes * _height;
    _data = (RD_uchar *)malloc(_dataLen);
    if (!_data) {
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return false;
    }

    for (RD_uint32 i = 0; i < _height; ++i) {
        /* OpenGL render from bottom -> top
         * but Png store data from top -> bottom
         * So fix the row_pointers by reverse order
         * (_height - 1 - i) is better than (i)
         */
        row_pointers[i] = _data + (_height - 1 - i) * rowbytes;
    }
    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, nullptr);
        
    // premultiplied alpha for RGBA8888
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        premultipliedAlpha();
    } else {
        _isPremultipliedAlpha = false;
    }

    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    return true;
}
/****  end decode png ****/
