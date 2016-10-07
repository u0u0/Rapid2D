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

#ifndef __RDResolution_H__
#define __RDResolution_H__

#include "RDTypes.h"

typedef enum {
    RD_Resolution_FixedHeight,
    RD_Resolution_FixedWidth,
    RD_Resolution_UnKnown,
}RD_ResolutionMode;

class RDResolution
{
public:
    RDResolution();
    ~RDResolution();

    static RDResolution *getInstance(void);
    
    /*
     * Get the frame size of EGL view.
     * In general, it returns the screen size since the EGL view is a fullscreen view.
     */
    const RDSize &getFrameSize(void);
    
    void setFrameSize(float width, float height);

    void setResolutionModeSize(RD_ResolutionMode mode, float width, float height);

    /* Get design resolution size.
     *  Default resolution size is the same as 'getFrameSize'.
     */
    const RDSize &getResolutionSize(void);

    float getScale() {return _scale;};
private:
    static RDResolution *_instance;
    
    // in full screen, _frameSize ==  screenSize
    RDSize _frameSize;
    // resolution size, it is the size appropriate for the app resources.
    RDSize _resolutionSize;
    RD_ResolutionMode _mode;
    float _scale;
};

#endif /* __RDResolution_H__ */
