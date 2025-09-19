#pragma once

enum cxMouseButton
{
	LEFT,
	RIGHT,
	MIDDLE,
	NONE
};

struct cxMouseEvent
{
	float x, y;

	cxMouseButton button;
};


