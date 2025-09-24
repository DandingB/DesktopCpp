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
#include "Audio.h"
#include <functional>
#include <fstream>

std::unique_ptr<cxFont> font;

class TabControl : public cxView
{
	int m_SelPage = 0;
	int m_PageHover = -1;
	bool m_HoverClose = false;

	float m_TabPadding = 50.f;
	float m_TabHeight = 40.f;

	using cxView::cxView;

	void GetTabIndex(float xPos, float yPos, int& index, bool& isClose)
	{
		index = -1;
		isClose = false;

		float tabMiddle = m_TabHeight * 0.5f;
		float x = 0;
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

			if (xPos > x and xPos < x + width + m_TabPadding)
			{
				index = i;
				if (xPos > x + width + m_TabPadding - 20.f and xPos < x + width + m_TabPadding - 10.f and yPos > tabMiddle - 5.f and yPos < tabMiddle + 5.f)
					isClose = true;
			}
			x += width + m_TabPadding;
		}
	}

	void OnSize() override
	{
		for (cxView* view : m_SubViews)
		{
			view->m_Left = 0;
			view->m_Top = m_TabHeight;
			view->m_Right = m_Right;
			view->m_Bottom = m_Bottom;
			view->OnSize();
		}
	}


	void OnMouseMove(cxMouseEvent event) override
	{
		int iTab;
		bool isClose;
		GetTabIndex(event.x, event.y, iTab, isClose);
		m_PageHover = iTab;
		m_HoverClose = isClose;
		m_TopParent->Invalidate();
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		int iTab;
		bool isClose;
		GetTabIndex(event.x, event.y, iTab, isClose);
		if (iTab != -1)
		{
			m_SelPage = iTab;

			if (isClose)
			{
				delete m_SubViews[iTab];
				m_SubViews.erase(m_SubViews.begin() + iTab);
				if (m_SelPage >= m_SubViews.size())
					m_SelPage--;
			}


			for (cxView* view : m_SubViews)
				view->m_Show = false;
			if (m_SubViews.size() > 0)
				m_SubViews[m_SelPage]->m_Show = true;


		}
		m_TopParent->Invalidate();
	}

	void OnMouseEnter() override
	{
		cxSetCursor(cxCursorType::cxArrow);
	}

	void OnMouseLeave() override
	{
		m_HoverClose = false;
		m_PageHover = -1;
		m_TopParent->Invalidate();
	}

	void OnPaint(cxWindowContainer* container) override
	{
		container->FillRectangle({ 0,0,m_Right - m_Left,m_Bottom - m_Top }, BRUSH_DARKERGREY);

		int i = 0;
		float x = 0;
		for (cxView* views : m_SubViews)
		{
			float width, height;
			font->GetFontTextMetrics(
				views->m_Title,
				200,
				m_TabHeight,
				{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER },
				width,
				height
			);

			if (i == m_PageHover)
				container->FillRectangle({ x, 0, x + width + m_TabPadding, m_TabHeight }, BRUSH_TABHOVERGREY);

			if (i == m_SelPage)
				container->FillRectangle({ x, 0, x + width + m_TabPadding, m_TabHeight }, BRUSH_TABGREY);

			container->DrawTextInRect(
				font.get(),
				i == m_SelPage ? BRUSH_TEXTWHITE : BRUSH_TEXTGREY,
				views->m_Title,
				{ x, 0, x + width + m_TabPadding - 20.f, m_TabHeight },
				{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER }
			);

			float tabMiddle = m_TabHeight * 0.5f;
			float closePadding = 10.f;
			container->DrawLine(
				{
					x + width + m_TabPadding - closePadding,
					tabMiddle + 5.f
				},
				{
					x + width + m_TabPadding - 10.f - closePadding,
					tabMiddle - 5.f
				},
				i == m_PageHover and m_HoverClose ? BRUSH_TEXTWHITE : BRUSH_TEXTGREY,
				2.f
			);

			container->DrawLine(
				{
					x + width + m_TabPadding - 10.f - closePadding,
					tabMiddle + 5.f
				},
				{
					x + width + m_TabPadding - closePadding,
					tabMiddle - 5.f
				},
				i == m_PageHover and m_HoverClose ? BRUSH_TEXTWHITE : BRUSH_TEXTGREY,
				2.f
			);

			x += width + m_TabPadding;
			i++;
		}
	}

public:
	void SetSelectedPage(int page)
	{
		if (page >= 0 and page < m_SubViews.size())
		{
			m_SelPage = page;

			for (cxView* view : m_SubViews)
				view->m_Show = false;
			if (m_SubViews.size() > 0)
				m_SubViews[m_SelPage]->m_Show = true;

			m_TopParent->Invalidate();
		}
	}
};


class WaveformView : public cxView
{
	using cxView::cxView;
public:

	float m_hScroll = 0.0f;

	AudioPeakData data;

	void OnMouseEnter() override
	{
		cxSetCursor(cxCursorType::cxArrow);
	}

	void OnMouseDown(cxMouseEvent event) override
	{
	}

	void OnMouseScroll(cxMouseScrollEvent event) override
	{
		m_hScroll -= event.scrollY;
		if (m_hScroll < 0.f) m_hScroll = 0.f;
		m_TopParent->Invalidate();
	}

	void OnPaint(cxWindowContainer* container) override
	{
		container->FillRectangle({ 0,0,m_Right - m_Left,m_Bottom - m_Top }, BRUSH_DARKGREY);

		float center = m_Bottom / 2.f;
		for (uint32_t i = 0; i < data.m_Peaks.size(); i++)
		{
			AudioPeakData::Peak peak = data.m_Peaks[i];

			container->FillRectangle({(float)i - m_hScroll, center - peak.high * 80.f, (float)i+1 - m_hScroll, center - peak.low * 80.f }, BRUSH_TEXTWHITE);
		}
	}
};


class MyWindow : public cxWindowContainer
{
public:
	TabControl* tabctrl;

	MyWindow()
	{
		MakeSolidBrush(BRUSH_TEXTWHITE, 1.0f, 1.0f, 1.0f, 1.0f);
		MakeSolidBrush(BRUSH_TEXTGREY, 0.6f, 0.6f, 0.6f, 1.0f);
		MakeSolidBrush(BRUSH_DARKGREY, 0.2f, 0.2f, 0.2f, 1.0f);
		MakeSolidBrush(BRUSH_DARKERGREY, 0.1f, 0.1f, 0.1f, 1.0f);
		MakeSolidBrush(BRUSH_TABGREY, 0.3f, 0.3f, 0.3f, 1.0f);
		MakeSolidBrush(BRUSH_TABHOVERGREY, 0.2f, 0.2f, 0.2f, 1.0f);
		MakeSolidBrush(BRUSH_BUTTON, 0.3f, 0.3f, 0.3f, 1.0f);
		MakeSolidBrush(BRUSH_BUTTONHIGHLIGHT, 0.4f, 0.4f, 0.4f, 1.0f);

		tabctrl = new TabControl(0, 0, 150, 100);
		AddView(tabctrl);

		GetChildView(0)->m_Right = 500;
		GetChildView(0)->m_Bottom = 500;
		GetChildView(0)->OnSize();
	}

	void OpenFile()
	{
		std::wstring filename;
		cxOpenFileDialog(filename);

		if (filename != L"")
		{
			std::wstring base_filename = filename.substr(filename.find_last_of(L"/\\") + 1);

			WaveformView* wfView = new WaveformView(50, 50, 150, 100);
			wfView->m_Title = base_filename;
			wfView->m_Show = false;

			LoadWaveFile(filename, wfView->data);

			tabctrl->AddView(wfView);
			tabctrl->SetSelectedPage(0);
			GetChildView(0)->OnSize();

			Invalidate();
		}
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
	}

	void OnPaint() override
	{
		cxWindowContainer::OnPaint();
	}

};

MyWindow* window;

void MenuCommand(int command)
{
	switch (command)
	{
	case 1:
		window->OpenFile();
		break;
	default:
		break;
	}
}

CX_FUNC_MAIN
{
	cxInitApp();
	//cxRegisterFontFile(L"pt-root-ui_vf.ttf");

	font = std::make_unique<cxFont>(CX_SYSTEM_FONT, 15.f);

	window = new MyWindow;

#ifdef __APPLE__
	std::vector<cxMenuItem> appMenu;
	appMenu.push_back({ L"Thingy", 4 });
	appMenu.push_back({ L"Lol", 5 });
#endif

	std::vector<cxMenuItem> fileMenu;
	fileMenu.push_back({ L"Open", 1 });


	std::vector<cxMenuItem> menu;
#ifdef __APPLE__
	menu.push_back({L"", 0, appMenu });
#endif
	menu.push_back({L"File", 0, fileMenu });



	window->SetMenu(menu);
	window->SetMenuCallback(MenuCommand);
	window->SetTitle(L"Waveform Viewer");
	window->Show();

	cxRunApp();

	return 0;

}
