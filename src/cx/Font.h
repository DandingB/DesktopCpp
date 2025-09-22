#pragma once
#include <memory>
#include <vector>
#include <string>

class cxFont
{
private:
    // PIMPL, platform types
    struct Impl;
    std::unique_ptr<Impl> p;

public:
    cxFont();
    ~cxFont();
};