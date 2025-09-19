#include "cx.h"

#define BRUSH_WHITE 0
#define BRUSH_RED   1
#define BRUSH_GREEN 2
#define BRUSH_BLACK 3
#define FONT_SMALL  0


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
			m_TopParent->GetFontTextMetrics(FONT_SMALL, view->m_Title, 200, 30, {cxTextOptions::TEXT_ALIGNMENT_CENTER}, width, height);

			if (event.x > x and event.x < x + width + 10)
			{
				m_SelPage = i;
			}

			view->m_Show = false;
			x += width + 10;
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
			container->GetFontTextMetrics(FONT_SMALL, views->m_Title, 200, 30, {cxTextOptions::TEXT_ALIGNMENT_CENTER}, width, height);

			if (i == m_SelPage) 
				container->FillRectangle({x, 0, x + width + 10, 30}, 0);

			container->DrawText(FONT_SMALL, views->m_Title, { x, 0, x + width + 10, 30 }, BRUSH_BLACK, {cxTextOptions::TEXT_ALIGNMENT_LEFT});
			x += width + 10;
			i++;
		}
	}
};

class MyView : public cxView
{
	using cxView::cxView;

	void OnMouseDown(cxMouseEvent event) override
	{
		cxLog(L"MyView %d %d", event.x, event.y);
	}

	void OnPaint(cxWindowContainer* container) override
	{
		m_TopParent->FillRectangle({ 0,0,m_Right - m_Left,m_Bottom - m_Top }, 1);
		//m_TopParent->DrawRectangle({ 1,1,m_Right - m_Left - 1 ,m_Bottom - m_Top - 1 }, 2, 2.0);
		m_TopParent->DrawText(FONT_SMALL, m_Title, { 0,0,m_Right - m_Left,m_Bottom - m_Top }, BRUSH_BLACK, {cxTextOptions::TEXT_ALIGNMENT_CENTER});
	}
};

class MyWindow : public cxWindowContainer
{
public:
	MyWindow()
	{
		MakeSolidBrush(BRUSH_WHITE, 1.0, 1.0, 1.0, 1.0);
		MakeSolidBrush(BRUSH_RED, 1.0, 0.0, 0.0, 1.0);
		MakeSolidBrush(BRUSH_GREEN, 0.0, 1.0, 0.0, 1.0);
		MakeSolidBrush(BRUSH_BLACK, 0.0, 0.0, 0.0, 1.0);
		MakeFont(FONT_SMALL, L"Segoe UI", 15.0);


		TabControl* tabctrl = new TabControl(0, 0, 150, 100);
		AddView(tabctrl);

		MyView* mv = new MyView(50, 50, 150, 100);
		mv->m_Title = L"MyView";
		tabctrl->AddView(mv);

		MyView* mv2 = new MyView(50, 50, 150, 100);
		mv2->m_Title = L"MyView2";
		tabctrl->AddView(mv2);

		MyView* mv3 = new MyView(50, 50, 150, 100);
		mv3->m_Title = L"MyView3 asdasdasdas";
		tabctrl->AddView(mv3);

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

	MyWindow* window = new MyWindow;

	window->SetTitle(L"Test");
	window->Show();

	cxRunApp();

	return 0;

}
