#include "WindowContainer.h"
#include "platform/Platform.h"

cxView* g_pFocusView = nullptr;

std::unordered_map<cxView*, cxWindowContainer*> g_WindowRelations;


cxView::cxView(float left, float top, float right, float bottom, cxView* parent)
	: m_Left(left), m_Top(top), m_Right(right), m_Bottom(bottom), m_Parent(parent), m_TopParent(nullptr)
{
	if (m_Parent) m_Parent->AddView(this);
}

cxView::cxView(float left, float top, float right, float bottom, cxWindowContainer* parent)
	: m_Left(left), m_Top(top), m_Right(right), m_Bottom(bottom), m_Parent(nullptr), m_TopParent(parent)
{
	if (m_TopParent) m_TopParent->AddView(this);
}

bool cxView::PointInView(float x, float y)
{
	float left, top, right, bottom;
	GetWindowRect(left, top, right, bottom);

	if (x > left and x < right and y > top and y < bottom)
		return true;
	else
		return false;
}

void cxView::GetWindowPos(float& x, float& y)
{
	if (!m_Parent)
	{
		x = m_Left;
		y = m_Top;
	}
	else
	{
		float x1, y1;
		m_Parent->GetWindowPos(x1, y1);
		x = x1 + m_Left;
		y = y1 + m_Top;
	}
}

void cxView::GetWindowRect(float& left, float& top, float& right, float& bottom)
{
	if (!m_Parent)
	{
		left = m_Left;
		top = m_Top;
		right = m_Right;
		bottom = m_Bottom;
	}
	else
	{
		float left1, top1, right1, bottom1;
		m_Parent->GetWindowRect(left1, top1, right1, bottom1);

		left = left1 + m_Left;
		top = top1 + m_Top;
		right = left1 + m_Right;
		bottom = top1 + m_Bottom;

		if (right > right1)
			right = right1;
			
		if (bottom > bottom1)
			bottom = bottom1;
	}
}

void cxView::GetSize(float& width, float& height)
{
	width = m_Right - m_Left;
	height = m_Bottom - m_Top;
}

cxWindowContainer* cxView::GetWindowContainer()
{
	cxView* view = this;
	while (view->m_Parent != nullptr)
	{
		view = view->m_Parent;
	}

	if (!view->m_TopParent)
		cxLog(L"cxView::GetWindowContainer cannot get a m_TopParent");

	return view->m_TopParent;
}

void cxView::SetFocus()
{
	if (g_pFocusView)
		g_pFocusView->OnFocusLost();

	g_pFocusView = this;
}

bool cxView::HasFocus()
{
	return g_pFocusView == this;
}

void cxView::ShowCaret(bool show)
{
	cxWindowContainer* window = GetWindowContainer();
	if (window)
		window->ShowCaret(show);
}

void cxView::SetCaretPos(cxPoint p)
{
	cxWindowContainer* window = GetWindowContainer();
	if (window)
	{
		float x, y;
		GetWindowPos(x, y);
		window->SetCaretPos({x + p.x, y + p.y });
	}
}

void cxView::AddView(cxView* view)
{
	m_SubViews.push_back(view);
	//view->m_Parent = this;
	//view->m_TopParent = m_TopParent;
	Invalidate();
}

void cxView::Invalidate()
{
	cxWindowContainer* window = GetWindowContainer();
	if (window)
	{
		window->Invalidate();
	}
}



cxWindowContainer::cxWindowContainer() : m_pMouseOver(nullptr), m_pHasCapture(nullptr)
{
}

cxWindowContainer::~cxWindowContainer()
{
	
}

void cxWindowContainer::AddView(cxView* view)
{
	m_SubViews.push_back(view);
	//view->m_Parent = nullptr;
	//view->m_TopParent = this;
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
	float width, height;
	GetClientSize(width, height);

	PaintSubviews(m_SubViews, 0, 0, width, height);
}


void cxWindowContainer::PaintSubviews(std::vector<cxView*>& views, float left, float top, float right, float bottom)
{
	for (cxView* view : views)
	{
		if (!(view->m_Show))
			continue;

		float sub_left, sub_top, sub_right, sub_bottom;

		sub_left = left + view->m_Left;
		sub_top = top + view->m_Top;
		sub_right = left + view->m_Right;
		sub_bottom = top + view->m_Bottom;

		if (sub_right > right) sub_right = right;
		if (sub_bottom > bottom) sub_bottom = bottom;
		if ((sub_right - sub_left) <= 0 or (sub_bottom - sub_top) <= 0) continue;

		SetDrawConstraints({ sub_left, sub_top, sub_right, sub_bottom });
		view->OnPaint(this);
		RemoveDrawConstraints();

		PaintSubviews(view->m_SubViews, sub_left, sub_top, sub_right, sub_bottom);
	}
}

void cxWindowContainer::OnSize(float width, float height)
{
	cxWindowBase::OnSize(width, height);
}


void cxWindowContainer::OnMouseDown(cxMouseEvent event)
{
	float width, height;
	GetClientSize(width, height);

	cxView* view = GetViewAtLocation(m_SubViews, 0, 0, width, height, event.x, event.y);
	if (view)
	{
		float x, y;
		view->GetWindowPos(x, y);
		view->OnMouseDown({ event.x - x, event.y - y, event.button });
	}
}

void cxWindowContainer::OnMouseUp(cxMouseEvent event)
{
	float width, height;
	GetClientSize(width, height);

	cxView* view = GetViewAtLocation(m_SubViews, 0, 0, width, height, event.x, event.y);
	if (view)
	{
		float x, y;
		view->GetWindowPos(x, y);
		view->OnMouseUp({ event.x - x, event.y - y, event.button });
	}
}

void cxWindowContainer::OnMouseMove(cxMouseEvent event)
{
	float width, height;
	GetClientSize(width, height);

	cxView* view = GetViewAtLocation(m_SubViews, 0, 0, width, height, event.x, event.y);
	if (view)
	{
		if (view != m_pMouseOver)
		{
			if (m_pMouseOver)
				m_pMouseOver->OnMouseLeave();

			view->OnMouseEnter();
		}
		m_pMouseOver = view;

		float x, y;
		view->GetWindowPos(x, y);
		view->OnMouseMove({ event.x - x, event.y - y, event.button });
	}
}

void cxWindowContainer::OnMouseLeave()
{
	if (m_pMouseOver)
	{
		m_pMouseOver->OnMouseLeave();
		m_pMouseOver = nullptr;
	}
}

void cxWindowContainer::OnMouseScroll(cxMouseScrollEvent event)
{
	float width, height;
	GetClientSize(width, height);

	cxView* view = GetViewAtLocation(m_SubViews, 0, 0, width, height, event.x, event.y);
	if (view)
	{
		m_pMouseOver = view;

		float x, y;
		view->GetWindowPos(x, y);
		m_pMouseOver->OnMouseScroll({ event.x - x, event.y - y, event.scrollX, event.scrollY });
	}
}

void cxWindowContainer::OnKeyDown(cxKeyEvent event)
{
	if (g_pFocusView) g_pFocusView->OnKeyDown(event);
}

void cxWindowContainer::OnFocusLost()
{
	if (g_pFocusView) g_pFocusView->OnFocusLost();
	g_pFocusView = nullptr;
}


cxView* cxWindowContainer::GetViewAtLocation(std::vector<cxView*>& views, float top, float left, float right, float bottom, float x, float y)
{
	for (int i = views.size() - 1; i >= 0; i--)
	{
		cxView* view = views[i];
		if (!(view->m_Show))
			continue;

		float left1, top1, right1, bottom1;

		left1 = left + view->m_Left;
		top1 = top + view->m_Top;
		right1 = left + view->m_Right;
		bottom1 = top + view->m_Bottom;

		if (right1 > right) right1 = right;
		if (bottom1 > bottom) bottom1 = bottom;
		if (left1 == right1 or top1 == bottom1) continue;

		if ((x > left1) and (x < right1) and (y > top1) and (y < bottom1))
		{
			cxView* subview = GetViewAtLocation(view->m_SubViews, top1, left1, right1, bottom1, x, y);
			if (subview)
				return subview;
			else
				return view;
		}
	}
	return nullptr;
}