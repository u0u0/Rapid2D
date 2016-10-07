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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "RDPortingTTF.h"

bool RDPortingTTF_getBitmap32(const char *utf8_text,
                              const char *fontName,
                              float fontSize,
                              RDColor *fontColor,// 0~255
                              int align,// 0 left, 1 center, 2 right
                              RD_Bitmap32 *outMap)
{
    NSString *string = [NSString stringWithUTF8String:utf8_text];
    UIFont *font = [UIFont fontWithName:[NSString stringWithUTF8String:fontName] size:fontSize];
    if (!font) {
        NSLog(@"Error: RDPortingTTF_getBitmap32 fail to laod font:%s", fontName);
        return false;
    }
    
    // draw attri
    UIColor *foregroundColor = [UIColor colorWithRed:fontColor->r / 255.0f
                                               green:fontColor->g / 255.0f
                                                blue:fontColor->b / 255.0f
                                               alpha:fontColor->a / 255.0f];
    
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
    [paragraphStyle setLineBreakMode:NSLineBreakByCharWrapping];
    [paragraphStyle setLineSpacing:2.0];// make text rect same with mac & win32
    switch (align) {
        case 0://left
            [paragraphStyle setAlignment:NSTextAlignmentLeft];
            break;
        case 1://center
            [paragraphStyle setAlignment:NSTextAlignmentCenter];
            break;
        case 2://right
            [paragraphStyle setAlignment:NSTextAlignmentRight];
            break;
        default:
            NSLog(@"Error: RDPortingTTF_getBitmap32 fail for wrong aling:%d", align);
            return false;
    }
    
    // attribute dict
    NSDictionary *tokenAttributesDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                         foregroundColor,NSForegroundColorAttributeName,
                                         font, NSFontAttributeName,
                                         paragraphStyle, NSParagraphStyleAttributeName, nil];
    
    // get text size
    CGSize textureSize =[string boundingRectWithSize:CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX)
                                             options:NSStringDrawingUsesLineFragmentOrigin
                                          attributes:tokenAttributesDict
                                             context:nil].size;
    
    // fix for image width height to int
    textureSize.width = ceilf(textureSize.width);
    textureSize.height = ceilf(textureSize.height);
    
    // alloc image buffer
    size_t bufffSize = textureSize.width * textureSize.height * 4;
    unsigned char *buffer = (unsigned char *)malloc(bufffSize);
    memset(buffer, 0, bufffSize);
    
    // create context
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(buffer,
                                                 textureSize.width,
                                                 textureSize.height,
                                                 8,
                                                 (int)(textureSize.width) * 4,
                                                 colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    
    if (!context) {
        free(buffer);
        NSLog(@"Error: RDPortingTTF_getBitmap32 fail for create context!");
        return false;
    }
    
    // store the current context
    UIGraphicsPushContext(context);
    [string drawInRect:CGRectMake(0, 0, textureSize.width, textureSize.height)
              withAttributes:tokenAttributesDict];
    // pop the context
    UIGraphicsPopContext();
    // release the context
    CGContextRelease(context);

    // output setting
    outMap->buffer = buffer;
    outMap->width = textureSize.width;
    outMap->height = textureSize.height;
    outMap->isPremultipliedAlpha = true;
    
    return true;
}
