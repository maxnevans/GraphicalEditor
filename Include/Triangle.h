#pragma once
#include "Rectangle.h"
class Triangle :
	public Rectangle
{
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics);
};

