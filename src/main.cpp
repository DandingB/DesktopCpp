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



static GLfloat pixels[] =
{
	1, 0, 0, 1,
	0, 1, 0, 1,
	0, 0, 1, 1,
	1, 1, 1, 1
};

GLuint m_Texture = -1;

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

		cxFillRect(0, 0, 500, 500, 0.2, 0.2, 0.2, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(10, 10);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(100, 10);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(100, 100);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(10, 100);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		EndPaint();
	}

};

class MyWindow2 : public cxWindowContainer
{
public:
	MyWindow2()
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

		cxFillRect(0, 0, 500, 500, 0.2, 0.2, 0.2, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(10, 10);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(200, 10);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(200, 200);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(10, 200);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		EndPaint();
	}

};



CX_FUNC_MAIN
{
	cxInitApp();

	
	MyWindow* window = new MyWindow;


	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	MyWindow2* window2 = new MyWindow2;


	window->SetTitle(L"Test");
	window->Show();
	window2->Show();

	cxRunApp();

	return 0;
}