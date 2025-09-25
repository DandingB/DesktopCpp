#pragma once
#include <string>
#include <vector>



struct cxMouseEvent
{
	float x, y;

    enum MouseButton
    {
        LEFT,
        RIGHT,
        MIDDLE,
        NONE
    } button;
};

struct cxMouseScrollEvent
{
    float x, y;
	float scrollX, scrollY;
};

struct cxKeyEvent
{
    enum Key
    {
        ARROW_UP,
        ARROW_DOWN,
        ARROW_LEFT,
        ARROW_RIGHT,
        SPACE,
        ENTER,
        BACKSPACE,
        TAB,
    } m_Key;

    wchar_t m_Char;
};

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

struct cxTextOptions
{
    enum TextAlignment
    {
        TEXT_ALIGNMENT_LEFT,
        TEXT_ALIGNMENT_CENTER,
        TEXT_ALIGNMENT_RIGHT
    } m_TextAlignment;

    enum ParagraphAlignment
    {
        PARAGRAPH_ALIGNMENT_TOP,
        PARAGRAPH_ALIGNMENT_CENTER,
        PARAGRAPH_ALIGNMENT_BOTTOM
    } m_ParagraphAlignment;

    enum WordWrapping
    {
        WORD_WRAPPING_NONE,
        WORD_WRAPPING_WORD,
        WORD_WRAPPING_CHARACTER
    } m_WordWrapping;
};

struct cxMenuItem
{
    std::wstring title;
    int command;

    std::vector<cxMenuItem> subitems;
};

struct cxRect
{
    float left, top, right, bottom;
};

struct cxPoint
{
    float x, y;
};