#pragma once
#include <string>
#include "Event.h"

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

class cxWindowBase
{
    void* m_Window;

    void CreateContext();

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

    void SetContext();

    void* GetPlatformWindow() { return m_Window; }
    float GetDPIScale();

    virtual void OnInit() {}
    virtual void OnClosing() {}
    virtual void OnSize(int width, int height) {}
    virtual void OnPaint() {}

    virtual void OnMouseDown(cxMouseEvent event) {}
    virtual void OnMouseUp(cxMouseEvent event) {}
    virtual void OnMouseMove(cxMouseEvent event) {}
    virtual void OnMouseDragged(cxMouseEvent event) {}

};
