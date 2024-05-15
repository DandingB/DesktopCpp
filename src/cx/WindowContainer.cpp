#include "WindowContainer.h"
#include "../platform/Platform.h"


bool cxView::PointInView(int x, int y)
{
	if (!m_Parent)
	{
		if ((x > m_X) and (x < m_X + m_Width) and (y > m_Y) and (y < m_Y + m_Height))
			return true;
	}
	else
	{

	}
	return false;
}

void cxView::PosInWindow(int& x, int& y)
{
	if (!m_Parent)
	{
		x = m_X;
		y = m_Y;
	}
	else
	{
		int x1, y1;
		m_Parent->PosInWindow(x1, y1);
		x = m_X + x1;
		y = m_Y + y1;
	}
}

void cxView::AddView(cxView* view)
{
	m_SubViews.push_back(view);
	view->m_Parent = this;
}


cxWindowContainer::cxWindowContainer()
{
	//glEnable(GL_MULTISAMPLE);

 //   // Create multisample framebuffer
 //   glGenFramebuffers(1, &multisampleFBO);
 //   glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);

 //   // Create multisample color buffer
 //   glGenRenderbuffers(1, &multisampleColorBuffer);
 //   glBindRenderbuffer(GL_RENDERBUFFER, multisampleColorBuffer);
 //   glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGBA8, 500, 500);
 //   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multisampleColorBuffer);

 //     // Check framebuffer completeness
 //   GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
 //   if (status != GL_FRAMEBUFFER_COMPLETE) {
	//	cxLog(L"Framebuffer is incomplete: %x", status);
 //   }
}

cxWindowContainer::~cxWindowContainer()
{
	
}

void cxWindowContainer::AddView(cxView* view)
{
	m_SubViews.push_back(view);
	view->m_Parent = nullptr;
}

cxView* cxWindowContainer::GetChildView(int i)
{
	if (i >= 0 and i < m_SubViews.size())
		return m_SubViews[i];
	else
		return nullptr;
}

void cxWindowContainer::OnPaint()
{
	int width, height;
	GetClientSize(width, height);

	//glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT);

	cxLog(L"---------------");

	PaintSubViews(m_SubViews, 0, 0, width, height);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
 //   glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
 //   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
 //   glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
 //   glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glFlush();
}


void cxWindowContainer::PaintSubViews(std::vector<cxView*>& views, int left, int top, int right, int bottom)
{
	int clientWidth, clientHeight;
	GetClientSize(clientWidth, clientHeight);

	for (cxView* view : views)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, view->m_Width, view->m_Height, 0, -1, 1);

		int x, y, width, height;

		x = left + view->m_X;
		y = top + view->m_Y;
		width = view->m_Width;
		height = view->m_Height;

		if (x > right or y > bottom)
			continue;

		if (x + view->m_Width > right)
			width = right - x;
			
		if (y + view->m_Height > bottom)
			height = bottom - y;


		cxLog(L"x: %d, y: %d, width: %d, height: %d", x, y, width, height);


		glViewport(x, clientHeight - height - y, width, height);
		
		view->OnPaint();

		PaintSubViews(view->m_SubViews, x, y, x + width, y + height);
	}
}


void cxWindowContainer::OnSize(int width, int height)
{

	void* test = wglGetProcAddress("glBindRenderbuffer");

	(test)(2, multisampleColorBuffer);

	//glBindRenderbuffer(GL_RENDERBUFFER, multisampleColorBuffer);
 //   glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGBA8, width, height);

	//HMODULE module = LoadLibraryA("opengl32.dll");
	//p = (void*)GetProcAddress(module, name);
}


void cxWindowContainer::OnMouseDown(cxMouseEvent event)
{
	for (int i = m_SubViews.size() - 1; i >= 0; i--)
	{
		cxView* view = m_SubViews[i];
		if ((event.x > view->m_X) and (event.x < view->m_X + view->m_Width) and (event.y > view->m_Y) and (event.y < view->m_Y + view->m_Height))
		{
			view->OnMouseDown({ event.x - view->m_X, event.y - view->m_Y, event.button});
			break;
		}
	}
}

void cxWindowContainer::OnMouseUp(cxMouseEvent event)
{
	for (int i = m_SubViews.size() - 1; i >= 0; i--)
	{
		cxView* view = m_SubViews[i];
		if ((event.x > view->m_X) and (event.x < view->m_X + view->m_Width) and (event.y > view->m_Y) and (event.y < view->m_Y + view->m_Height))
		{
			view->OnMouseUp({ event.x - view->m_X, event.y - view->m_Y, event.button });
			break;
		}
	}
}

void cxWindowContainer::OnMouseMove(cxMouseEvent event)
{
	for (int i = m_SubViews.size() - 1; i >= 0; i--)
	{
		cxView* view = m_SubViews[i];
		if ((event.x > view->m_X) and (event.x < view->m_X + view->m_Width) and (event.y > view->m_Y) and (event.y < view->m_Y + view->m_Height))
		{
			view->OnMouseMove({ event.x - view->m_X, event.y - view->m_Y, event.button });
			break;
		}
	}
}
