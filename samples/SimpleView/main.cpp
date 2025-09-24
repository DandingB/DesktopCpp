#include "cx/cx.h"

#define BRUSH_WHITE 0
#define BRUSH_RED 1
#define BRUSH_GREY 2

cxFont* font;

class SimpleView : public cxView
{
	using cxView::cxView;

	void OnPaint(cxWindowContainer* context)
	{
		float width, height;
		GetSize(width, height);

		context->FillRectangle({0, 0, width, height }, BRUSH_GREY);
		context->DrawRectangle({0, 0, width, height }, BRUSH_RED, 5.f);
		context->DrawTextInRect(
			font, 
			BRUSH_WHITE,
			L"Simple View", 
			{ 0, 0, width, height }, 
			{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER }
		);
	}
};

class SimpleWindow : public cxWindowContainer
{
public:
	SimpleWindow()
	{
		MakeSolidBrush(BRUSH_WHITE, 1.f, 1.f, 1.f, 1.f);
		MakeSolidBrush(BRUSH_RED, 1.f, 0.2f, 0.2f, 1.f);
		MakeSolidBrush(BRUSH_GREY, 0.3f, 0.3f, 0.3f, 1.f);

		AddView(new SimpleView(50.f, 50.f, 100.f, 100.f));
	}

	void OnSize(int width, int height) override
	{
		cxWindowContainer::OnSize(width, height);

		GetChildView(0)->m_Right = width - 50.f;
		GetChildView(0)->m_Bottom = height - 50.f;
	}

	void OnClosing()
	{
		cxQuitApp();
	}
};

CX_FUNC_MAIN
{
	cxInitApp();

	font = new cxFont(CX_SYSTEM_FONT, 15.f);

	SimpleWindow* window = new SimpleWindow;
	window->SetTitle(L"Simple View");
	window->Show();

	cxRunApp();
}