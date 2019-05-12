#include "Circle.h"
#include "../ShapesFactory.h"

Custom::Circle::Circle()
	:
	Ellipse(ShapesFactory::CIRCLE)
{
}

Custom::Circle::Circle(const wchar_t* name)
	:
	Ellipse(name)
{
}

void Custom::Circle::Redraw(Gdiplus::Graphics * const graphics)
{
	using namespace Gdiplus;

	Pen pen(this->color);

	int dim = this->width < this->height ? this->height : this->width;
	int xAnchor = (this->x2 > this->x1) ? this->x1 : this->x1 - dim;
	int yAnchor = (this->y2 > this->y1) ? this->y1 : this->y1 - dim;

	Rect rect(xAnchor, yAnchor, dim, dim);
	graphics->DrawEllipse(&pen, rect);
}
