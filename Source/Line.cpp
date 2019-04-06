#include "Line.h"
#include "ShapesFactory.h"
#include <sstream>

using namespace Gdiplus;

void Custom::Line::Redraw(Graphics* const graphics)
{
	Pen pen(this->color);
	graphics->DrawLine(&pen, this->x1, this->y1, this->x2, this->y2);
}

std::wstring Custom::Line::SerializeText()
{
	std::wstringstream ss;
	ss << ShapesFactory::LINE << " ";
	ss << BaseShape::SerializeText();
	return ss.str();
}
