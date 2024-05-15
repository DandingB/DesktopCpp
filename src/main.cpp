#include "cx.h"

void FillRect(int x, int y, int width, int height, float r, float g, float b, float a)
{
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
		FillRect(m_X, m_Y, 200, 100, 1.0, 1.0, 0.0, 0.5);


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
		m_Dragging = true;
		CaptureMouse();
	}


	void OnMouseUp(cxMouseEvent event) override
	{
		m_Dragging = false;
		ReleaseMouse();
	}

	void OnMouseMove(cxMouseEvent event) override
	{
		cxLog(L"Mouse move");
		if (m_Dragging)
		{
			m_X = event.x;
			m_Y = event.y;

			Invalidate();
		}
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