#include "Line.h"

void Custom::Line::Redraw(Gdiplus::Graphics* const graphics)
{
	Gdiplus::Pen pen(this->color);
	graphics->DrawLine(&pen, this->x1, this->y1, this->x2, this->y2);
}

Custom::BaseShape* Custom::Line::ShapeFactory()
{
	return new Line();
}

std::wstring Custom::Line::GetName()
{
	return this->NAME;
}
