#include "Circle.h"

void Custom::Circle::Redraw(Gdiplus::Graphics * const graphics)
{
	using namespace Gdiplus;

	Pen pen(this->color);

	int dim = (this->width - this->height) > 0 ? this->height : this->width;
	int xAnchor = (this->x2 > this->x1) ? this->x1 : this->x1 - dim;
	int yAnchor = (this->y2 > this->y1) ? this->y1 : this->y1 - dim;

	Rect rect(xAnchor, yAnchor, dim, dim);
	graphics->DrawEllipse(&pen, rect);
}
