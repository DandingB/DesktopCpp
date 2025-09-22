#pragma once

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

    enum ParagraphAlignment
    {
        PARAGRAPH_ALIGNMENT_TOP,
        PARAGRAPH_ALIGNMENT_CENTER,
        PARAGRAPH_ALIGNMENT_BOTTOM
    } m_ParagraphAlignment;
};
