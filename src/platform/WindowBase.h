#pragma once
#include <string>
#include "Platform.h"
#include "../cx/Event.h"
#include <vector>
#include <map>

enum cxCursorType
{
    cxArrow,
    cxIBeam,
    cxPointingHand,
    cxHand,
    cxGrab,
    cxCrosshair,
    cxSizeNWSE,
    cxSizeNESW,
    cxSizeWE,
    cxSizeNS,
    cxSizeAll,
    cxNo,
    
};

struct cxRect
{
    float left, top, right, bottom;
};

struct cxTextOptions
{
    enum TextAlignment
    {
        TEXT_ALIGNMENT_LEFT,
        TEXT_ALIGNMENT_CENTER,
        TEXT_ALIGNMENT_RIGHT
    } m_TextAlignment;
};

class cxWindowBase
{
#ifdef __APPLE__
    void* m_NSWindow;
    void* m_NSView;
    std::map<int, cxSolidBrush> m_pBrushes;
    std::map<int, cxFont> m_pFonts;

#elif _WIN32
    HWND m_hWnd;
    ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
    std::map<int, ComPtr<ID2D1Brush>> m_pBrushes;
    std::map<int, ComPtr<IDWriteTextFormat>> m_pFonts;
#endif

public:
    cxWindowBase();
    ~cxWindowBase();

    void SetTitle(std::wstring title);
    void SetPosition(int x, int y);
    void SetSize(int width, int height);

    void GetTitle(std::wstring& out);
    void GetPosition(int& x, int& y);
    void GetSize(int& width, int& height);
    void GetClientSize(int& width, int& height);

    void Show(bool show = true);

    void ShowCursor(bool show = true);
    void SetCursor(cxCursorType type);
    void CaptureMouse();
    void ReleaseMouse();

    void Invalidate();

    void SetDrawConstraints(cxRect rect);
    void RemoveDrawConstraints();

    void MakeSolidBrush(int key, float r, float g, float b, float a);
    void MakeFont(int key, std::wstring fontName, float size);
    void GetFontTextMetrics(int key, std::wstring str, float maxWidth, float maxHeight, cxTextOptions options, float& width, float& height);

    void FillRectangle(cxRect rect, int brushKey);
    void DrawRectangle(cxRect rect, int brushKey, float strokeWidth = 1.0);
    void FillRoundedRectangle(cxRect rect, float r1, float r2, int brushKey);
    void DrawRoundedRectangle(cxRect rect, float r1, float r2, int brushKey, float strokeWidth = 1.0);

    void DrawText(int fontKey, int brushKey, std::wstring str, cxRect rect, cxTextOptions options);
    
    float GetDPIScale();

    virtual void OnInit() {}
    virtual void OnClosing() {}
    virtual void OnSize(int width, int height) {}
    virtual void OnPaint() {}

    virtual void OnMouseDown(cxMouseEvent event) {}
    virtual void OnMouseUp(cxMouseEvent event) {}
    virtual void OnMouseMove(cxMouseEvent event) {}
    virtual void OnMouseDragged(cxMouseEvent event) {}


#ifdef __APPLE__
    //void* GetPlatformWindow() { return m_NSWindow; }
#elif _WIN32
    HWND GetPlatformWindow() { return m_hWnd; }
#endif

#ifdef _WIN32
    ID2D1HwndRenderTarget* GetWin32RenderTarget() { return m_pRenderTarget.Get(); }
#endif

};
