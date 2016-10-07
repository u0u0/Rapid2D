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
#import <Cocoa/Cocoa.h>
#import <math.h>
#include "RDPortingTTF.h"

bool RDPortingTTF_getBitmap32(const char *utf8_text,
                              const char *fontName,
                              float fontSize,
                              RDColor *fontColor,
                              int align,
                              RD_Bitmap32 *outMap)
{
    NSString *string = [NSString stringWithUTF8String:utf8_text];
    // fix with fontName
    NSFont *font = [[NSFontManager sharedFontManager]
                    fontWithFamily:[NSString stringWithUTF8String:fontName]
                    traits:NSUnboldFontMask | NSUnitalicFontMask
                    weight:0
                    size:fontSize];
    if (font == nil) {
        NSLog(@"Error: RDPortingTTF_getBitmap32 fail to load font:%s", fontName);
        return false;
    }
    
    NSColor *foregroundColor = [NSColor colorWithDeviceRed:fontColor->r / 255.0f
                                                     green:fontColor->g / 255.0f
                                                      blue:fontColor->b / 255.0f
                                                     alpha:fontColor->a / 255.0f];
    
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
    [paragraphStyle setLineBreakMode:NSLineBreakByCharWrapping];
    switch (align) {
        case 0://left
            [paragraphStyle setAlignment:NSLeftTextAlignment];
            break;
        case 1://center
            [paragraphStyle setAlignment:NSCenterTextAlignment];
            break;
        case 2://right
            [paragraphStyle setAlignment:NSRightTextAlignment];
            break;
        default:
            NSLog(@"Error: RDPortingTTF_getBitmap32 fail for wrong aling:%d", align);
            return false;
    }
    
    // attribute
    NSDictionary *tokenAttributesDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                         foregroundColor,NSForegroundColorAttributeName,
                                         font, NSFontAttributeName,
                                         paragraphStyle, NSParagraphStyleAttributeName, nil];
    
    NSAttributedString *stringWithAttributes =[[NSAttributedString alloc] initWithString:string
                                                                              attributes:tokenAttributesDict];
				
    NSSize realDimensions = [stringWithAttributes size];
    if (realDimensions.width <= 0 || realDimensions.height <= 0) {
        NSLog(@"Error: RDPortingTTF_getBitmap32 fail for get string SIZE");
        return false;
    }
    
    // fix for image width height to int
    realDimensions.width = ceilf(realDimensions.width);
    realDimensions.height = ceilf(realDimensions.height);
    
    /* ===== everything ready, start draw on image ===== */
    NSImage *image = [[NSImage alloc] initWithSize:realDimensions];
    [image lockFocus];// set image to draw on
    // mac retina display fix
    [[NSAffineTransform transform] set];
    // reverse image on height
    CGContextTranslateCTM([[NSGraphicsContext currentContext] CGContext], 0, realDimensions.height);
    CGContextScaleCTM ([[NSGraphicsContext currentContext] CGContext], 1.0, -1.0);
    // draw string
    NSRect textRect = NSMakeRect(0, 0, realDimensions.width, realDimensions.height);
    [stringWithAttributes drawInRect:textRect];
    // get image date
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithFocusedViewRect:textRect];
    [image unlockFocus];
    /* ===== draw end ===== */
    
    // get image data
    unsigned char *data = (unsigned char*) [bitmap bitmapData];
    // output setting
    NSUInteger bitmapSize = realDimensions.width * realDimensions.height * 4;
    outMap->buffer = (RD_uchar *)malloc(bitmapSize);
    memcpy(outMap->buffer, data, bitmapSize);
    outMap->width = realDimensions.width;
    outMap->height = realDimensions.height;
    outMap->isPremultipliedAlpha = true;
    
    return true;
}
