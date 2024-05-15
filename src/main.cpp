#include "cx.h"

void FillRect(int x, int y, int width, int height, float r, float g, float b, float a)
{
	//float x1 = x * 2 - 1;
	//float y1 = y * 2 - 1;
	//float width1 = width * 2;
	//float height1 = height * 2;

	//glBegin(GL_QUADS);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex2f(x1,			y1);
	//glVertex2f(x1 + width1, y1);
	//glVertex2f(x1 + width1, y1 + height1);
	//glVertex2f(x1,			y1 + height1);
	//glEnd();

	glBegin(GL_QUADS);
	glColor4f(r, g, b, a);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();


}

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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, 1);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);



		FillRect(10, 10, 200, 100, 1.0, 0.0, 0.0, 1.0);
		FillRect(50, 50, 200, 100, 1.0, 1.0, 0.0, 0.5);

		//glBegin(GL_TRIANGLES);
		//glColor3f(1.0f, 0.0f, 0.0f);
		//glVertex2f(10, 10);
		//glColor3f(0.0f, 1.0f, 1.0f);
		//glVertex2f(100, 100);
		//glColor3f(0.0f, 1.0f, 1.0f);
		//glVertex2f(10, 100);
		//glEnd();

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