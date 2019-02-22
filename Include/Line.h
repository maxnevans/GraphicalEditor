#pragma once
#include "BaseShape.h"
class Line :
	public BaseShape
{
public:
	Line(Gdiplus::Graphics* const graphics);
	void Redraw() override;
	~Line();
};

