#include "Rectangle.h"


void Custom::Rectangle::Redraw(Gdiplus::Graphics* const graphics)
{
	Gdiplus::Pen pen(this->color);

	Gdiplus::Rect rect(this->x, this->y, this->width, this->height);
	graphics->DrawRectangle(&pen, rect);
}