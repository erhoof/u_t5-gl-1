
#include "gl_app.h"

void InitScene() // scene init
{
	glClearColor(0.25f, 1.0f, 0.25f, 0.0f);

	// TODO: other init stuff
}

void InitOpenGLExt(HWND hWnd) // OpenGL extensions init
{
	hDC = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd = { 0 }; // Pixel Format description
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Structure size
	pfd.dwFlags =
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | // OpenGL support
		PFD_DOUBLEBUFFER; // Double Buffer support
	pfd.iLayerType = PFD_MAIN_PLANE; // Layer type - Main / Overlay / Underlay
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 24;
	pfd.cColorBits = 24; // R + G + B (8+8+8) - 8 bit space
	pfd.cAlphaBits = 8;
	pfd.nVersion = 1; // Data structure version. Only v1 exists =)

	// Find closest available pfd
	auto pixelFormat = ChoosePixelFormat(hDC, &pfd);
	
	// Set pixel foramt
	SetPixelFormat(hDC, pixelFormat, &pfd);

	// Get pfn of opengl for windows exts by fake hrc
	auto hrcFake = wglCreateContext(hDC); // Create temp rendering context
	wglMakeCurrent(hDC, hrcFake); // Set cur context
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)
		wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
		wglGetProcAddress("wglCreateContextAttribsARB");

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrcFake);
}