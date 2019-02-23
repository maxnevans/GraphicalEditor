#pragma once
#include "Rectangle.h"
class Ellipse :
	public Rectangle
{
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics);
};

