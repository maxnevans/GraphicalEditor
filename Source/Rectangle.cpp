#include "..\Include\Rectangle.h"


void Rectangle::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;
	Pen pen(this->color);

	Rect rect(this->x, this->y, this->width, this->height);
	graphics->DrawRectangle(&pen, rect);
}