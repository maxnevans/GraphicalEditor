#include "ComplexShape.h"

ComplexShape::ComplexShape(std::vector<BaseShape*> shapes)
{
	this->shapes = shapes;
}

void ComplexShape::Redraw(Gdiplus::Graphics* const graphics)
{
	for (BaseShape* shape : this->shapes)
		shape->Redraw(graphics);
}

const wchar_t* ComplexShape::GetName()
{
	return this->NAME;
}

IShapeFactoryFunctor* ComplexShape::CreateFactoryFunctor(std::vector<BaseShape*> shapes)
{
	return new ComplexShapeFactoryFunctor(shapes);
}

ComplexShape::ComplexShapeFactoryFunctor::ComplexShapeFactoryFunctor(std::vector<BaseShape*> shapes)
{
	this->shapes = shapes;
}

BaseShape* ComplexShape::ComplexShapeFactoryFunctor::operator()()
{
	return new ComplexShape(this->shapes);
}
