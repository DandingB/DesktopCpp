#pragma once
#include <string>


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
    void Invalidate();
    void CaptureMouse();
    void ReleaseMouse();

    void* GetPlatformWindow() { return m_Window; }

    virtual void OnInit() {}
    virtual void OnClosing() {}
    virtual void OnSize(int width, int height) {}
    virtual void OnPaint() {}

    virtual void OnMouseDown(int x, int y) {}
    virtual void OnMouseUp(int x, int y) {}
    virtual void OnMouseMove(int x, int y) {}
    virtual void OnMouseDragged(int x, int y) {}

};
