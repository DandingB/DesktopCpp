#pragma once
#include "WindowBase.h"
#include "Event.h"

#include <vector>



class cxWindowContainer;

class cxView
{
public:
	int m_Left, m_Top, m_Right, m_Bottom;
	//int m_Width, m_Height;

	cxView* m_Parent;
	std::vector<cxView*> m_SubViews;

	std::wstring m_Title;

	void AddView(cxView* view);

	bool PointInView(int x, int y);
	void GetWindowPos(int& x, int& y);
	void GetWindowRect(int& left, int& top, int& right, int& bottom);

	virtual void OnPaint() {};
	virtual void OnSize() {};

	virtual void OnMouseDown(cxMouseEvent event) {}
	virtual void OnMouseUp(cxMouseEvent event) {}
	virtual void OnMouseMove(cxMouseEvent event) {}

	friend cxWindowContainer;
};



class cxWindowContainer : public cxWindowBase
{
	std::vector<cxView*> m_SubViews;

	unsigned multisampleFBO;
	unsigned multisampleColorBuffer;

public:
	cxWindowContainer();
	~cxWindowContainer();

	void AddView(cxView* view);
	cxView* GetChildView(int i);

	void StartPaint();
	void EndPaint();

	void OnPaint() override;
	void OnSize(int width, int height) override;

	void OnMouseDown(cxMouseEvent event) override;
	void OnMouseUp(cxMouseEvent event) override;
	void OnMouseMove(cxMouseEvent event) override;

private:
	void PaintSubviews(std::vector<cxView*>& views, int top, int left, int right, int bottom);

};
