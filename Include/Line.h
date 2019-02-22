#pragma once
#include "BaseShape.h"
class Line :
	public BaseShape
{
public:
	void Redraw(Gdiplus::Graphics* const graphics) override;
	~Line();
};

