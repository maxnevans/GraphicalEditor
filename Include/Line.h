#pragma once
#include "BaseShape.h"
class Line :
	public BaseShape
{
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics);
};

