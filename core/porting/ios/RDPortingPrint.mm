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

#include <memory.h>
#include "RDPortingPrint.h"

#define RD_MAX_LOG_LENGTH (1024)

void RDPortingPrint_log(const char *format, va_list args)
{
    char buf[RD_MAX_LOG_LENGTH];
    memset(buf, 0, sizeof(char) * RD_MAX_LOG_LENGTH);
    vsnprintf(buf, RD_MAX_LOG_LENGTH - 3, format, args);
    
    fprintf(stdout, "%s", buf);
    fflush(stdout);
}
