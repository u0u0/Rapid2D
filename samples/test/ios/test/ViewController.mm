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

#import "ViewController.h"
#import "RDGLView.h"
#import "startup.h"

@interface ViewController () {
    CGFloat realWidth;
    CGFloat realHeight;
}

@end

@implementation ViewController

- (void)loadView
{
    /* if locked' Orientations, here is same bug for view to get frame.size,
     * so the safe way is get real size by myself.
     */
    CGFloat small = MIN([UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height);
    CGFloat big = MAX([UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height);
    if ([self supportedInterfaceOrientations] == UIInterfaceOrientationMaskPortrait) {
        realWidth = small;
        realHeight = big;
    } else {
        realWidth = big;
        realHeight = small;
    }
    
    self.view = [[RDGLView alloc] initWithFrame:CGRectMake(0, 0, realWidth, realHeight)];
}

- (void)viewDidLoad {
    [super viewDidLoad];

    Rapid2D_startup(realWidth * self.view.contentScaleFactor, realHeight * self.view.contentScaleFactor);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

// orientations support
- (NSUInteger) supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskPortrait;
}

- (BOOL) shouldAutorotate {
    return YES;
}
// end of orientations support

@end
