#pragma once

#include <Windows.h>

#include <GL/gl.h>
#include <KHR/khrplatform.h> // EGL Platform : ogl <==> windows
#include <GL/glext.h> // OpenGL 1.2 +
#include <GL/glcorearb.h> // OpenGL core profile / ARB Extensions
#include <GL/wglext.h> // Windows OpenGL Extension

#define UPDATE_TIMER_ID 200 // for timer later

void InitScene(); // scene init
ATOM InitOpenGLExt(HWND hWnd); // OpenGL extensions init
ATOM InitOpenGL(HWND hWnd); // Init OpenGL Core
void DeInitOpenGL(HWND hWnd);

void Reshape(GLsizei width, GLsizei height, GLint x = 0, GLint y = 0); // Reshape GL Scene
void DrawScene(DWORD elapsedTime); // Clear and Redraw GL Scene

// window - WM_TIMER message - Timer identifier - Current System time
void CALLBACK DrawTimer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);





