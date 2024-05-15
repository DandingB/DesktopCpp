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
