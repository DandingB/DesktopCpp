
enum cxMouseButton
{
	LEFT,
	RIGHT,
	MIDDLE,
	NONE
};

struct cxMouseEvent
{
	int x, y;

	cxMouseButton button;
};


