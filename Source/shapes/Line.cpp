#include "Line.h"
#include "../ShapesFactory.h"
#include <sstream>

using namespace Gdiplus;

Custom::Line::Line()
	:
	BaseShape(ShapesFactory::LINE)
{
}

Custom::Line::Line(const wchar_t* name)
	:
	BaseShape(name)
{
}

void Custom::Line::Redraw(Graphics* const graphics)
{
	Pen pen(this->color);
	graphics->DrawLine(&pen, this->x1, this->y1, this->x2, this->y2);
}
