#pragma once
#include "WindowBase.h"
#include "Event.h"

#include <vector>

class cxWindowContainer;

class cxView
{
public:
	int m_X, m_Y;
	int m_Width, m_Height;

	//std::vector<cxView> m_SubViews;


	virtual void OnPaint() {};
	virtual void OnSize() {};

	virtual void OnMouseDown(cxMouseEvent event) {}
	virtual void OnMouseUp(cxMouseEvent event) {}
	virtual void OnMouseMove(cxMouseEvent event) {}
	virtual void OnMouseDragged(cxMouseEvent event) {}

	friend cxWindowContainer;
};

class cxWindowContainer : public cxWindowBase
{

	std::vector<cxView*> m_SubViews;

public:
	void AddView(cxView* view);

	cxView* GetChildView(int i);

	void OnPaint() override;



};
