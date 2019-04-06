#include "ShapesFactory.h"
#include <sstream>
#include "Exception.h"
#include "Line.h"
#include "Rectangle.h"
#include "Square.h"
#include "Triangle.h"
#include "Polygon.h"
#include "Ellipse.h"
#include "Circle.h"

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