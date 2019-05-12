#include "Polygon.h"

void Custom::Polygon::SetPoints(const Gdiplus::Point* points, int count)
{
	this->points = points;
	this->pointsCount = count;
}

void Custom::Polygon::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;

	Pen pen(this->color);

	graphics->DrawPolygon(&pen, this->points, this->pointsCount);
}
