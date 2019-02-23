#pragma once
#include "Rectangle.h"
class Square :
	public Rectangle
{
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics);
};

