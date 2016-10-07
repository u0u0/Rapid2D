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

#include <android/log.h>
#include <sys/types.h>

#include "c2java.h"

static JavaVM *s_vm=NULL;
static JNIEnv *s_env = NULL;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	// cache vm, DO NOT cache env here, otherwise ndk crash.
	s_vm = vm;
	return JNI_VERSION_1_4;
}

int c2java_getStaticInfo(const char *classname,
		const char *func,
		const char *param,
		c2java_info *info)
{
	jclass cclass;
	jmethodID methodID;//If staticMethodID, do not release local ref.

	// cache env in GLThread
	if (NULL == s_env) {
		if(s_vm->GetEnv((void**)&s_env, JNI_VERSION_1_4) != JNI_OK) {
			__android_log_print(ANDROID_LOG_VERBOSE, "Rapid2D_LOG",
					"Error: c2java fail for GetEnv!");
			return -1;
		}
	}

	// java class -> c class
	cclass = s_env->FindClass(classname);
	if (cclass == NULL) {
		__android_log_print(ANDROID_LOG_INFO, "Rapid2D_LOG", "c2java FindClass error");
		return -2;
	}

	// get method
	methodID = s_env->GetStaticMethodID(cclass, func, param);
	if (methodID == NULL) {
		s_env->DeleteLocalRef(cclass);
		__android_log_print(ANDROID_LOG_INFO, "Rapid2D_LOG", "c2java GetMethodID error");
		return -3;
	}

	info->env = s_env;
	info->method = methodID;
	info->cls = cclass;
	/* info->obj = NULL; */

	return 0;
}


void c2java_release(c2java_info *info)
{
	/* if (info->obj) { */
	/* 	(info->env)->DeleteLocalRef(info->obj); */
	/* } */

	if (info->cls) {
		(info->env)->DeleteLocalRef(info->cls);
	}
}
