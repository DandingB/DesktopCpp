#include "cx.h"
#include <iostream>

class MyWindow : public cxWindowBase
{
public:
	MyWindow()
	{
		// cxMessageBox(L"Hej med dig");
	}

	void OnInit() override
	{
	}

	void OnClosing() override
	{
		cxQuitApp();
	}

	void OnSize(int width, int height) override
	{
		glViewport(0, 0, width, height);
	}

	void OnPaint() override
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//glBegin(GL_TRIANGLES);
		//glColor3f(1.0f, 0.0f, 0.0f);
		//glVertex2i(0, 1);
		//glColor3f(0.0f, 1.0f, 1.0f);
		//glVertex2i(-1, -1);
		//glColor3f(0.0f, 1.0f, 1.0f);
		//glVertex2i(1, -1);
		//glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.4, 0.4);
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex2f(-1, -1);
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex2f(1, -1);
		glEnd();

		//drawRectWithRoundedCorners(0, 0, 10, 5, 10, 0x00204080);

		glFlush();
	}

	void OnMouseDown(int x, int y) override
	{
		//OutputDebugStringW(L"Mouse Down\n");
		//cxMessageBox(L"Hej med dig");
		//SetPosition(100, 500);
		//CaptureMouse();
		int x1, y1;
		GetClientSize(x1, y1);

		cxLog(L"asdasd %d %d", x1, y1);

		// int x1, y1;
		// GetClientSize(x1, y1);
		// std::wstring test = std::to_wstring(x1) + L", " + std::to_wstring(y1) + L"\n";
		// OutputDebugStringW(test.c_str());
	}


	void OnMouseUp(int x, int y) override
	{
		//OutputDebugStringW(L"Mouse Up\n");
	}

	void OnMouseMove(int x, int y) override
	{

	}

};

CX_FUNC_MAIN
{
	MyWindow * window = new MyWindow();
	window->SetTitle(L"Test");
	window->Show();


	cxRunApp();

	return 0;
}