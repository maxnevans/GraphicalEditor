#include "ShapesFactory.h"
#include <sstream>
#include "Exception.h"
#include "shapes/Line.h"
#include "shapes/Rectangle.h"
#include "shapes/Square.h"
#include "shapes/Triangle.h"
#include "shapes/Polygon.h"
#include "shapes/Ellipse.h"
#include "shapes/Circle.h"

Custom::BaseShape* ShapesFactory::CreateShape(std::wstring name)
{
	Custom::BaseShape* shape = nullptr;

	if (name == LINE)
	{
		shape = new Custom::Line();
	}
	else if (name == RECTANGLE)
	{
		shape = new Custom::Rectangle();
	}
	else if (name == SQUARE)
	{
		shape = new Custom::Square();
	}
	else if (name == TRIANGLE)
	{
		shape = new Custom::Triangle();
	}
	else if (name == POLYGON)
	{
		shape = new Custom::Polygon();
	}
	else if (name == ELLIPSE)
	{
		shape = new Custom::Ellipse();
	}
	else if (name == CIRCLE)
	{
		shape = new Custom::Circle();
	}
	else
	{
		std::wstringstream ss;
		ss << L"Undefined shape type [" << name << L"]";
		throw DebugException(ss.str());
	}

	return shape;
}