#pragma once

#include <Windows.h>

#include <GL/gl.h>
#include <KHR/khrplatform.h> // EGL Platform : ogl <==> windows
#include <GL/glext.h> // OpenGL 1.2 +
#include <GL/glcorearb.h> // OpenGL core profile / ARB Extensions
#include <GL/wglext.h> // Windows OpenGL Extension

#define UPDATE_TIMER_ID 200 // for timer later

// Global Variables
HDC hDC = nullptr; // Device Context for the client area of a smt
HGLRC hRC = nullptr; // Rendering Context
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr; // Pixel Format: opts of FB
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr; // Context Attribs

void InitScene(); // scene init
void InitOpenGLExt(HWND hWnd); // OpenGL extensions init
