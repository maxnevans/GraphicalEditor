#include "Ellipse.h"

void Custom::Ellipse::Redraw(Gdiplus::Graphics* const graphics)
{

	Gdiplus::Pen pen(this->color);
	Gdiplus::Rect rect(this->x, this->y, this->width, this->height);
	graphics->DrawEllipse(&pen, rect);
}

BaseShape* Custom::Ellipse::ShapeFactory()
{
	return new Ellipse();
}

const wchar_t* Custom::Ellipse::GetName()
{
	return this->NAME;
}
