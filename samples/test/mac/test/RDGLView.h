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

#import <Cocoa/Cocoa.h>

#define GLVIEW_WIDTH (480)
#define GLVIEW_HEIGHT (800)

@interface RDGLView : NSOpenGLView
{
    bool isTouched;
    CGFloat orginalWidth;
    CGFloat orginalHeight;
    intptr_t ids[1];
    float xs[1];
    float ys[1];
}

- (id) initWithFrame:(CGRect)frame;

@end
