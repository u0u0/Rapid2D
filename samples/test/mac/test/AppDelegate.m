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

#import "AppDelegate.h"
#import "RDGLView.h"
#import "startup.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSRect screenRect = [[NSScreen mainScreen] frame];
    // set window size and position
    [_window setContentSize:CGSizeMake(GLVIEW_WIDTH, GLVIEW_HEIGHT)];
    [_window setFrameTopLeftPoint:CGPointMake((screenRect.size.width - GLVIEW_WIDTH) / 2,
                                              (screenRect.size.height - GLVIEW_HEIGHT) / 2 + GLVIEW_HEIGHT)];
    // fixed ratio to resize the window
    _window.aspectRatio = CGSizeMake(GLVIEW_WIDTH, GLVIEW_HEIGHT);
    // disable fullscreen and keep resize able
    [[_window standardWindowButton:NSWindowZoomButton] setHidden:YES];
    [_window becomeFirstResponder];
    // init OpenGL View
    RDGLView *glView = [[RDGLView alloc] initWithFrame:CGRectMake(0, 0, GLVIEW_WIDTH, GLVIEW_HEIGHT)];
    [_window setContentView:glView];
    // set title for display
    [_window setTitle:@"test"];
    // Close button callback set
    NSButton *closeButton = [_window standardWindowButton:NSWindowCloseButton];
    [closeButton setTarget:self];
    [closeButton setAction:@selector(closeApplication)];
    
    // start engine
    Rapid2D_startup(GLVIEW_WIDTH, GLVIEW_HEIGHT);
}

- (void) closeApplication {
    // trigger applicationWillTerminate
    [[NSApplication sharedApplication] terminate:nil];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    Rapid2D_stop();
}

@end
