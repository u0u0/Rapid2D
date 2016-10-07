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

#include <math.h>

#include "RDResolution.h"
#include "RDMacros.h"

RDResolution *RDResolution::_instance = nullptr;

RDResolution::RDResolution()
:_scale(1)
{
}

RDResolution::~RDResolution()
{
}

RDResolution *RDResolution::getInstance(void)
{
    if (!_instance) {
        _instance = new RDResolution();
    }
    return _instance;
}

void RDResolution::setResolutionModeSize(RD_ResolutionMode mode, float width, float height)
{
    if (width == 0.0f || height == 0.0f) {
        return;
    }

    _resolutionSize.width = width;
    _resolutionSize.height = height;
    
    float scaleX = (float)_frameSize.width / _resolutionSize.width;
    float scaleY = (float)_frameSize.height / _resolutionSize.height;

    if ( mode == RD_Resolution_FixedHeight) {
        _scale = scaleY;
    	_resolutionSize.width = ceilf(_frameSize.width / _scale);
    }

    if ( mode == RD_Resolution_FixedWidth) {
    	_scale = scaleX;
    	_resolutionSize.height = ceilf(_frameSize.height / _scale);
    }
    
    _mode = mode;
}

const RDSize &RDResolution::getResolutionSize(void)
{
    return _resolutionSize;
}

const RDSize &RDResolution::getFrameSize(void)
{
    return _frameSize;
}

void RDResolution::setFrameSize(float width, float height)
{
    glViewport(0, 0, width, height);
    _frameSize.width = width;
    _frameSize.height = height;
}
