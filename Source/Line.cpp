#include "..\Include\Line.h"

using namespace Gdiplus;

Line::Line(Graphics* const graphics)
	:
	BaseShape(graphics)
{
}

void Line::Redraw()
{
	Pen pen(this->color);
	this->graphics->DrawLine(&pen, this->x1, this->y1, this->x2, this->y2);
}


Line::~Line()
{
}
