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

#include <string.h>
#include <jni.h>
// for native asset manager
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "../../../common/cpp/startup.h"

jstring
Java_org_rapid2d_library_Rapid2DActivity_stringFromJNI(JNIEnv* env,
		jobject thiz)
{
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__x86_64__)
   #define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
   #define ABI "mips64"
#elif defined(__mips__)
   #define ABI "mips"
#elif defined(__aarch64__)
   #define ABI "arm64-v8a"
#else
   #define ABI "unknown"
#endif

    return (*env)->NewStringUTF(env, "Rapid2D Compiled with ABI:" ABI ".");
}

AAssetManager *g_AssetMgr = NULL;
// save assert manager
void Java_org_rapid2d_library_Rapid2DActivity_setAssetManager(JNIEnv* env,
		jobject thiz,
		jobject assetManager)
{
	g_AssetMgr = AAssetManager_fromJava(env, assetManager);
}

const char *g_androidFilesDir = NULL;
// save writable path
void Java_org_rapid2d_library_Rapid2DActivity_setFilesDir(JNIEnv* env,
		jobject thiz,
		jstring dir)
{
	// convert to utf8 string
	const char *temp = (*env)->GetStringUTFChars(env, dir, NULL);
	// save to gloable
	g_androidFilesDir = strdup(temp);
	// release java reference count
	(*env)->ReleaseStringUTFChars(env, dir, temp);
}

// start engine
void
Java_org_rapid2d_library_Rapid2DActivity_init(JNIEnv* env,
		jobject thiz, 
		jint width,
		jint height)
{
	Rapid2D_startup(width, height);
}

// mainLoop call from java, then sink into engine loop
typedef void(*RDPortingMainLoopCB)(void);
void *g_mainFrameCB = NULL;
void Java_org_rapid2d_library_Rapid2DActivity_mainLoop(JNIEnv* env, jobject thiz)
{
	RDPortingMainLoopCB cb = (RDPortingMainLoopCB)g_mainFrameCB;
	if (cb) {
		cb();
	}
}

// java touch send to engine
void Java_org_rapid2d_library_Rapid2DActivity_sendTouchs(JNIEnv* env,
		jobject thiz,
		jstring jtype,
		int count,
		jintArray ja_ids,
		jfloatArray ja_xs,
		jfloatArray ja_ys)
{
	jint *ids = (*env)->GetIntArrayElements(env, ja_ids, JNI_FALSE);
	jfloat *xs = (*env)->GetFloatArrayElements(env, ja_xs, JNI_FALSE);
	jfloat *ys = (*env)->GetFloatArrayElements(env, ja_ys, JNI_FALSE);
	char *type = (char *)(*env)->GetStringUTFChars(env, jtype, NULL);

	Rapid2D_touchEvent(type, count, ids, xs, ys);

	(*env)->ReleaseIntArrayElements(env, ja_ids, ids, 0);
	(*env)->ReleaseFloatArrayElements(env, ja_xs, xs, 0);
	(*env)->ReleaseFloatArrayElements(env, ja_ys, ys, 0);
	(*env)->ReleaseStringUTFChars(env, jtype, type);
}

// java key event send to engine
void Java_org_rapid2d_library_Rapid2DActivity_sendKeyEvent(JNIEnv* env,
		jobject thiz,
		jstring jtype,
		int code)
{
	char *type = (char *)(*env)->GetStringUTFChars(env, jtype, NULL);

	Rapid2D_keyBoardEvent(type, code);

	(*env)->ReleaseStringUTFChars(env, jtype, type);
}
