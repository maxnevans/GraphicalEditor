#pragma once
#include <windows.h>
#include <gdiplus.h>

class BaseShape
{
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics) = 0;
	virtual void SetPoints(int x1, int y1, int x2, int y2);
	void SetColor(Gdiplus::Color color);

protected:
	int x, y, width, height;
	int x1, y1, x2, y2;
	Gdiplus::Color color;
};

