#pragma once
#include "../cx/Types.h"
#include "../cx/Event.h"
#include <memory>
#include <vector>
#include <string>


class cxWindowBase
{
    // PIMPL, platform types
    struct Impl;
    std::unique_ptr<Impl> p;

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
    void GetClientSize(float& width, float& height);

    void Show(bool show = true);

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

    void DrawTextInRect(int fontKey, int brushKey, std::wstring str, cxRect rect, cxTextOptions options);
    
    float GetDPIScale();

    virtual void OnInit() {}
    virtual void OnClosing() {}
    virtual void OnSize(int width, int height);
    virtual void OnPaint() {}

    virtual void OnMouseDown(cxMouseEvent event) {}
    virtual void OnMouseUp(cxMouseEvent event) {}
    virtual void OnMouseMove(cxMouseEvent event) {}
    virtual void OnMouseDragged(cxMouseEvent event) {}


#ifdef _WIN32
    void StartPaint();
    void EndPaint();
#endif

};
