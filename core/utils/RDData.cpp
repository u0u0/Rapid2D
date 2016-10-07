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

#include <stdlib.h>

#include "RDData.h"

RDData::RDData()
:_buffer(NULL)
,_hasBOM(false)
,_size(0)
{
}

RDData::RDData(RD_uchar *buffer, size_t size)
:_hasBOM(false)
{
    _buffer = buffer;
    _size = size;
}

RDData::~RDData()
{
	if (_buffer) {
		free(_buffer);
    }
}

size_t RDData::getSize()
{
	return _hasBOM ? _size - 3 : _size;
}

RD_uchar *RDData::getBuffer()
{
	return _hasBOM ? _buffer + 3 : _buffer;
}
