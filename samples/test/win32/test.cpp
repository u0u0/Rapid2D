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

#include "stdafx.h"

// OpenGL Extension, make windows support 2.0
#include <GL/glew.h>
// OpenGL Windows Manager
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

#include <windows.h>
#include "startup.h"
#include "Rapid2D.h"

// lib for init OpenGL
#pragma comment(lib,"glew/win32/glew32.lib")
#pragma comment(lib,"glfw/win32/glfw3dll.lib")
#pragma comment(lib,"opengl32.lib")
// third library
#pragma comment(lib,"png/win32/libpng.lib")
#pragma comment(lib,"z/win32/zlib.lib")
// for audio
#pragma comment(lib,"OpenAL/win32/common.lib")
#pragma comment(lib,"OpenAL/win32/OpenAL32.lib")

// expectant window size, but real size may different
#define WIN_WIDTH (480.0)
#define WIN_HEIGHT (800.0)

// for porting layer
typedef void(*RDPortingMainLoopCB)(void);
static RDPortingMainLoopCB s_frameCB = NULL;
extern void main_setFrameCallBack(void *cb)
{
	s_frameCB = (RDPortingMainLoopCB)cb;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	printf("resize windows: %d * %d\n", width, height);
	glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		Rapid2D_keyBoardEvent("down", key);
		break;
	case GLFW_RELEASE:
		Rapid2D_keyBoardEvent("up", key);
		break;
	default:
		break;
	}
}

static int s_realWidth = WIN_WIDTH;
static int s_realHeight = WIN_HEIGHT;
static double s_mouseX = 0;
static double s_mouseY = 0;
static bool s_isMousePressed = false;
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	intptr_t ids[] = { 1 };
	float xs[] = { s_mouseX };
	float ys[] = { s_mouseY };

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch (action)
		{
		case GLFW_PRESS:
			Rapid2D_touchEvent("began", 1, ids, xs, ys);
			s_isMousePressed = true;
			break;
		case GLFW_RELEASE:
			Rapid2D_touchEvent("ended", 1, ids, xs, ys);
			s_isMousePressed = false;
			break;
		default:
			break;
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	s_mouseX = x;
	s_mouseY = s_realHeight - y;// view -> OpenGL
	if (s_isMousePressed) {
		intptr_t ids[] = { 1 };
		float xs[] = { s_mouseX };
		float ys[] = { s_mouseY };
		Rapid2D_touchEvent("moved", 1, ids, xs, ys);
	}
}

int main(void)
{
	GLFWwindow* glwindow;
	GLenum err;
	DWORD style;
	HWND win;

	/****************** Init glfw *********************/
	if (!glfwInit()) {
		fprintf(stderr, "GLFW Init Error\n");
		return -1;
	}

	int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
	RECT rt;// rt.bottom task bar height
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rt, 0);
	float scale = min((rt.right - rt.left) / WIN_WIDTH, (rt.bottom - rt.top - titleBarHeight - 14) / WIN_HEIGHT);
	s_realWidth = scale * WIN_WIDTH;
	s_realHeight = scale * WIN_HEIGHT;

	/* Create a windowed mode window and its OpenGL context */
	glwindow = glfwCreateWindow(s_realWidth, s_realHeight, "Hello World", NULL, NULL);
	if (!glwindow)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(glwindow);
	// set resize callback
	glfwSetFramebufferSizeCallback(glwindow, framebuffer_size_callback);
	// Fixed size
	win = glfwGetWin32Window(glwindow);
	style = GetWindowLong(win, GWL_STYLE);
	style = style & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX);
	SetWindowLong(win, GWL_STYLE, style);
	// event callback
	glfwSetKeyCallback(glwindow, key_callback);
	glfwSetMouseButtonCallback(glwindow, mouse_button_callback);
	glfwSetCursorPosCallback(glwindow, cursor_position_callback);

	/***************** Init glew ********************/
	err = glewInit();// IMPORTANT:must after glew init
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	// init Rapid2D
	Rapid2D_startup(s_realWidth, s_realHeight);

	LARGE_INTEGER nNow;
	LARGE_INTEGER nLast;
	LARGE_INTEGER nFreq;

	QueryPerformanceFrequency(&nFreq);
	nFreq.QuadPart /= 60;// Refash rate
	QueryPerformanceCounter(&nLast);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(glwindow))
	{
		/* Render Frame */
		QueryPerformanceCounter(&nNow);
		if (nNow.QuadPart - nLast.QuadPart > nFreq.QuadPart)
		{
			nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % nFreq.QuadPart);

			if (s_frameCB) {
				s_frameCB();
				/* Swap front and back buffers */
				glfwSwapBuffers(glwindow);
			}
		} else {
			Sleep(1);
		}

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
