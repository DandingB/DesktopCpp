#include <cx/cx.h>

#define BRUSH_WHITE 0
#define BRUSH_RED 1
#define BRUSH_GREY 2

#define BRUSH_TBG 3
#define BRUSH_TBGSEL 4
#define BRUSH_TBORDER 5
#define BRUSH_TBORDERSEL 6

cxFont* font;

class cxTextBox : public cxView
{
	using cxView::cxView;

	void OnMouseEnter() override
	{
		cxSetCursor(cxIBeam);
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		SetFocus();
		Invalidate();
	}

	void OnPaint(cxWindowContainer* context) override
	{
		float width, height;
		GetSize(width, height);
		context->FillRectangle({ 0, 0, width, height }, HasFocus() ? BRUSH_TBGSEL : BRUSH_TBG);
		context->DrawRectangle({ 0, 0, width, height }, HasFocus() ? BRUSH_TBORDERSEL : BRUSH_TBORDER, 1.f);
		context->DrawTextInRect(
			font,
			BRUSH_WHITE,
			m_Title,
			{ 10, 0, width, height },
			{ cxTextOptions::TEXT_ALIGNMENT_LEFT, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER }
		);
	}
	void OnFocusLost() override
	{
		Invalidate();
	}
};

class SimpleView : public cxView
{
	using cxView::cxView;

	void OnMouseEnter() override
	{
		cxSetCursor(cxArrow);
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		SetFocus();
		Invalidate();
	}

	void OnPaint(cxWindowContainer* context) override
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
		MakeSolidBrush(BRUSH_GREY, 0.2f, 0.2f, 0.2f, 1.f);

		MakeSolidBrush(BRUSH_TBG, 0.19f, 0.19f, 0.19f, 1.f);
		MakeSolidBrush(BRUSH_TBGSEL, 0.08f, 0.08f, 0.08f, 1.f);
		MakeSolidBrush(BRUSH_TBORDER, 0.31f, 0.31f, 0.31f, 1.f);
		MakeSolidBrush(BRUSH_TBORDERSEL, 0.f, 0.48f, 0.8f, 1.f);

		SimpleView* view = new SimpleView(50.f, 50.f, 100.f, 100.f, this);
		cxTextBox* textbox = new cxTextBox(5.f, 5.f, 160.f, 35.f, view);
		textbox->m_Title = L"Hello";
	}

	void OnSize(int width, int height) override
	{
		cxWindowContainer::OnSize(width, height);

		GetChildView(0)->m_Right = width - 50.f;
		GetChildView(0)->m_Bottom = height - 50.f;
	}

	void OnClosing() override
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