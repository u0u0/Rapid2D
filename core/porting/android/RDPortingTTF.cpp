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

#include <jni.h>
#include <string.h>

#include "RDPortingTTF.h"
#include "c2java.h"

// work for single thread
static RD_Bitmap32 *s_bmp = NULL;

bool RDPortingTTF_getBitmap32(const char *utf8_text,
	const char *fontName,
	float fontSize,
	RDColor *fontColor,// 0~255
	int align,// 0 left, 1 center, 2 right
	RD_Bitmap32 *outMap)
{
	c2java_info info = {0};
	int rtn = c2java_getStaticInfo("org/rapid2d/library/RDBitmap",
			"createTextBitmap",
			"(Ljava/lang/String;Ljava/lang/String;FIIIII)V",
			&info);
	if (rtn < 0) {
		return rtn;
	}
	
	s_bmp = outMap;

	// call function
    jstring jText = info.env->NewStringUTF(utf8_text);
    jstring jFontName = info.env->NewStringUTF(fontName);
    info.env->CallStaticVoidMethod(info.cls, info.method,
			jText,
			jFontName,
			fontSize,
			fontColor->r,
			fontColor->g,
			fontColor->b,
			fontColor->a,
			align);
	info.env->DeleteLocalRef(jText); // release memory
	info.env->DeleteLocalRef(jFontName); // release memory

	c2java_release(&info);
	return true;
}

extern "C"
{
	// call from java
	void Java_org_rapid2d_library_RDBitmap_initNativeBMP(JNIEnv *env,
			jobject thiz,
			int width,
			int height,
			jbyteArray pixels)
	{
		int lineSize = width * 4;
		int size = lineSize * height;
		s_bmp->isPremultipliedAlpha = true;
		s_bmp->width = width;
		s_bmp->height = height;
		s_bmp->buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
		/* OpenGL render from bottom -> top */
		jbyte *data = env->GetByteArrayElements(pixels, NULL);
		for (int i = 0; i < height; i++)
		{
			memcpy(s_bmp->buffer + (lineSize * i),
					data + (lineSize * (height - i - 1)),
					lineSize);
		}
		env->ReleaseByteArrayElements(pixels, data, JNI_ABORT);
	}
};
