#include "Audio.h"
#include "TabControl.h"

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

			container->FillRectangle({(float)i - m_hScroll, center - peak.high * 50.f, (float)i+1 - m_hScroll, center - peak.low * 50.f }, BRUSH_TEXTWHITE);
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
	window->SetTitle(L"Test");
	window->Show();

	cxRunApp();

	return 0;

}
