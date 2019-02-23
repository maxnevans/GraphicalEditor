#pragma once
#include "Ellipse.h"
class Circle :
	public Ellipse
{
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics);
};

