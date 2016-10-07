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

#import <OpenGL/gl.h>
#import "RDGLView.h"
#include "Rapid2D.h"

static NSOpenGLContext *_context = nil;

@implementation RDGLView

- (id) initWithFrame:(CGRect)frame
{
    NSOpenGLPixelFormatAttribute attribs[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        0
    };
    
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    if (!pixelFormat) {
        NSLog(@"No OpenGL pixel format");
        return nil;
    }
    
    orginalWidth = frame.size.width;
    orginalHeight = frame.size.height;
    // init OpenGL for view
    if((self = [super initWithFrame:frame pixelFormat:pixelFormat])) {
        _context = [self openGLContext];
    }
    
    // shader control PointSize, ONLY NEED BY OpenGL on desktop
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    return self;
}

-(void)computerTouchPos:(NSEvent *)theEvent
{
    ids[0] = 1;
    // after windos resize, must change pos to new size.
    xs[0] = [theEvent locationInWindow].x * orginalWidth / [self bounds].size.width;
    ys[0] = [theEvent locationInWindow].y * orginalHeight / [self bounds].size.height;
}

-(void)mouseDown:(NSEvent *)theEvent
{
    [self computerTouchPos:theEvent];
    Rapid2D_touchEvent("began", 1, ids, xs, ys);
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    [self computerTouchPos:theEvent];
    Rapid2D_touchEvent("moved", 1, ids, xs, ys);
}

-(void)mouseUp:(NSEvent *)theEvent
{
    [self computerTouchPos:theEvent];
    Rapid2D_touchEvent("ended", 1, ids, xs, ys);
}

- (void)keyDown:(NSEvent *)theEvent
{
    Rapid2D_keyBoardEvent("down", [self macKeyCode2GLFW:[theEvent keyCode]]);
}

- (void)keyUp:(NSEvent *)theEvent
{
    Rapid2D_keyBoardEvent("up", [self macKeyCode2GLFW:[theEvent keyCode]]);
}

- (void)reshape
{
    // reset viewport
    Rapid2D_setFrameSize([self bounds].size.width, [self bounds].size.height);
}

// make view get keyboard event
- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (int)macKeyCode2GLFW:(unsigned short)code
{
    switch (code) {
        case 0: return 65;//GLFW_KEY_A
        case 1: return 83;//GLFW_KEY_S
        case 2: return 68;//GLFW_KEY_D
        case 3: return 70;//GLFW_KEY_F
        case 4: return 72;//GLFW_KEY_H
        case 5: return 71;//GLFW_KEY_G
        case 6: return 90;//GLFW_KEY_Z
        case 7: return 88;//GLFW_KEY_X
        case 8: return 67;//GLFW_KEY_C
        case 9: return 86;//GLFW_KEY_V
        case 11: return 66;//GLFW_KEY_B
        case 12: return 81;//GLFW_KEY_Q
        case 13: return 87;//GLFW_KEY_W
        case 14: return 69;//GLFW_KEY_E
        case 15: return 82;//GLFW_KEY_R
        case 16: return 89;//GLFW_KEY_Y
        case 17: return 84;//GLFW_KEY_T
        case 18: return 49;//GLFW_KEY_1
        case 19: return 50;//GLFW_KEY_2
        case 20: return 51;//GLFW_KEY_3
        case 21: return 52;//GLFW_KEY_4
        case 22: return 54;//GLFW_KEY_6
        case 23: return 53;//GLFW_KEY_5
        case 24: return 61;//GLFW_KEY_EQUAL
        case 25: return 57;//GLFW_KEY_9
        case 26: return 55;//GLFW_KEY_7
        case 27: return 45;//GLFW_KEY_MINUS
        case 28: return 56;//GLFW_KEY_8
        case 29: return 48;//GLFW_KEY_0
        case 30: return 93;//GLFW_KEY_RIGHT_BRACKET
        case 31: return 79;//GLFW_KEY_O
        case 32: return 85;//GLFW_KEY_U
        case 33: return 91;//GLFW_KEY_LEFT_BRACKET
        case 34: return 73;//GLFW_KEY_I
        case 35: return 80;//GLFW_KEY_P
        case 36: return 257;//GLFW_KEY_ENTER
        case 37: return 76;//GLFW_KEY_L
        case 38: return 74;//GLFW_KEY_J
        case 39: return 39;//GLFW_KEY_APOSTROPHE
        case 40: return 75;//GLFW_KEY_K
        case 41: return 59;//GLFW_KEY_SEMICOLON
        case 42: return 92;//GLFW_KEY_BACKSLASH
        case 43: return 44;//GLFW_KEY_COMMA
        case 44: return 47;//GLFW_KEY_SLASH
        case 45: return 78;//GLFW_KEY_N
        case 46: return 77;//GLFW_KEY_M
        case 47: return 46;//GLFW_KEY_PERIOD
        case 48: return 258;//GLFW_KEY_TAB
        case 49: return 32;//GLFW_KEY_SPACE
        case 50: return 96;//GLFW_KEY_GRAVE_ACCENT
        case 51: return 259;//GLFW_KEY_BACKSPACE
        case 53: return 256;//GLFW_KEY_ESCAPE
        case 65: return 330;//GLFW_KEY_KP_DECIMAL
        case 67: return 332;//GLFW_KEY_KP_MULTIPLY
        case 69: return 334;//GLFW_KEY_KP_ADD
        case 71: return 282;//GLFW_KEY_NUM_LOCK
        case 75: return 331;//GLFW_KEY_KP_DIVIDE
        case 76: return 335;//GLFW_KEY_KP_ENTER
        case 78: return 333;//GLFW_KEY_KP_SUBTRACT
        case 82: return 320;//GLFW_KEY_KP_0
        case 83: return 321;//GLFW_KEY_KP_1
        case 84: return 322;//GLFW_KEY_KP_2
        case 85: return 323;//GLFW_KEY_KP_3
        case 86: return 324;//GLFW_KEY_KP_4
        case 87: return 325;//GLFW_KEY_KP_5
        case 88: return 326;//GLFW_KEY_KP_6
        case 89: return 327;//GLFW_KEY_KP_7
        case 91: return 328;//GLFW_KEY_KP_8
        case 92: return 329;//GLFW_KEY_KP_9
        case 96: return 294;//GLFW_KEY_F5
        case 97: return 295;//GLFW_KEY_F6
        case 98: return 296;//GLFW_KEY_F7
        case 99: return 292;//GLFW_KEY_F3
        case 100: return 297;//GLFW_KEY_F8
        case 101: return 298;//GLFW_KEY_F9
        case 105: return 283;//GLFW_KEY_PRINT_SCREEN
        case 109: return 299;//GLFW_KEY_F10
        case 114: return 260;//GLFW_KEY_INSERT
        case 115: return 268;//GLFW_KEY_HOME
        case 116: return 266;//GLFW_KEY_PAGE_UP
        case 117: return 261;//GLFW_KEY_DELETE
        case 118: return 293;//GLFW_KEY_F4
        case 119: return 269;//GLFW_KEY_END
        case 120: return 291;//GLFW_KEY_F2
        case 121: return 267;//GLFW_KEY_PAGE_DOWN
        case 122: return 290;//GLFW_KEY_F1
        case 123: return 263;//GLFW_KEY_LEFT
        case 124: return 262;//GLFW_KEY_RIGHT
        case 125: return 264;//GLFW_KEY_DOWN
        case 126: return 265;//GLFW_KEY_UP
        default: return -1;//unknown
    }
}

@end

// draw on screen
void RDGLView_FBO_draw(void)
{
    [_context flushBuffer];
}
