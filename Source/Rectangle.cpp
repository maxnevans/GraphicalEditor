#include "Rectangle.h"
#include "ShapesFactory.h"

Custom::Rectangle::Rectangle()
	:
	Line(ShapesFactory::RECTANGLE)
{
}

Custom::Rectangle::Rectangle(const wchar_t* name)
	:
	Line(name)
{
}

void Custom::Rectangle::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;
	Pen pen(this->color);

	Rect rect(this->x, this->y, this->width, this->height);
	graphics->DrawRectangle(&pen, rect);
}