#include "cx.h"


class MyWindow : public cxWindowContainer
{
public:
	MyWindow()
	{
	}

	void OnClosing() override
	{
		cxQuitApp();
	}

	void OnSize(int width, int height) override
	{
		cxWindowContainer::OnSize(width, height);
		Invalidate();
	}

	void OnPaint() override
	{
		StartPaint();

		EndPaint();
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