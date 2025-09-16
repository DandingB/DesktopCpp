#pragma once
#include "../platform/WindowBase.h"
#include <vector>

class cxWindowContainer;

class cxView
{
public:
	int m_Left, m_Top, m_Right, m_Bottom;
	//int m_Width, m_Height;

	cxView* m_Parent;
	cxWindowContainer* m_TopParent;
	std::vector<cxView*> m_SubViews;

	std::wstring m_Title;

	bool m_Show = true;

	cxView(int left, int top, int right, int bottom) : m_Left(left), m_Top(top), m_Right(right), m_Bottom(bottom), m_Parent(0) {}

	void AddView(cxView* view);

	bool PointInView(int x, int y);
	void GetWindowPos(int& x, int& y);
	void GetWindowRect(int& left, int& top, int& right, int& bottom);

	virtual void OnPaint(cxWindowContainer* container) {};
	virtual void OnSize() {};

	virtual void OnMouseDown(cxMouseEvent event) {}
	virtual void OnMouseUp(cxMouseEvent event) {}
	virtual void OnMouseMove(cxMouseEvent event) {}

	friend cxWindowContainer;
};



class cxWindowContainer : public cxWindowBase
{
	std::vector<cxView*> m_SubViews;

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

	cxView* GetViewAtLocation(std::vector<cxView*>& views, int top, int left, int right, int bottom, int x, int y);

private:
	void PaintSubviews(std::vector<cxView*>& views, int top, int left, int right, int bottom);

};
