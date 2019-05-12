#include "Triangle.h"
#include "../ShapesFactory.h"

Custom::Triangle::Triangle()
	:
	Line(ShapesFactory::TRIANGLE)
{
}

Custom::Triangle::Triangle(const wchar_t* name)
	:
	Line(name)
{
}

void Custom::Triangle::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;

	Pen pen(this->color);

	Point points[3];
	points[0] = Point(this->x1, this->y2);
	points[1] = Point(this->x2, this->y2);
	points[2] = Point(this->x + this->width / 2, this->y1);

	graphics->DrawPolygon(&pen, points, 3);
}
