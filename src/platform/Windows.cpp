#ifdef _WIN32

#include "../cx.h"
#include "../window/WindowBase.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>

#define WND_HWND ((Win32Wnd*)m_Window)->hWnd
#define WND_DC ((Win32Wnd*)m_Window)->dc
#define WND_RC ((Win32Wnd*)m_Window)->rc


struct Win32Wnd
{
    HWND hWnd;	// Window handle
    HDC dc;		// Device context
    HGLRC rc;	// Render context
};


bool RegisterWindowClass();

const wchar_t CLASS_NAME[] = L"Main_Window";
bool g_ClassRegistered = RegisterWindowClass();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    cxWindowBase* wnd = (cxWindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg)
    {
    case WM_CREATE:
    {
        CREATESTRUCT* CrtStrPtr = (CREATESTRUCT*)lParam;
        cxWindowBase* wnd1 = (cxWindowBase*)(CrtStrPtr->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd1);

        wnd1->OnInit();

        return 0;
    }
    case WM_PAINT:
    {
        Win32Wnd* pWnd = (Win32Wnd*)(wnd->GetPlatformWindow());
        wglMakeCurrent(pWnd->dc, pWnd->rc);

        wnd->OnPaint();

        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_SIZE:
    {
        wnd->OnSize(LOWORD(lParam), HIWORD(lParam));
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        wnd->OnMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_LBUTTONUP:
    {
        wnd->OnMouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        wnd->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_CLOSE:
    {
        wnd->OnClosing();
        delete wnd;
        return 0;
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void cxWindowBase::CreateContext()
{
    WND_DC = GetDC(WND_HWND);
    if (!WND_DC)
        return;

    int pixelFormat;
    PIXELFORMATDESCRIPTOR pixelFormatDesc;

    /* initialize bits to 0 */
    memset(&pixelFormatDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelFormatDesc.nVersion = 1;
    pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
    pixelFormatDesc.cColorBits = 32;
    pixelFormatDesc.cAlphaBits = 8;
    pixelFormatDesc.cDepthBits = 24;

    pixelFormat = ChoosePixelFormat(WND_DC, &pixelFormatDesc);
    if (!pixelFormat)
        return;

    if (!SetPixelFormat(WND_DC, pixelFormat, &pixelFormatDesc))
        return;

    WND_RC = wglCreateContext(WND_DC);
    if (!WND_RC)
        return;

}

cxWindowBase::cxWindowBase()
{
	m_Window = new Win32Wnd;

    HINSTANCE hInstance = GetModuleHandle(0);

	WND_HWND = CreateWindowEx(0, CLASS_NAME, L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, this);

    if (m_Window == NULL)
        return;

    CreateContext();

}

cxWindowBase::~cxWindowBase()
{
    wglMakeCurrent(WND_DC, NULL);
    wglDeleteContext(WND_RC);
    ReleaseDC(WND_HWND, WND_DC);

    DestroyWindow(WND_HWND);

	delete m_Window;
}

void cxWindowBase::SetTitle(std::wstring title)
{
    SetWindowTextW(WND_HWND, title.c_str());
}

void cxWindowBase::SetPosition(int x, int y)
{
    SetWindowPos(WND_HWND, NULL, x, y, 0, 0, SWP_NOSIZE);
}

void cxWindowBase::SetSize(int width, int height)
{
    SetWindowPos(WND_HWND, NULL, 0, 0, width, height, SWP_NOMOVE);
}

void cxWindowBase::Show(bool show)
{
    ShowWindow(WND_HWND, show);
}

void cxWindowBase::Invalidate()
{
    InvalidateRect(WND_HWND, NULL, TRUE);
}

bool RegisterWindowClass()
{
    HINSTANCE hInstance = GetModuleHandle(0);

    WNDCLASSEXW wc = { };

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wc);

    return true;
}

void cxRunApp()
{
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void cxQuitApp(int exitCode)
{
    PostQuitMessage(exitCode);
}

void cxMessageBox(std::wstring text)
{
    MessageBoxExW(NULL, text.c_str(), L"", MB_OK, NULL);
}

#endif