#include "WindowContainer.h"
#include "../platform/Platform.h"

void cxWindowContainer::AddView(cxView* view)
{
	m_SubViews.push_back(view);
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT);


	for (cxView* view : m_SubViews)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, view->m_Width, view->m_Height, 0, -1, 1);
		glViewport(view->m_X, height - view->m_Height - view->m_Y, view->m_Width, view->m_Height);
		
		view->OnPaint();
	}

	glFlush();
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
			view->OnMouseDown({ event.x - view->m_X, event.y - view->m_Y, event.button });
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
			view->OnMouseDown({ event.x - view->m_X, event.y - view->m_Y, event.button });
			break;
		}
	}
}
