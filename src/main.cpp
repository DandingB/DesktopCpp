#include "cx.h"

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
		
	}

	void OnPaint() override
	{
		int width, height;
		GetClientSize(width, height);

		glViewport(0, 0, width, height);
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

	void OnMouseDown(cxMouseEvent event) override
	{
		//OutputDebugStringW(L"Mouse Down\n");
		//cxMessageBox(L"Hej med dig");
		//SetPosition(100, 500);
		//CaptureMouse();

		cxLog(L"asdasd %lf \n", GetDPIScale());
		Invalidate();

		// int x1, y1;
		// GetClientSize(x1, y1);
		// std::wstring test = std::to_wstring(x1) + L", " + std::to_wstring(y1) + L"\n";
		// OutputDebugStringW(test.c_str());
	}


	void OnMouseUp(cxMouseEvent event) override
	{
		//cxLog(L"asdasd %d", event.button);
		//OutputDebugStringW(L"Mouse Up\n");
	}

	void OnMouseMove(cxMouseEvent event) override
	{

	}

};

CX_FUNC_MAIN
{
	cxInitApp();

	MyWindow* window = new MyWindow();
	window->SetTitle(L"Test");
	window->Show();


	cxRunApp();

	return 0;
}