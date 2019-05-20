#include "Line.h"

void Custom::Line::Redraw(Gdiplus::Graphics* const graphics)
{
	Gdiplus::Pen pen(this->color);
	graphics->DrawLine(&pen, this->x1, this->y1, this->x2, this->y2);
}

IShapeFactoryFunctor* Custom::Line::CreateFactoryFunctor()
{
	return new LineFactoryFunctor();
}

const wchar_t* Custom::Line::GetName()
{
	return this->NAME;
}

BaseShape* Custom::Line::LineFactoryFunctor::operator()()
{
	return new Line();
}
