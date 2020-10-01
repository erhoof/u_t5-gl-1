// u_t5-gl-1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "u_t5-gl-1.h"

#include "gl_app.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_UT5GL1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UT5GL1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_OWNDC; // Allocates unique DC
    wcex.lpfnWndProc    = WndProc;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UT5GL1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   // Overlapping, With TitleBar, With SizingBorder
   /*HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/
   HWND hWnd = CreateWindowW(szWindowClass,
       szTitle,
       WS_OVERLAPPED | WS_SYSMENU | WS_SIZEBOX,
       CW_USEDEFAULT,
       0,
       CW_USEDEFAULT,
       0,
       nullptr,
       nullptr,
       hInstance,
       nullptr
   );

   if (!hWnd)
   {
      return FALSE;
   }
   
   // OpenGL Context
   switch (InitOpenGLExt(hWnd))
   {
   case 1:
       MessageBox(hWnd, L"Could not init Device Context", L"DC error.", MB_ICONERROR);
       ExitProcess(-1);
       break;
   case 2:
       MessageBox(hWnd, L"Could not get OpenGL extensions", L"Extensions error.", MB_ICONERROR);
       ExitProcess(-1);
       break;
   default:
       break;
   }

   if (InitOpenGL(hWnd) == 1)
   {
       MessageBox(hWnd, L"Could not create Rendering Context", L"RC error.", MB_ICONERROR);
       ExitProcess(-1);
   }

   // Add Hotkey CTRL+Q
   if (RegisterHotKey(
       hWnd,
       1, //ID
       MOD_CONTROL | MOD_NOREPEAT,
       0x51 // Q key
   ))
   {
       _tprintf(_T("Hotkey 'CTRL+Q' defined with MOD_NOREPEAT flag\n"));
   }


   ShowWindow(hWnd, nCmdShow);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_ACTIVATE:
        {
            int wmId = LOWORD(wParam);
            // Parse window activate state
            switch (wmId)
            {
            case WA_ACTIVE:
            case WA_CLICKACTIVE:
                SetTimer(hWnd,
                    UPDATE_TIMER_ID,
                    USER_TIMER_MINIMUM,
                    (TIMERPROC)&DrawTimer);
                break;
            case WA_INACTIVE:
                KillTimer(hWnd, UPDATE_TIMER_ID);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
                DestroyWindow(hWnd);
            break;
        }
    case WM_HOTKEY:
        {
            if (wParam == 1)
                PostQuitMessage(WM_CLOSE);
            break;
        }
    case WM_SIZE:
        {
            Reshape(LOWORD(lParam), HIWORD(lParam));
            break;
        }
    case WM_DESTROY:
        KillTimer(hWnd, UPDATE_TIMER_ID);
        DeInitOpenGL(hWnd);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
