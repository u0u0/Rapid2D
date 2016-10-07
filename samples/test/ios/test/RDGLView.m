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

#import "RDGLView.h"
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "Rapid2D.h"

//http://www.raywenderlich.com/3664/opengl-tutorial-for-ios-opengl-es-2-0

static EAGLContext *_context = NULL;

@implementation RDGLView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (id) initWithFrame:(CGRect)frame
{
    if((self = [super initWithFrame:frame])) {
        self.layer.opaque = YES;
        // to get real screen size
        self.contentScaleFactor = [[UIScreen mainScreen] scale];
        
        // init OpenGL
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!_context) {
            NSLog(@"Failed to create ES context");
        }
        [EAGLContext setCurrentContext:_context];
        
        // init FBO, order is important: depth -> color -> frame
        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,
                              GL_DEPTH_COMPONENT16,// review this param
                              self.frame.size.width * self.contentScaleFactor,
                              self.frame.size.height * self.contentScaleFactor);
        
        GLuint colorBuffer;
        glGenRenderbuffers(1, &colorBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
        // new for iOS EAGLContext
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        
        GLuint framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, colorBuffer);
        // if not binding depthBuffer, GL_DEPTH_TEST will not work
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, depthBuffer);
        
        self.multipleTouchEnabled = YES;
    }
    
    return self;
}

- (int)fillTouchInfo:(NSSet *)touches {
    int index = 0;
    for (UITouch *touch in [touches allObjects]) {
        CGPoint point = [touch locationInView:self];
        ids[index] = (intptr_t)touch;
        // view space convert to OpenGL space
        xs[index] = point.x * self.contentScaleFactor;
        ys[index] = (self.frame.size.height - point.y) * self.contentScaleFactor;
        index++;
        
        if (index == MAX_TOUCH_COUNT) {
            break;
        }
    }
    return index;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    int count = [self fillTouchInfo:touches];
    Rapid2D_touchEvent("began", count, ids, xs, ys);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    int count = [self fillTouchInfo:touches];
    Rapid2D_touchEvent("moved", count, ids, xs, ys);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    int count = [self fillTouchInfo:touches];
    Rapid2D_touchEvent("ended", count, ids, xs, ys);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    int count = [self fillTouchInfo:touches];
    Rapid2D_touchEvent("ended", count, ids, xs, ys);
}

@end

void RDGLView_FBO_draw(void)
{
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}
