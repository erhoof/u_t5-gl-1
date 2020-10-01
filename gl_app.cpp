
#include "gl_app.h"

// Global Variables
HDC hDC = nullptr; // Device Context for the client area of a smt
HGLRC hRC = nullptr; // Rendering Context
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr; // Pixel Format: opts of FB
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr; // Context Attribs

void InitScene() // scene init
{
	glClearColor(0.25f, 1.0f, 0.25f, 0.0f);

	// TODO: other init stuff
}

ATOM InitOpenGLExt(HWND hWnd) // OpenGL extensions init
{
	hDC = GetDC(hWnd);

	if (!hDC)
		return 1;

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

	if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB)
		return 2;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrcFake);

	return 0;
}

ATOM InitOpenGL(HWND hWnd)
{
	GLint pixelFormat = 0;
	GLuint numFormats = 0;

	PIXELFORMATDESCRIPTOR pfd = { 0 };

	const GLint pixelFormatAttribList[] = {
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // RGBA Support
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_COLOR_BITS_ARB, 24, // 8+8+8 8bit space
		WGL_ALPHA_BITS_ARB, 8,
		0 // End of list
	};

	GLint contextAttribVersion[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // 3
		WGL_CONTEXT_MINOR_VERSION_ARB, 5, // 3
		0
	};

	/* Check for compatable pixel format
	 BOOL wglChoosePixelFormatARB(HDC hdc,
                                 const int *piAttribIList,
                                 const FLOAT *pfAttribFList,
                                 UINT nMaxFormats,
                                 int *piFormats,
                                 UINT *nNumFormats);
	*/
	wglChoosePixelFormatARB(hDC,
		pixelFormatAttribList,
		NULL,
		1,
		&pixelFormat,
		&numFormats);

	SetPixelFormat(hDC, pixelFormat, &pfd);

	// Create rendering context
	hRC = wglCreateContextAttribsARB(hDC, 0, contextAttribVersion);

	if (!hRC)
		return 1; // Error handling

	// Make cur context
	wglMakeCurrent(hDC, hRC);

	InitScene();
}

void DeInitOpenGL(HWND hWnd)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}

void Reshape(GLsizei width, GLsizei height, GLint x, GLint y)
{
	if (!height)
		return;

	glViewport(x, y, width, height); // Set Viewport
}

void DrawScene(DWORD elapsedTime)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// etc...
}

void CALLBACK DrawTimer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	DrawScene(dwTime); // Current time

	SwapBuffers(hDC); // Swap screen buffer
}