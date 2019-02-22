#pragma once
#include <windows.h>
#include <gdiplus.h>

class BaseShape
{
public:
	BaseShape();
	virtual void Redraw() = 0;
	void SetPoints(int x1, int y1, int x2, int y2);
	void SetColor(Gdiplus::Color);

protected:
	int x1, x2, y1, y2;
	Gdiplus::Color color;
};

