#include "Common.h"

class TabControl : public cxView
{
	int m_SelPage = 0;
	int m_PageHover = -1;
	bool m_HoverClose = false;

	float m_TabPadding = 50.f;
	float m_TabHeight = 40.f;

	using cxView::cxView;

	void GetTabIndex(float xPos, float yPos, int& index, bool& isClose)
	{
		index = -1;
		isClose = false;

		float tabMiddle = m_TabHeight * 0.5f;
		float x = 0;
		for (int i = 0; i < m_SubViews.size(); i++)
		{
			cxView* view = m_SubViews[i];

			float width, height;
			font->GetFontTextMetrics(
				view->m_Title,
				200,
				30,
				{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER },
				width,
				height
			);

			if (xPos > x and xPos < x + width + m_TabPadding)
			{
				index = i;
				if (xPos > x + width + m_TabPadding - 20.f and xPos < x + width + m_TabPadding - 10.f and yPos > tabMiddle - 5.f and yPos < tabMiddle + 5.f)
					isClose = true;
			}
			x += width + m_TabPadding;
		}
	}

	void OnSize() override
	{
		for (cxView* view : m_SubViews)
		{
			view->m_Left = 0;
			view->m_Top = m_TabHeight;
			view->m_Right = m_Right;
			view->m_Bottom = m_Bottom;
			view->OnSize();
		}
	}


	void OnMouseMove(cxMouseEvent event) override
	{
		int iTab;
		bool isClose;
		GetTabIndex(event.x, event.y, iTab, isClose);
		m_PageHover = iTab;
		m_HoverClose = isClose;
		m_TopParent->Invalidate();
	}

	void OnMouseDown(cxMouseEvent event) override
	{
		int iTab;
		bool isClose;
		GetTabIndex(event.x, event.y, iTab, isClose);
		if (iTab != -1)
		{
			m_SelPage = iTab;

			if (isClose)
			{
				delete m_SubViews[iTab];
				m_SubViews.erase(m_SubViews.begin() + iTab);
				if (m_SelPage >= m_SubViews.size())
					m_SelPage--;
			}


			for (cxView* view : m_SubViews)
				view->m_Show = false;
			if (m_SubViews.size() > 0)
				m_SubViews[m_SelPage]->m_Show = true;


		}
		m_TopParent->Invalidate();
	}

	void OnMouseEnter() override
	{
		cxSetCursor(cxCursorType::cxArrow);
	}

	void OnMouseLeave() override
	{
		m_HoverClose = false;
		m_PageHover = -1;
		m_TopParent->Invalidate();
	}

	void OnPaint(cxWindowContainer* container) override
	{
		container->FillRectangle({ 0,0,m_Right - m_Left,m_Bottom - m_Top }, BRUSH_DARKERGREY);

		int i = 0;
		float x = 0;
		for (cxView* views : m_SubViews)
		{
			float width, height;
			font->GetFontTextMetrics(
				views->m_Title,
				200,
				m_TabHeight,
				{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER },
				width,
				height
			);

			if (i == m_PageHover)
				container->FillRectangle({ x, 0, x + width + m_TabPadding, m_TabHeight }, BRUSH_TABHOVERGREY);

			if (i == m_SelPage)
				container->FillRectangle({ x, 0, x + width + m_TabPadding, m_TabHeight }, BRUSH_TABGREY);

			container->DrawTextInRect(
				font.get(),
				i == m_SelPage ? BRUSH_TEXTWHITE : BRUSH_TEXTGREY,
				views->m_Title,
				{ x, 0, x + width + m_TabPadding - 20.f, m_TabHeight },
				{ cxTextOptions::TEXT_ALIGNMENT_CENTER, cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER }
			);

			float tabMiddle = m_TabHeight * 0.5f;
			float closePadding = 10.f;
			container->DrawLine(
				{
					x + width + m_TabPadding - closePadding,
					tabMiddle + 5.f
				},
				{
					x + width + m_TabPadding - 10.f - closePadding,
					tabMiddle - 5.f
				},
				i == m_PageHover and m_HoverClose ? BRUSH_TEXTWHITE : BRUSH_TEXTGREY,
				2.f
			);

			container->DrawLine(
				{
					x + width + m_TabPadding - 10.f - closePadding,
					tabMiddle + 5.f
				},
				{
					x + width + m_TabPadding - closePadding,
					tabMiddle - 5.f
				},
				i == m_PageHover and m_HoverClose ? BRUSH_TEXTWHITE : BRUSH_TEXTGREY,
				2.f
			);

			x += width + m_TabPadding;
			i++;
		}
	}

public:
	void SetSelectedPage(int page)
	{
		if (page >= 0 and page < m_SubViews.size())
		{
			m_SelPage = page;

			for (cxView* view : m_SubViews)
				view->m_Show = false;
			if (m_SubViews.size() > 0)
				m_SubViews[m_SelPage]->m_Show = true;

			m_TopParent->Invalidate();
		}
	}
};
