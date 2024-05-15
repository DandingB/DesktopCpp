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

void cxDrawRect(int x, int y, int width, int height, float r, float g, float b, float a)
{
	glBegin(GL_LINES);
	glColor4f(r, g, b, a);

	glVertex2f(x, y);
	glVertex2f(x + width, y);

	glVertex2f(x + width, y);	
	glVertex2f(x + width, y + height);

	glVertex2f(x + width, y + height - 1);
	glVertex2f(x, y + height - 1);

	glVertex2f(x+1, y + height);
	glVertex2f(x+1, y);

	glEnd();
}

float g_SplitterX = 0.2;

class MyWindow : public cxWindowContainer
{
	bool m_Dragging = false;

public:
	MyWindow()
	{
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
		cxWindowContainer::OnSize(width, height);

		cxView* view1 = GetChildView(0);
		cxView* view2 = GetChildView(1);

		view1->m_Height = height - 20;
		view1->m_Width = width * g_SplitterX - 20;

		view2->m_X = width * g_SplitterX;
		view2->m_Width = width * (1.0 - g_SplitterX) - 10;
		view2->m_Height = height - 20;

		Invalidate();
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		cxWindowContainer::OnMouseDown(event);

		int width, height;
		GetClientSize(width, height);

		if (event.x > width * g_SplitterX - 20 and event.x < width * g_SplitterX)
		{
			m_Dragging = true;
		}
	}

	void OnMouseUp(cxMouseEvent event) override
	{
		cxWindowContainer::OnMouseUp(event);
		m_Dragging = false;
	}

	void OnMouseMove(cxMouseEvent event) override
	{
		cxWindowContainer::OnMouseMove(event);

		int width, height;
		GetClientSize(width, height);

		if (m_Dragging)
		{
			g_SplitterX = (float)(event.x+2) / width;

			if (g_SplitterX < 0.0)
				g_SplitterX = 0.0;

			if (g_SplitterX > 1.0)
				g_SplitterX = 1.0;


			cxView* view1 = GetChildView(0);
			cxView* view2 = GetChildView(1);

			view1->m_Height = height - 20;
			view1->m_Width = width * g_SplitterX - 20;

			view2->m_X = width * g_SplitterX;
			view2->m_Width = width * (1.0 - g_SplitterX) - 10;
			view2->m_Height = height - 20;

			Invalidate();
		}

		if (event.x > width * g_SplitterX - 20 and event.x < width * g_SplitterX)
		{
			SetCursor(cxSizeWE);
		}
		else
			SetCursor(cxArrow);
	}

};


class MyControl1: public cxView
{
public:

	MyControl1()
	{
		m_X = 10;
		m_Y = 10;
		m_Width = 100;
		m_Height = 120;
	}

	void OnPaint() override
	{
		cxFillRect(0, 0, m_Width, m_Height, 0.2, 0.2, 0.2, 1.0);
		cxDrawRect(0, 0, m_Width, m_Height, 1.0, 1.0, 1.0, 1.0);

		glBegin(GL_QUADS);
		glColor4f(0.5, 1.0, 0.4, 1.0);
		glVertex2f(10, 10);
		glVertex2f(100, 10);
		glVertex2f(120, 100);
		glVertex2f(10, 100);
		glEnd();

	}

	void OnMouseDown(cxMouseEvent event) override
	{
		cxLog(L"MyControl1: x: %d, y: %d\n", event.x, event.y);
	}

	void OnMouseMove(cxMouseEvent event) override
	{
		//cxLog(L"MyControl1: x: %d, y: %d\n", event.x, event.y);
	}
};


class MyControl2 : public cxView
{
public:

	MyControl2()
	{
		m_X = 200;
		m_Y = 10;
		m_Width = 100;
		m_Height = 120;
	}

	void OnPaint() override
	{
		cxFillRect(0, 0, m_Width, m_Height, 0.2, 0.2, 0.2, 1.0);
		cxFillRect(20, 50, m_Width - 40, 20, 1.0, 1.0, 0.0, 1.0);
		cxDrawRect(0, 0, m_Width, m_Height, 1.0, 1.0, 1.0, 1.0);
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		cxLog(L"MyControl2: x: %d, y: %d\n", event.x, event.y);
	}

	void OnMouseMove(cxMouseEvent event) override
	{
		//cxLog(L"MyControl2: x: %d, y: %d\n", event.x, event.y);
	}
};


class MyView : public cxView
{
public:

	MyView()
	{
		m_X = 10;
		m_Y = 300;
		m_Width = 400;
		m_Height = 400;
	}

	void OnPaint() override
	{
		cxFillRect(0, 0, m_Width, m_Height, 0.4, 0.4, 0.4, 1.0);
		cxDrawRect(0, 0, m_Width, m_Height, 1.0, 1.0, 1.0, 1.0);
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		cxLog(L"MyControl2: x: %d, y: %d\n", event.x, event.y);
	}
};


class MyButton : public cxView
{
public:

	MyButton()
	{
		m_X = 20;
		m_Y = 20;
		m_Width = 200;
		m_Height = 60;
	}

	void OnPaint() override
	{
		cxFillRect(0, 0, m_Width, m_Height, 0.0, 0.8, 0.2, 1.0);
		cxDrawRect(0, 0, m_Width, m_Height, 1.0, 1.0, 1.0, 1.0);
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		cxLog(L"MyControl2: x: %d, y: %d\n", event.x, event.y);
	}
};


CX_FUNC_MAIN
{
	cxInitApp();


	MyControl1* control1 = new MyControl1;
	MyControl2* control2 = new MyControl2;

	MyView* view = new MyView;
	control1->AddView(view);

	MyButton* btn = new MyButton;
	view->AddView(btn);

	MyWindow* window = new MyWindow;
	window->SetTitle(L"Test");

	window->AddView(control1);
	window->AddView(control2);
	window->Show();


	cxRunApp();

	return 0;
}