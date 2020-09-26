
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

void InitOpenGL(HWND hWnd)
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

	// Make cur context
	wglMakeCurrent(hDC, hRC);

	InitScene();
}