#pragma once
#include "Triangle.h"
class Polygon :
	public Triangle
{
public:
	virtual void SetPoints(const Gdiplus::Point* points, int count);
	virtual void Redraw(Gdiplus::Graphics* const graphics);
protected:
	const Gdiplus::Point* points;
	int pointsCount;
};

