#pragma once
#include "../Types.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>


class cxFont;

class cxWindowBase
{
private:
    // PIMPL, platform types
    struct Impl;
    std::unique_ptr<Impl> p;

    std::function<void(int)> m_MenuCallback;

public:
    cxWindowBase();
    ~cxWindowBase();

    void Show(bool show = true);
    void SetTitle(std::wstring title);
    void SetPosition(int x, int y);
    void SetSize(int width, int height);

    void GetTitle(std::wstring& out);
    void GetPosition(int& x, int& y);
    void GetSize(int& width, int& height);
    void GetClientSize(float& width, float& height);

    void SetMenu(std::vector<cxMenuItem>& menu);
    void SetMenuCallback(std::function<void(int)> callback);
    void GetMenuCallback(std::function<void(int)>& callback);

    void CaptureMouse();
    void ReleaseMouse();

    void Invalidate();
    void SetDrawConstraints(cxRect rect);
    void RemoveDrawConstraints();
    void MakeSolidBrush(int key, float r, float g, float b, float a);
    void FillRectangle(cxRect rect, int brushKey);
    void DrawRectangle(cxRect rect, int brushKey, float strokeWidth = 1.0);
    void FillRoundedRectangle(cxRect rect, float r1, float r2, int brushKey);
    void DrawRoundedRectangle(cxRect rect, float r1, float r2, int brushKey, float strokeWidth = 1.0);
    void DrawLine(cxPoint p1, cxPoint p2, int brushKey, float strokeWidth = 1.0);
    void DrawTextInRect(cxFont* font, int brushKey, std::wstring str, cxRect rect, cxTextOptions options);
    
    float GetDPIScale();

    virtual void OnInit() {}
    virtual void OnClosing() {}
    virtual void OnSize(int width, int height);
    virtual void OnPaint() {}

    virtual void OnMouseDown(cxMouseEvent event) {}
    virtual void OnMouseUp(cxMouseEvent event) {}
    virtual void OnMouseMove(cxMouseEvent event) {}
    virtual void OnMouseDragged(cxMouseEvent event) {}
    virtual void OnMouseLeave() {}
    virtual void OnMouseScroll(cxMouseScrollEvent event) {} 

    virtual void OnFocusLost() {}


#ifdef _WIN32
    void StartPaint();
    void EndPaint();
#endif

};
