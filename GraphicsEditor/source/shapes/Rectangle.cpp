#include "Rectangle.h"


void Custom::Rectangle::Redraw(Gdiplus::Graphics* const graphics)
{
	Gdiplus::Pen pen(this->color);

	Gdiplus::Rect rect(this->x, this->y, this->width, this->height);
	graphics->DrawRectangle(&pen, rect);
}

Custom::BaseShape* Custom::Rectangle::ShapeFactory()
{
	return new Rectangle();
}

std::wstring Custom::Rectangle::GetName()
{
	return this->NAME;
}
