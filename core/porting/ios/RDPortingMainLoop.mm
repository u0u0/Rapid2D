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

#import <QuartzCore/QuartzCore.h>
#import "RDPortingMainLoop.h"

// ====== wrap Obejct-c class =====
@interface DisplayLinkWarpper : NSObject {
    CADisplayLink *displayLink;
}

@property(nonatomic, assign) RDPortingMainLoopCB callback;
@property(nonatomic, readonly) double interval;

- (void)start;
- (void)stop;
- (void)doCaller:(id)sender;
+ (DisplayLinkWarpper *)sharedInstance;

@end

@implementation DisplayLinkWarpper

/* begin of ARC singleton */
__strong static DisplayLinkWarpper *_singleton = nil;

+ (DisplayLinkWarpper *)sharedInstance
{
    static dispatch_once_t pred = 0;
    dispatch_once(&pred, ^{
        _singleton = [[super allocWithZone:NULL] init];
    });
    return _singleton;
}

+ (id)allocWithZone:(NSZone *)zone
{
    return [self sharedInstance];
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

/* end of singleton */

- (id)init
{
    if((self = [super init])) {
        displayLink = nil;
        _callback = NULL;
    }
    
    return self;
}

- (void)start
{
    if (displayLink) {
        NSLog(@"Error in RDMainLoop_start, you must stop first!");
        return;
    }
    
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(doCaller:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)stop
{
    if (displayLink) {
        [displayLink invalidate];
        displayLink = nil;
    }
}

extern "C" void RDGLView_FBO_draw(void);
- (void)doCaller:(id)sender
{
    if (self.callback) {
        self.callback();
        RDGLView_FBO_draw();
    }
}

@end

// ======= end of wapper class =======


///////////  porting inferface /////////////

void RDPortingMainLoop_start(RDPortingMainLoopCB cb)
{
    [DisplayLinkWarpper sharedInstance].callback = cb;
    [[DisplayLinkWarpper sharedInstance] start];
}

void RDPortingMainLoop_stop(void)
{
    [[DisplayLinkWarpper sharedInstance] stop];
}
