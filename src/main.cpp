#include "cx.h"
#include <functional>
#include <fstream>

#define BRUSH_TEXTWHITE 0
#define BRUSH_TEXTGREY  4
#define BRUSH_DARKGREY   1
#define BRUSH_DARKERGREY 2
#define BRUSH_TABGREY 3
#define BRUSH_BUTTON 5
#define BRUSH_BUTTONHIGHLIGHT 6

std::unique_ptr<cxFont> font;

class TabControl : public cxView
{
	int m_SelPage = 0;

	using cxView::cxView;

	void OnSize() override
	{
		for (cxView* view : m_SubViews)
		{
			view->m_Left = 0;
			view->m_Top = 30;
			view->m_Right = m_Right;
			view->m_Bottom = m_Bottom;
			view->OnSize();
		}
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		int x = 0;
		for (int i = 0; i < m_SubViews.size(); i++)
		{
			cxView* view = m_SubViews[i];

			float width, height;
			font->GetFontTextMetrics(
				view->m_Title, 
				200, 
				30, 
				{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER },
				width, 
				height
			);

			if (event.x > x and event.x < x + width + 20)
			{
				m_SelPage = i;
			}

			view->m_Show = false;
			x += width + 20.f;
		}

		if (m_SubViews.size() > 0)
			m_SubViews[m_SelPage]->m_Show = true;
		m_TopParent->Invalidate();
	}

	void OnPaint(cxWindowContainer* container) override
	{
		container->FillRectangle({ 0,0,m_Right - m_Left,m_Bottom - m_Top }, 2);

		int i = 0;
		float x = 0;
		for (cxView* views : m_SubViews)
		{
			float width, height;
			font->GetFontTextMetrics( 
				views->m_Title, 
				200, 
				30, 
				{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER },
				width, 
				height
			);

			if (i == m_SelPage)
				container->FillRectangle({x, 0, x + width + 20, 30}, BRUSH_TABGREY);

			container->DrawTextInRect(
				font.get(),
				i == m_SelPage ? BRUSH_TEXTWHITE : BRUSH_TEXTGREY,
				views->m_Title, 
				{ x, 0, x + width + 20, 30 }, 
				{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER }
			);

			x += width + 20;
			i++;
		}
	}
	

};

class MyView : public cxView
{
	using cxView::cxView;
public:
	std::wstring text;

	void OnMouseEnter() override
	{
		cxSetCursor(cxCursorType::cxArrow);
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		cxLog(L"MyView %d %d", event.x, event.y);
	}

	void OnPaint(cxWindowContainer* container) override
	{
		m_TopParent->FillRectangle({ 0,0,m_Right - m_Left,m_Bottom - m_Top }, 1);
		//m_TopParent->DrawRectangle({ 1,1,m_Right - m_Left - 1 ,m_Bottom - m_Top - 1 }, 2, 2.0);
		m_TopParent->DrawTextInRect(
			font.get(),
			BRUSH_TEXTWHITE,
			text,
			{ 0,0,m_Right - m_Left,m_Bottom - m_Top }, 
			{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER }
		);
	}
};

class MyButton : public cxView
{
	using cxView::cxView;

public:
	bool m_Highlight = false;
	std::function<void()> callback;

	void OnMouseDown(cxMouseEvent event) override
	{
	}

	void OnMouseUp(cxMouseEvent event) override
	{
		callback();
	}

	void OnMouseEnter() override
	{
		m_Highlight = true;
		m_TopParent->Invalidate();
	}

	void OnMouseLeave() override
	{
		m_Highlight = false;
		m_TopParent->Invalidate();
	}

	void OnPaint(cxWindowContainer* container) override
	{
		container->FillRectangle({ 0,0,m_Right - m_Left,m_Bottom - m_Top }, m_Highlight ? BRUSH_BUTTONHIGHLIGHT : BRUSH_BUTTON);

		container->DrawTextInRect(
			font.get(),
			BRUSH_TEXTWHITE,
			m_Title,
			{ 0,0,m_Right - m_Left,m_Bottom - m_Top },
			{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER }
		);

	}
};

class MyWindow : public cxWindowContainer
{
	TabControl* tabctrl;

	MyView* mv;
	MyView* mv2;
	MyView* mv3;
public:
	MyWindow()
	{
		MakeSolidBrush(BRUSH_TEXTWHITE, 1.0f, 1.0f, 1.0f, 1.0f);
		MakeSolidBrush(BRUSH_TEXTGREY, 0.6f, 0.6f, 0.6f, 1.0f);
		MakeSolidBrush(BRUSH_DARKGREY, 0.2f, 0.2f, 0.2f, 1.0f);
		MakeSolidBrush(BRUSH_DARKERGREY, 0.1f, 0.1f, 0.1f, 1.0f);
		MakeSolidBrush(BRUSH_TABGREY, 0.3f, 0.3f, 0.3f, 1.0f);
		MakeSolidBrush(BRUSH_BUTTON, 0.3f, 0.3f, 0.3f, 1.0f);
		MakeSolidBrush(BRUSH_BUTTONHIGHLIGHT, 0.4f, 0.4f, 0.4f, 1.0f);

		tabctrl = new TabControl(0, 0, 150, 100);
		AddView(tabctrl);

		mv = new MyView(50, 50, 150, 100);
		mv->m_Title = L"File name";
		tabctrl->AddView(mv);
		MyButton* btn = new MyButton(10, 10, 100, 40);
		btn->callback = std::bind(&MyWindow::OpenFile, this);
		btn->m_Title = L"Open File";
		mv->AddView(btn);


		mv2 = new MyView(50, 50, 150, 100);
		mv2->m_Title = L"File contents";
		mv2->m_Show = false;
		tabctrl->AddView(mv2);

		mv3 = new MyView(50, 50, 150, 100);
		mv3->m_Title = L"MyView3asdasdasdas";
		mv3->m_Show = false;
		tabctrl->AddView(mv3);

		GetChildView(0)->m_Right = 500;
		GetChildView(0)->m_Bottom = 500;
		GetChildView(0)->OnSize();
	}

	void OpenFile()
	{
		std::wstring filename;
		cxOpenFileDialog(filename);
		mv->text = filename;

		std::wifstream file(filename);
		std::wstring contents;
		std::wstring str;
		while (std::getline(file, str))
		{
			contents += str;
		}

		cxLog(contents);

		mv2->text = contents;

		Invalidate();
	}

	void OnClosing() override
	{
		cxQuitApp();
	}

	void OnSize(int width, int height) override
	{
		cxWindowContainer::OnSize(width, height);
		GetChildView(0)->m_Right = width;
		GetChildView(0)->m_Bottom = height;
		GetChildView(0)->OnSize();
		//Invalidate();
	}

	void OnPaint() override
	{
		cxWindowContainer::OnPaint();
	}

};


CX_FUNC_MAIN
{
	cxInitApp();
	//cxRegisterFontFile(L"pt-root-ui_vf.ttf");

	font = std::make_unique<cxFont>(CX_SYSTEM_FONT, 15.f);

	MyWindow* window = new MyWindow;

	window->SetTitle(L"Test");
	window->Show();

	cxRunApp();

	return 0;

}
