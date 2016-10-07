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

#ifndef __C2JAVA_H__
#define __C2JAVA_H__

#include <jni.h>

typedef struct {
	JNIEnv *env;
	jclass cls;
	//jobject obj;// for non static method
	jmethodID method;
} c2java_info;

#ifdef __cplusplus
extern "C" {
#endif

int c2java_getStaticInfo(const char *classname,
		const char *func,
		const char *param,
		c2java_info *info);

void c2java_release(c2java_info *info);

#ifdef __cplusplus
}
#endif

#endif // end of __C2JAVA_H__
