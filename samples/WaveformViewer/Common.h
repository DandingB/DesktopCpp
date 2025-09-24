#pragma once

#define BRUSH_TEXTWHITE 0
#define BRUSH_TEXTGREY  1
#define BRUSH_DARKGREY   2
#define BRUSH_DARKERGREY 3
#define BRUSH_TABGREY 4
#define BRUSH_TABHOVERGREY 5
#define BRUSH_BUTTON 6
#define BRUSH_BUTTONHIGHLIGHT 7

#include "../../src/cx/cx.h"
#include <functional>
#include <fstream>

std::unique_ptr<cxFont> font;