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

	int m_Cursor = 0; // The cursor/selection start point
	int m_Select = 0; // The selection end point

	void OnMouseEnter() override
	{
		cxSetCursor(cxIBeam);
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		SetFocus();
		ShowCaret(true);
		Invalidate();
	}

	void OnKeyDown(cxKeyEvent event) override
	{
		if (event.m_Key == cxKeyEvent::ARROW_LEFT)
		{
			if (m_Cursor != 0)
			{
				m_Cursor--;
				m_Select = m_Cursor;
			}
		}

		if (event.m_Key == cxKeyEvent::ARROW_RIGHT)
		{
			if (m_Cursor < m_Title.size())
			{
				m_Cursor++;
				m_Select = m_Cursor;
			}
		}

		if (event.m_Key == cxKeyEvent::BACKSPACE)
		{
			if (m_Cursor != 0 and m_Cursor == m_Select)
			{
				m_Title.erase(m_Title.begin() + m_Cursor - 1);
				m_Cursor--;
				m_Select = m_Cursor;
			}
		}
		
		Invalidate();
	}

	void OnCharacter(cxKeyEvent event) override
	{
		if (event.m_Char != '\b')
		{
			m_Title.insert(m_Title.begin() + m_Cursor, event.m_Char);
			m_Cursor++;
			m_Select++;
		}
		Invalidate();
	}

	void OnPaint(cxWindowContainer* context) override
	{
		float width, height;
		GetSize(width, height);
		context->FillRoundedRectangle({ 0, 0, width, height }, 5.f, 5.f, HasFocus() ? BRUSH_TBGSEL : BRUSH_TBG);

		font->SetTextOptions({ cxTextOptions::TEXT_ALIGNMENT_LEFT, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER, cxTextOptions::WORD_WRAPPING_WORD });
		context->DrawTextInRect(
			font,
			BRUSH_WHITE,
			m_Title,
			{ 8.f, 0, width-8.f, height }
		);


		//cxPoint pt;
		//font->GetCharPosition(m_Title, m_Cursor, width, height, { cxTextOptions::TEXT_ALIGNMENT_LEFT, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER }, pt);
		//SetCaretPos({ 8.f + pt.x, height * 0.5f - 10.f });

		context->DrawRoundedRectangle({ 0, 0, width, height }, 5.f, 5.f, HasFocus() ? BRUSH_TBORDERSEL : BRUSH_TBORDER, 1.f);
		
	}
	void OnFocusLost() override
	{
		ShowCaret(false);
		Invalidate();
	}
};

class cxButton : public cxView
{
	using cxView::cxView;

public:
	bool m_Highlight = false;
	std::function<void()> callback;

	void OnMouseDown(cxMouseEvent event) override
	{
		SetFocus();
		m_Highlight = true;
		Invalidate();
	}

	void OnMouseUp(cxMouseEvent event) override
	{
		m_Highlight = false;
		Invalidate();
		if (callback) callback();
	}

	void OnMouseEnter() override
	{
		Invalidate();
	}

	void OnMouseLeave() override
	{
		m_Highlight = false;
		Invalidate();
	}

	void OnPaint(cxWindowContainer* container) override
	{
		float width, height;
		GetSize(width, height);

		container->FillRoundedRectangle({ 0, 0, width, height }, 5.f, 5.f, m_Highlight ? BRUSH_TBGSEL : BRUSH_GREY);

		font->SetTextOptions({ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER, cxTextOptions::WORD_WRAPPING_WORD });
		container->DrawTextInRect(
			font,
			BRUSH_WHITE,
			m_Title,
			{ 8.f, 0, width - 8.f, height }
		);

		container->DrawRoundedRectangle({ 0, 0, width, height }, 5.f, 5.f, HasFocus() ? BRUSH_TBORDERSEL : BRUSH_TBORDER, 1.f);
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

		font->SetTextOptions({ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER, cxTextOptions::WORD_WRAPPING_WORD });

		context->DrawTextInRect(
			font, 
			BRUSH_WHITE,
			L"Simple View", 
			{ 0, 0, width, height }
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

		MakeSolidBrush(BRUSH_TBG, 0.15f, 0.15f, 0.15f, 1.f);
		MakeSolidBrush(BRUSH_TBGSEL, 0.08f, 0.08f, 0.08f, 1.f);
		MakeSolidBrush(BRUSH_TBORDER, 0.31f, 0.31f, 0.31f, 1.f);
		MakeSolidBrush(BRUSH_TBORDERSEL, 0.f, 0.48f, 0.8f, 1.f);

		SimpleView* view = new SimpleView(50.f, 50.f, 100.f, 100.f, this);
		cxTextBox* textbox = new cxTextBox(10.f, 10.f, 160.f, 40.f, view);
		textbox->m_Title = L"Hello textbox, this is a string";

		cxButton* button = new cxButton(10.f, 50.f, 120.f, 75.f, view);
		button->m_Title = L"Button";
	}

	void OnSize(float width, float height) override
	{
		GetChildView(0)->m_Right = (float)width - 50.f;
		GetChildView(0)->m_Bottom = (float)height - 50.f;

		cxLog(L"width: %d, height: %d DPI: %f\n", width, height, GetDPIScale());
	}

	void OnPaint() override
	{
		float width, height;
		GetClientSize(width, height);

		FillRectangle({ 0, 0, width, height }, BRUSH_GREY);
		cxWindowContainer::OnPaint();
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

	cxLog(L"%f", window->GetDPIScale());

	cxRunApp();
}