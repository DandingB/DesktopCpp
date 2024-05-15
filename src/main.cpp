#include "cx.h"

void cxFillRect(int x, int y, int width, int height, float r, float g, float b, float a)
{
	glBegin(GL_QUADS);
	glColor4f(r, g, b, a);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

class MyWindow : public cxWindowContainer
{
	bool m_Dragging = false;

	int m_X = 50;
	int m_Y = 50;

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

	//void OnPaint() override
	//{
	//	int width, height;
	//	GetClientSize(width, height);

	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	glMatrixMode(GL_PROJECTION);
	//	glLoadIdentity();
	//	glOrtho(0, width, height, 0, -1, 1);
	//	glViewport(0, 0, width, height);
	//	glClear(GL_COLOR_BUFFER_BIT);


	//	cxFillRect(10, 10, 200, 100, 1.0, 0.0, 0.0, 1.0);
	//	cxFillRect(m_X, m_Y, 200, 100, 1.0, 1.0, 0.0, 0.5);


	//	//glBegin(GL_TRIANGLES);
	//	//glColor3f(1.0f, 0.0f, 0.0f);
	//	//glVertex2f(10, 10);
	//	//glColor3f(0.0f, 1.0f, 1.0f);
	//	//glVertex2f(100, 100);
	//	//glColor3f(0.0f, 1.0f, 1.0f);
	//	//glVertex2f(10, 100);
	//	//glEnd();

	//	glFlush();
	//}

	void OnMouseDown(cxMouseEvent event) override
	{
		if (event.button == LEFT)
		{
			m_Dragging = true;
			CaptureMouse();
		}
	}


	void OnMouseUp(cxMouseEvent event) override
	{
		m_Dragging = false;
		ReleaseMouse();
	}

	void OnMouseMove(cxMouseEvent event) override
	{
		if (m_Dragging)
		{
			cxView* view = GetChildView(0);
			view->m_X = event.x;
			view->m_Y = event.y;

			Invalidate();
		}
	}

};


class MyControl1: public cxView
{
public:

	MyControl1()
	{
		m_X = 10;
		m_Y = 20;
		m_Width = 100;
		m_Height = 120;
	}

	void OnPaint() override
	{
		cxFillRect(0, 0, m_Width, m_Height, 0.2, 0.2, 0.2, 1.0);
		cxFillRect(20, 20, 20, 20, 0.0, 1.0, 0.0, 1.0);
	}

};


class MyControl2 : public cxView
{
public:

	MyControl2()
	{
		m_X = 200;
		m_Y = 20;
		m_Width = 100;
		m_Height = 120;
	}

	void OnPaint() override
	{
		cxFillRect(0, 0, m_Width, m_Height, 0.2, 0.2, 0.2, 1.0);
		cxFillRect(20, 50, m_Width - 40, 20, 1.0, 1.0, 0.0, 1.0);
	}

};


CX_FUNC_MAIN
{
	cxInitApp();


	MyControl1* control1 = new MyControl1;
	MyControl2* control2 = new MyControl2;

	MyWindow* window = new MyWindow;
	window->SetTitle(L"Test");

	window->AddView(control1);
	window->AddView(control2);

	window->Show();


	cxRunApp();

	return 0;
}