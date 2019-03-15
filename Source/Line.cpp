#include "..\Include\Line.h"

using namespace Gdiplus;

void Custom::Line::Redraw(Graphics* const graphics)
{
	Pen pen(this->color);
	graphics->DrawLine(&pen, this->x1, this->y1, this->x2, this->y2);
}
