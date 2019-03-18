#include "BaseShape.h"

void Custom::BaseShape::SetPoints(int x1, int y1, int x2, int y2)
{
	this->x = (x1 - x2) < 0 ? x1 : x2;
	this->y = (y1 - y2) < 0 ? y1 : y2;
	this->width = (x1 - x2) < 0 ? x2 - x1 : x1 - x2;
	this->height = (y1 - y2) < 0 ? y2 - y1 : y1 - y2;
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}

void Custom::BaseShape::SetColor(Gdiplus::Color color)
{
	this->color = color;
}