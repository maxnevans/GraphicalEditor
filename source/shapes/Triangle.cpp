#include "Triangle.h"

void Custom::Triangle::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;

	Gdiplus::Pen pen(this->color);

	Gdiplus::Point points[3];
	points[0] = Point(this->x1, this->y2);
	points[1] = Point(this->x2, this->y2);
	points[2] = Point(this->x + this->width / 2, this->y1);

	graphics->DrawPolygon(&pen, points, 3);
}

Custom::BaseShape* Custom::Triangle::ShapeFactory()
{
	return new Triangle();
}

std::wstring Custom::Triangle::GetName()
{
	return this->NAME;
}
