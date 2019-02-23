#pragma once
#include "Line.h"
class Rectangle :
	public Line
{
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics);
};

