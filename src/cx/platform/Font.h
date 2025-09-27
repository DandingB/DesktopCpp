#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../Types.h"

class cxFont
{
private:
    // PIMPL, platform types
    struct Impl;
    std::unique_ptr<Impl> p;

public:
    cxFont(std::wstring fontName, float size);
    ~cxFont();

    void GetStringMetrics(std::wstring str, float maxWidth, float maxHeight, cxTextOptions options, float& width, float& height);
    void GetCharPosition(std::wstring str, int iChar, float maxWidth, float maxHeight, cxTextOptions options, cxPoint& out);

    friend class cxWindowBase;
};