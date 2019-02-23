#include "..\Include\Polygon.h"

void Polygon::SetPoints(const Gdiplus::Point* points, int count)
{
	this->points = points;
	this->pointsCount = count;
}

void Polygon::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;

	Pen pen(this->color);

	graphics->DrawPolygon(&pen, this->points, this->pointsCount);
}
