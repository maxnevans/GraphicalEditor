#include "Ellipse.h"

void Custom::Ellipse::Redraw(Gdiplus::Graphics* const graphics)
{

	Gdiplus::Pen pen(this->color);
	Gdiplus::Rect rect(this->x, this->y, this->width, this->height);
	graphics->DrawEllipse(&pen, rect);
}