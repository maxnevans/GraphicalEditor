#include "Ellipse.h"
#include "../ShapesFactory.h"

Custom::Ellipse::Ellipse()
	:
	BaseShape(ShapesFactory::ELLIPSE)
{
}

Custom::Ellipse::Ellipse(const wchar_t* name)
	:
	BaseShape(name)
{
}

void Custom::Ellipse::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;

	Pen pen(this->color);
	Rect rect(this->x, this->y, this->width, this->height);
	graphics->DrawEllipse(&pen, rect);
}
