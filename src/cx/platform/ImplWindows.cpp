#ifdef _WIN32

#define WINVER 0x0605
#include <windows.h>
#include <windowsx.h>
#include <wrl.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#include <map>
#include <string>

#include "WindowBase.h"
#include "Font.h"
#include "Platform.h"

#undef GetStringMetrics

using Microsoft::WRL::ComPtr;

HCURSOR cArrow = LoadCursor(NULL, IDC_ARROW);
HCURSOR cIBeam = LoadCursor(NULL, IDC_IBEAM);
HCURSOR cSizeWE = LoadCursor(NULL, IDC_SIZEWE);

bool RegisterWindowClass();

const wchar_t CLASS_NAME[] = L"cxWindowClass";
bool g_ClassRegistered = RegisterWindowClass();

ID2D1Factory* pD2DFactory;
IDWriteFactory* pDWriteFactory;


struct cxWindowBase::Impl
{
    HWND m_hWnd;
    ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
    std::map<int, ComPtr<ID2D1Brush>> m_pBrushes;
};

struct cxFont::Impl
{
    ComPtr<IDWriteTextFormat> m_pTextFormat;
};

class WndProc
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        cxWindowBase* wnd = (cxWindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

        switch (uMsg)
        {
        case WM_CREATE:
        {
            CREATESTRUCT* CrtStrPtr = (CREATESTRUCT*)lParam;
            cxWindowBase* wnd1 = (cxWindowBase*)(CrtStrPtr->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd1);

            CreateCaret(hwnd, (HBITMAP)NULL, 1, 20 * ((float)GetDpiForWindow(hwnd) / USER_DEFAULT_SCREEN_DPI));

            return 0;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            wnd->p->m_pRenderTarget->BeginDraw();
            wnd->p->m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
            wnd->p->m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

            D2D1_SIZE_F rtSize = wnd->p->m_pRenderTarget->GetSize();
            int width = static_cast<int>(rtSize.width);
            int height = static_cast<int>(rtSize.height);

            wnd->OnPaint();

            wnd->p->m_pRenderTarget->EndDraw();

            EndPaint(hwnd, &ps);

            ValidateRect(hwnd, NULL);
            return 0;
        }
        case WM_SIZE:
        {
            if (wnd->p->m_pRenderTarget)
                wnd->p->m_pRenderTarget->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));

            wnd->OnSize(LOWORD(lParam) / wnd->GetDPIScale(), HIWORD(lParam) / wnd->GetDPIScale());
            return 0;
        }
        case WM_LBUTTONDBLCLK:
        case WM_LBUTTONDOWN:
        {
            wnd->OnMouseDown({ (float)GET_X_LPARAM(lParam) / wnd->GetDPIScale(), (float)GET_Y_LPARAM(lParam) / wnd->GetDPIScale(), cxMouseEvent::LEFT });
            return 0;
        }
        case WM_LBUTTONUP:
        {
            wnd->OnMouseUp({ (float)GET_X_LPARAM(lParam) / wnd->GetDPIScale(), (float)GET_Y_LPARAM(lParam) / wnd->GetDPIScale(), cxMouseEvent::LEFT });
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            wnd->OnMouseDown({ (float)GET_X_LPARAM(lParam) / wnd->GetDPIScale(), (float)GET_Y_LPARAM(lParam) / wnd->GetDPIScale(), cxMouseEvent::RIGHT });
            return 0;
        }
        case WM_RBUTTONUP:
        {
            wnd->OnMouseUp({ (float)GET_X_LPARAM(lParam) / wnd->GetDPIScale(), (float)GET_Y_LPARAM(lParam) / wnd->GetDPIScale(), cxMouseEvent::RIGHT });
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.hwndTrack = hwnd;
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.dwHoverTime = HOVER_DEFAULT;
            TrackMouseEvent(&tme);

            wnd->OnMouseMove({ (float)GET_X_LPARAM(lParam) / wnd->GetDPIScale(), (float)GET_Y_LPARAM(lParam) / wnd->GetDPIScale(), cxMouseEvent::NONE });
            return 0;
        }
        case WM_MOUSELEAVE:
        {
            wnd->OnMouseLeave();
            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            float wheel = (SHORT)HIWORD(wParam);

            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hwnd, &p);

            wnd->OnMouseScroll({ (float)p.x / wnd->GetDPIScale(), (float)p.y / wnd->GetDPIScale(), 0, wheel });
            return 0;
        }
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_LEFT:   wnd->OnKeyDown({ cxKeyEvent::ARROW_LEFT }); return 0;
            case VK_RIGHT:  wnd->OnKeyDown({ cxKeyEvent::ARROW_RIGHT }); return 0;
            case VK_UP:     wnd->OnKeyDown({ cxKeyEvent::ARROW_UP }); return 0;
            case VK_DOWN:   wnd->OnKeyDown({ cxKeyEvent::ARROW_DOWN }); return 0;
            case VK_BACK:   wnd->OnKeyDown({ cxKeyEvent::BACKSPACE });  return 0;
            }
            return 0;
        }
        case WM_CHAR:
        {
            wnd->OnCharacter({ cxKeyEvent::CHARACTER, (wchar_t)wParam });
            return 0;
        }
        case WM_DPICHANGED:
        {
            int dpi = HIWORD(wParam);

            return 0;
        }
        case WM_COMMAND:
        {
            std::function<void(int)> callback;
            wnd->GetMenuCallback(callback);
            if (callback != nullptr)
                callback(wParam);
            return 0;
        }
        case WM_KILLFOCUS:
        {
            wnd->OnFocusLost();
            return 0;
        }
        case WM_CLOSE:
        {
            wnd->OnClosing();
            return 0;
        }
        case WM_DESTROY:
        {
            DestroyCaret();
            return 0;
        }
        return 0;

        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};


cxWindowBase::cxWindowBase() : p(std::make_unique<Impl>())
{
    HINSTANCE hInstance = GetModuleHandle(0);

	p->m_hWnd = CreateWindowEx(0, CLASS_NAME, L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, this);
    if (p->m_hWnd == NULL)
    {
        cxLog(L"m_hWnd could not be created. Closing");
        exit(1);
    }

    RECT rc;
    GetClientRect(p->m_hWnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top);

    // Create a Direct2D render target.
    pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(p->m_hWnd, size),
        &p->m_pRenderTarget
    );
}

cxWindowBase::~cxWindowBase()
{
    DestroyWindow(p->m_hWnd);
}

void cxWindowBase::Show(bool show)
{
    ShowWindow(p->m_hWnd, show);
}

void cxWindowBase::SetTitle(std::wstring title)
{
    SetWindowTextW(p->m_hWnd, title.c_str());
}

void cxWindowBase::SetPosition(int x, int y)
{
    SetWindowPos(p->m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
}

void cxWindowBase::SetSize(int width, int height)
{
    SetWindowPos(p->m_hWnd, NULL, 0, 0, width, height, SWP_NOMOVE);
}

void cxWindowBase::GetTitle(std::wstring& out)
{
    wchar_t buffer[128] = { '\0' };
    GetWindowTextW(p->m_hWnd, buffer, 128);
    out = std::wstring(buffer);
}

void cxWindowBase::GetPosition(int& x, int& y)
{
    RECT rect;
    GetWindowRect(p->m_hWnd, &rect);
    x = rect.left;
    y = rect.top;
}

void cxWindowBase::GetSize(int& width, int& height)
{
    RECT rect;
    GetWindowRect(p->m_hWnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}

void cxWindowBase::GetClientSize(float& width, float& height)
{
    RECT rect;
    GetClientRect(p->m_hWnd, &rect);
    width = (float)rect.right * GetDPIScale();
    height = (float)rect.bottom * GetDPIScale();
}

void EnumerateMenu(HMENU hMenu, std::vector<cxMenuItem>& menu)
{
    for (cxMenuItem& item : menu)
    {
        HMENU subMenu = NULL;
        if (item.subitems.size() > 0)
        {
            subMenu = CreateMenu();
            EnumerateMenu(subMenu, item.subitems);
        }
        AppendMenuW(hMenu, subMenu ? MF_POPUP : MF_STRING, (subMenu ? (UINT_PTR)subMenu : item.command), item.title.c_str());
    }
}

void cxWindowBase::SetMenu(std::vector<cxMenuItem>& menu)
{
    HMENU hMenu = CreateMenu();
    EnumerateMenu(hMenu, menu);

    ::SetMenu(p->m_hWnd, hMenu);
}

void cxWindowBase::SetMenuCallback(std::function<void(int)> callback)
{
    m_MenuCallback = callback;
}

void cxWindowBase::GetMenuCallback(std::function<void(int)>& callback)
{
    callback = m_MenuCallback;
}

void cxWindowBase::CaptureMouse()
{
    SetCapture(p->m_hWnd);
}

void cxWindowBase::ReleaseMouse()
{
    ReleaseCapture();
}

void cxWindowBase::ShowCaret(bool show)
{
    if (show)
        ::ShowCaret(p->m_hWnd);
    else
        ::HideCaret(p->m_hWnd);
}

void cxWindowBase::SetCaretPos(cxPoint p)
{
    ::SetCaretPos(p.x * GetDPIScale(), p.y * GetDPIScale());
}

void cxWindowBase::Invalidate()
{
    InvalidateRect(p->m_hWnd, NULL, TRUE);
}

void cxWindowBase::SetDrawConstraints(cxRect rect)
{
    p->m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(rect.left, rect.top));
    p->m_pRenderTarget->PushAxisAlignedClip(D2D1::RectF(0, 0, rect.right - rect.left, rect.bottom - rect.top), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}

void cxWindowBase::RemoveDrawConstraints()
{
    p->m_pRenderTarget->PopAxisAlignedClip();
}

void cxWindowBase::MakeSolidBrush(int key, float r, float g, float b, float a)
{
    ComPtr<ID2D1SolidColorBrush> brush;
    p->m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF(r, g, b, a)),
        &brush
    );
    p->m_pBrushes.insert({ key, brush });
}

void cxWindowBase::FillRectangle(cxRect rect, int brushKey)
{
    p->m_pRenderTarget->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), p->m_pBrushes[brushKey].Get());
}

void cxWindowBase::DrawRectangle(cxRect rect, int brushKey, float strokeWidth)
{
    p->m_pRenderTarget->DrawRectangle(D2D1::RectF(0.5f + rect.left, 0.5f + rect.top, -0.5f + rect.right, -0.5f + rect.bottom), p->m_pBrushes[brushKey].Get(), strokeWidth);
}

void cxWindowBase::FillRoundedRectangle(cxRect rect, float r1, float r2, int brushKey)
{
    p->m_pRenderTarget->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), r1, r2), p->m_pBrushes[brushKey].Get());
}

void cxWindowBase::DrawRoundedRectangle(cxRect rect, float r1, float r2, int brushKey, float strokeWidth)
{
    p->m_pRenderTarget->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(0.5f + rect.left, 0.5f + rect.top, -0.5f + rect.right, -0.5f + rect.bottom), r1, r2), p->m_pBrushes[brushKey].Get(), strokeWidth);
}

void cxWindowBase::DrawLine(cxPoint p1, cxPoint p2, int brushKey, float strokeWidth)
{
    p->m_pRenderTarget->DrawLine(D2D1::Point2F(p1.x, p1.y), D2D1::Point2F(p2.x, p2.y), p->m_pBrushes[brushKey].Get(), strokeWidth);
}

void cxWindowBase::DrawTextInRect(cxFont* font, int brushKey, std::wstring str, cxRect rect)
{
    if (!font)
    {
        cxLog(L"DrawTextInRect(): font was NULL");
        return;
    }

    IDWriteTextFormat* format = font->p->m_pTextFormat.Get();
    if (!format)
    {
        cxLog(L"DrawTextInRect(): m_pTextFormat was NULL");
        return;
    }

    p->m_pRenderTarget->DrawTextW(
        str.c_str(),
        str.length(),
        format,
        D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom),
        p->m_pBrushes[brushKey].Get()
    );
}


float cxWindowBase::GetDPIScale()
{
    return (float)GetDpiForWindow(p->m_hWnd) / USER_DEFAULT_SCREEN_DPI;
}

cxFont::cxFont(std::wstring fontName, float size) : p(std::make_unique<Impl>())
{
    //IDWriteFontCollection* m_dwFontColl;
    //if (FAILED(pDWriteFactory->GetSystemFontCollection(&m_dwFontColl, false)))
    //{
    //    OutputDebugString(L"Failed to retrieve system font collection.\n");
    //}

    pDWriteFactory->CreateTextFormat(
        fontName.c_str(),                       // Font family name.
        NULL,                                   // Font collection (NULL sets it to use the system font collection).
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        size,
        L"en-US",
        &p->m_pTextFormat
    );

    //p->m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    //p->m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

cxFont::~cxFont()
{

}

void cxFont::SetTextOptions(cxTextOptions options)
{
    IDWriteTextFormat* textFormat = p->m_pTextFormat.Get();

    switch (options.m_TextAlignment)
    {
    case cxTextOptions::TEXT_ALIGNMENT_LEFT: textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING); break;
    case cxTextOptions::TEXT_ALIGNMENT_CENTER: textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); break;
    case cxTextOptions::TEXT_ALIGNMENT_RIGHT: textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); break;
    }

    switch (options.m_ParagraphAlignment)
    {
    case cxTextOptions::PARAGRAPH_ALIGNMENT_TOP: textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR); break;
    case cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER: textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); break;
    case cxTextOptions::PARAGRAPH_ALIGNMENT_BOTTOM: textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR); break;
    }

    switch (options.m_WordWrapping)
    {
    case cxTextOptions::WORD_WRAPPING_NONE: textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP); break;
    case cxTextOptions::WORD_WRAPPING_WORD: textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP); break;
    case cxTextOptions::WORD_WRAPPING_CHARACTER: textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_CHARACTER); break;
    }
}

void cxFont::GetStringMetrics(std::wstring str, float maxWidth, float maxHeight, float& width, float& height)
{
    HRESULT hr;

    IDWriteTextFormat* textFormat = p->m_pTextFormat.Get();

    ComPtr<IDWriteTextLayout> pTextLayout;
    hr = pDWriteFactory->CreateTextLayout(str.c_str(), static_cast<UINT32>(str.size()), textFormat, maxWidth, maxHeight, pTextLayout.GetAddressOf());

    if (SUCCEEDED(hr))
    {
        DWRITE_TEXT_METRICS textMetrics;
        hr = pTextLayout->GetMetrics(&textMetrics);
        D2D1_SIZE_F size = D2D1::SizeF(ceil(textMetrics.widthIncludingTrailingWhitespace), ceil(textMetrics.height));

        width = size.width;
        height = size.height;
    }
}

void cxFont::GetCharPosition(std::wstring str, int iChar, float maxWidth, float maxHeight, cxPoint& out)
{
    HRESULT hr;

    IDWriteTextFormat* textFormat = p->m_pTextFormat.Get();

    ComPtr<IDWriteTextLayout> pTextLayout;
    hr = pDWriteFactory->CreateTextLayout(str.c_str(), static_cast<UINT32>(str.size()), textFormat, maxWidth, maxHeight, pTextLayout.GetAddressOf());

    if (SUCCEEDED(hr))
    {
        DWRITE_HIT_TEST_METRICS metrics;
        pTextLayout->HitTestTextPosition(iChar, false, &out.x, &out.y, &metrics);
    }
}


bool RegisterWindowClass()
{
    HINSTANCE hInstance = GetModuleHandle(0);

    WNDCLASSEXW wc = { };

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WndProc::WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wc);


    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&pD2DFactory))))
    {
        cxLog(L"D2D1CreateFactory() failed");
        return false;
    }

    if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory))))
    {
        cxLog(L"DWriteCreateFactory() failed");
        return false;
    }

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

void cxShowCursor(bool show)
{
    ::ShowCursor(show);
}

void cxSetCursor(cxCursorType type)
{
    switch (type)
    {
    case cxArrow:
        ::SetCursor(cArrow);
        return;
    case cxIBeam:
        ::SetCursor(cIBeam);
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
        ::SetCursor(cSizeWE);
        return;
    case cxSizeNS:
        return;
    case cxNo:
        return;
    default:
        return;
    }

}

void cxRegisterFontFile(std::wstring file)
{
    //if (AddFontResourceExW(file.c_str(), FR_PRIVATE, NULL) == 0)
    //{
    //    OutputDebugStringW(L"Error adding font resource!\n");
    //    exit(1);
    //}



    //// Create and register the loader
    //auto loader = new SingleFileFontLoader(pDWriteFactory, file);
    //pDWriteFactory->RegisterFontCollectionLoader(loader);

    //// The "key" can be anything, we don't use it in our loader
    //pDWriteFactory->CreateCustomFontCollection(
    //    loader,
    //    nullptr, // collectionKey
    //    0,       // collectionKeySize
    //    &fontCollection
    //);

    //// Extract family name
    //ComPtr<IDWriteFontFamily> family;
    //fontCollection->GetFontFamily(0, &family);

    //ComPtr<IDWriteLocalizedStrings> names;
    //family->GetFamilyNames(&names);

    //UINT32 index = 0;
    //BOOL exists = FALSE;
    //names->FindLocaleName(L"en-us", &index, &exists);
    //if (!exists) index = 0;

    //WCHAR familyName[100];
    //names->GetString(index, familyName, ARRAYSIZE(familyName));

}


void cxOpenFileDialog(std::wstring& filename)
{
    OPENFILENAME ofn;       // common dialog box structure
    WCHAR szFile[260];     // buffer for file name

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE)
    {
        filename = std::wstring(ofn.lpstrFile);
    }
}


#endif