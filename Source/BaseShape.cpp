#include "..\Include\BaseShape.h"

using namespace Gdiplus;

BaseShape::BaseShape(Graphics* const graphics)
	:
	graphics(graphics)
{

}

void BaseShape::SetPoints(int x1, int y1, int x2, int y2)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
}

void BaseShape::SetColor(Gdiplus::Color color)
{
	this->color = color;
}