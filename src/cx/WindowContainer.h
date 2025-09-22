#pragma once
#include "WindowBase.h"
#include <vector>

class cxWindowContainer;

class cxView
{
public:
	float m_Left, m_Top, m_Right, m_Bottom;
	//int m_Width, m_Height;

	cxView* m_Parent;
	cxWindowContainer* m_TopParent;
	std::vector<cxView*> m_SubViews;

	std::wstring m_Title;

	bool m_Show = true;

	cxView(float left, float top, float right, float bottom) : m_Left(left), m_Top(top), m_Right(right), m_Bottom(bottom), m_Parent(0) {}

	void AddView(cxView* view);

	bool PointInView(float x, float y);
	void GetWindowPos(float& x, float& y);
	void GetWindowRect(float& left, float& top, float& right, float& bottom);

	virtual void OnPaint(cxWindowContainer* container) {};
	virtual void OnSize() {};

	virtual void OnMouseDown(cxMouseEvent event) {}
	virtual void OnMouseUp(cxMouseEvent event) {}
	virtual void OnMouseMove(cxMouseEvent event) {}
	virtual void OnMouseEnter() {}
	virtual void OnMouseLeave() {}

	friend cxWindowContainer;
};



class cxWindowContainer : public cxWindowBase
{
	std::vector<cxView*> m_SubViews;

	cxView* m_pMouseOver;
	cxView* m_pHasCapture;

public:
	cxWindowContainer();
	~cxWindowContainer();

	void AddView(cxView* view);
	cxView* GetChildView(int i);

	void OnPaint() override;
	void OnSize(int width, int height) override;

	void OnMouseDown(cxMouseEvent event) override;
	void OnMouseUp(cxMouseEvent event) override;
	void OnMouseMove(cxMouseEvent event) override;
	void OnMouseLeave() override;

	cxView* GetViewAtLocation(std::vector<cxView*>& views, float top, float left, float right, float bottom, float x, float y);

private:
	void PaintSubviews(std::vector<cxView*>& views, float top, float left, float right, float bottom);

};
