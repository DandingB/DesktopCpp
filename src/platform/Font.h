#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../cx/Types.h"

class cxFont
{
private:
    // PIMPL, platform types
    struct Impl;
    std::unique_ptr<Impl> p;

public:
    cxFont(std::wstring fontName, float size);
    ~cxFont();

    void GetFontTextMetrics(std::wstring str, float maxWidth, float maxHeight, cxTextOptions options, float& width, float& height);

    friend class cxWindowBase;
};