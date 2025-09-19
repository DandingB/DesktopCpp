#ifdef _WIN32
#include "WindowBase.h"

HCURSOR cArrow = LoadCursor(NULL, IDC_ARROW);
HCURSOR cSizeWE = LoadCursor(NULL, IDC_SIZEWE);

bool RegisterWindowClass();

const wchar_t CLASS_NAME[] = L"Main_Window";
bool g_ClassRegistered = RegisterWindowClass();

ID2D1Factory* m_pDirect2dFactory;
IDWriteFactory* pDWriteFactory;


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

        return 0;
    }
    case WM_PAINT:
    {
        wnd->GetWin32RenderTarget()->BeginDraw();
        wnd->GetWin32RenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
        wnd->GetWin32RenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));

        D2D1_SIZE_F rtSize = wnd->GetWin32RenderTarget()->GetSize();
        int width = static_cast<int>(rtSize.width);
        int height = static_cast<int>(rtSize.height);

        wnd->OnPaint();

        wnd->GetWin32RenderTarget()->EndDraw();

        return 0;
    }
    case WM_SIZE:
    {
        if (wnd->GetWin32RenderTarget())
        {
            wnd->GetWin32RenderTarget()->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
        }

        wnd->OnSize(LOWORD(lParam), HIWORD(lParam));
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        wnd->OnMouseDown({ (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), LEFT });
        return 0;
    }
    case WM_LBUTTONUP:
    {
        wnd->OnMouseUp({ (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), LEFT });
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        wnd->OnMouseDown({ (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), RIGHT });
        return 0;
    }
    case WM_RBUTTONUP:
    {
        wnd->OnMouseUp({ (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), RIGHT });
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        wnd->OnMouseMove({ (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), NONE });
        return 0;
    }
    case WM_DPICHANGED:
    {
        int dpi = HIWORD(wParam);

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



cxWindowBase::cxWindowBase()
{
    HINSTANCE hInstance = GetModuleHandle(0);

	m_hWnd = CreateWindowEx(0, CLASS_NAME, L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, this);

    if (m_hWnd == NULL)
        return;

    RECT rc;
    GetClientRect(m_hWnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top);

    // Create a Direct2D render target.
    m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hWnd, size),
        &m_pRenderTarget
    );


}

cxWindowBase::~cxWindowBase()
{
    DestroyWindow(m_hWnd);
}

void cxWindowBase::SetTitle(std::wstring title)
{
    SetWindowTextW(m_hWnd, title.c_str());
}

void cxWindowBase::SetPosition(int x, int y)
{
    SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
}

void cxWindowBase::SetSize(int width, int height)
{
    SetWindowPos(m_hWnd, NULL, 0, 0, width, height, SWP_NOMOVE);
}

void cxWindowBase::GetTitle(std::wstring& out)
{
    wchar_t buffer[128] = { '\0' };
    GetWindowTextW(m_hWnd, buffer, 128);
    out = std::wstring(buffer);
}

void cxWindowBase::GetPosition(int& x, int& y)
{
    RECT rect;
    GetWindowRect(m_hWnd, &rect);
    x = rect.left;
    y = rect.top;
}

void cxWindowBase::GetSize(int& width, int& height)
{
    RECT rect;
    GetWindowRect(m_hWnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}

void cxWindowBase::GetClientSize(int& width, int& height)
{
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    width = rect.right;
    height = rect.bottom;
}

void cxWindowBase::Show(bool show)
{
    ShowWindow(m_hWnd, show);
}

void cxWindowBase::ShowCursor(bool show)
{
    ::ShowCursor(show);
}

void cxWindowBase::SetCursor(cxCursorType type)
{
    switch (type)
    {
    case cxArrow:
    {
        ::SetCursor(cArrow);
        return;
    }
    case cxIBeam:
        return;
    case cxPointingHand:
        return;
    case cxHand:
        return;
    case cxGrab:
        return;
    case cxCrosshair:
        return;
    case cxSizeWE:
    {   
        ::SetCursor(cSizeWE);
        return;
    }
    case cxSizeNS:
        return;
    case cxNo:
        return;
    default:
        return;
    }
}

void cxWindowBase::CaptureMouse()
{
    SetCapture(m_hWnd);
}

void cxWindowBase::ReleaseMouse()
{
    ReleaseCapture();
}


void cxWindowBase::Invalidate()
{
    InvalidateRect(m_hWnd, NULL, TRUE);
}

void cxWindowBase::SetDrawConstraints(cxRect rect)
{
    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(rect.left, rect.top));
    m_pRenderTarget->PushAxisAlignedClip(D2D1::RectF(0, 0, rect.right - rect.left, rect.bottom - rect.top), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}

void cxWindowBase::RemoveDrawConstraints()
{
    m_pRenderTarget->PopAxisAlignedClip();
}

void cxWindowBase::MakeSolidBrush(int key, float r, float g, float b, float a)
{
    ComPtr<ID2D1SolidColorBrush> brush;
    m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF(r, g, b, a)),
        &brush
    );
    m_pBrushes.insert({ key, brush });
}

void cxWindowBase::MakeFont(int key, std::wstring fontName, float size)
{
    ComPtr<IDWriteTextFormat> font;
    pDWriteFactory->CreateTextFormat(
        fontName.c_str(),                // Font family name.
        NULL,                       // Font collection (NULL sets it to use the system font collection).
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        size,
        L"en-us",
        &font
    );

    font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    m_pFonts.insert({ key, font });
}

void cxWindowBase::GetFontTextMetrics(int key, std::wstring str, float maxWidth, float maxHeight, cxTextOptions options, float& width, float& height)
{
    HRESULT hr = S_OK;

    IDWriteTextFormat* format = m_pFonts[key].Get();

    switch (options.m_TextAlignment)
    {
        case cxTextOptions::TEXT_ALIGNMENT_LEFT: format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING); break;
        case cxTextOptions::TEXT_ALIGNMENT_CENTER: format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); break;
        case cxTextOptions::TEXT_ALIGNMENT_RIGHT: format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); break;
    }
    

    ComPtr<IDWriteTextLayout> pTextLayout;
    hr = pDWriteFactory->CreateTextLayout(str.c_str(), static_cast<UINT32>(str.size()), format, maxWidth, maxHeight, pTextLayout.GetAddressOf());

    if (SUCCEEDED(hr))
    {
        // Get text size  
        DWRITE_TEXT_METRICS textMetrics;
        hr = pTextLayout->GetMetrics(&textMetrics);
        D2D1_SIZE_F size = D2D1::SizeF(ceil(textMetrics.widthIncludingTrailingWhitespace), ceil(textMetrics.height));

        width = size.width;
        height = size.height;
    }
}

void cxWindowBase::FillRectangle(cxRect rect, int brushKey)
{
    m_pRenderTarget->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), m_pBrushes[brushKey].Get());
}

void cxWindowBase::DrawRectangle(cxRect rect, int brushKey, float strokeWidth)
{
    m_pRenderTarget->DrawRectangle(D2D1::RectF(0.5f + rect.left, 0.5f + rect.top, -0.5f + rect.right, -0.5f + rect.bottom), m_pBrushes[brushKey].Get(), strokeWidth);
}

void cxWindowBase::FillRoundedRectangle(cxRect rect, float r1, float r2, int brushKey)
{
    m_pRenderTarget->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), r1, r2), m_pBrushes[brushKey].Get());
}

void cxWindowBase::DrawRoundedRectangle(cxRect rect, float r1, float r2, int brushKey, float strokeWidth)
{
    m_pRenderTarget->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), r1, r2), m_pBrushes[brushKey].Get(), strokeWidth);
}

void cxWindowBase::DrawText(int fontKey, int brushKey, std::wstring str, cxRect rect, cxTextOptions options)
{

    IDWriteTextFormat* format = m_pFonts[fontKey].Get();

    switch (options.m_TextAlignment)
    {
        case cxTextOptions::TEXT_ALIGNMENT_LEFT: format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING); break;
        case cxTextOptions::TEXT_ALIGNMENT_CENTER: format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); break;
        case cxTextOptions::TEXT_ALIGNMENT_RIGHT: format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING); break;
    }

    m_pRenderTarget->DrawText(
        str.c_str(),
        str.length(),
        format,
        D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom),
        m_pBrushes[brushKey].Get()
    );
}


float cxWindowBase::GetDPIScale()
{
    return (float)GetDpiForWindow(m_hWnd) / USER_DEFAULT_SCREEN_DPI;
}




bool RegisterWindowClass()
{
    HRESULT hr = S_OK;

    HINSTANCE hInstance = GetModuleHandle(0);

    WNDCLASSEXW wc = { };

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wc);

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&m_pDirect2dFactory));

    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&pDWriteFactory)
    );

    //HWND hDesktop = CreateWindowEx(0, L"Main_Window", L"", NULL, 0, 0, 500, 500, NULL, NULL, hInstance, NULL);

    return true;
}

void cxInitApp()
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
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

void cxLog(std::wstring str, ...)
{
    va_list args;
    va_start(args, str);

    int len = _vscwprintf(str.c_str(), args);
    wchar_t* buffer = new wchar_t[len + 1];
    vswprintf(buffer, len + 1, str.c_str(), args);

    va_end(args);

    OutputDebugStringW(buffer);

    delete[] buffer;
}

void cxGetMousePosition(int& x, int& y)
{
    POINT pt;
    GetCursorPos(&pt);

    x = pt.x;
    y = pt.y;
}

#endif