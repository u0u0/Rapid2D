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

#ifndef __RDPortingGL_h__
#define __RDPortingGL_h__

/****** For iOS ********/
#ifdef RD_PLATFORM_IOS
#define	glClearDepth				glClearDepthf
#define glDeleteVertexArrays		glDeleteVertexArraysOES
#define glGenVertexArrays			glGenVertexArraysOES
#define glBindVertexArray			glBindVertexArrayOES
#define glMapBuffer					glMapBufferOES
#define glUnmapBuffer				glUnmapBufferOES

#define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES
#define GL_WRITE_ONLY				GL_WRITE_ONLY_OES

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif // end of RD_PLATFORM_IOS

/****** For Mac ********/
#ifdef RD_PLATFORM_MAC
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

#define glDeleteVertexArrays            glDeleteVertexArraysAPPLE
#define glGenVertexArrays               glGenVertexArraysAPPLE
#define glBindVertexArray               glBindVertexArrayAPPLE
#endif // end of RD_PLATFORM_MAC

/****** For Win32 ********/
#ifdef RD_PLATFORM_WIN32
#include <GL/glew.h>
#endif // end of RD_PLATFORM_WIN32

/****** For Android ********/
#ifdef RD_PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif // end of RD_PLATFORM_ANDROID


#endif /* __RDPortingGL_h__ */
