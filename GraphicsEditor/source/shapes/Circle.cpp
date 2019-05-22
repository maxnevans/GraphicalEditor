#include "Circle.h"

void Custom::Circle::Redraw(Gdiplus::Graphics * const graphics)
{

	Gdiplus::Pen pen(this->color);

	int dim = this->width < this->height ? this->height : this->width;
	int xAnchor = (this->x2 > this->x1) ? this->x1 : this->x1 - dim;
	int yAnchor = (this->y2 > this->y1) ? this->y1 : this->y1 - dim;

	Gdiplus::Rect rect(xAnchor, yAnchor, dim, dim);
	graphics->DrawEllipse(&pen, rect);
}

IShapeFactoryFunctor* Custom::Circle::CreateFactoryFunctor()
{
	return new CircleFactoryFunctor();
}

BaseShape* Custom::Circle::CircleFactoryFunctor::operator()()
{
	return new Circle();
}
