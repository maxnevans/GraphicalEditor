#include "..\Include\Circle.h"



Circle::Circle()
{
}


Circle::~Circle()
{
}

void Circle::Redraw(Gdiplus::Graphics * const graphics)
{
	using namespace Gdiplus;

	Pen pen(this->color);

	int dim = (this->width - this->height) > 0 ? this->height : this->width;

	Rect rect(this->x, this->y, dim, dim);
	graphics->DrawEllipse(&pen, rect);
}