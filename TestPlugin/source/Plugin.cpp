#include "Plugin.h"
#include "TestShape.h"

ShapeFactoryMethod Plugin::GetShapeFactory()
{
	return TestShape::ShapeFactory;
}

std::wstring Plugin::GetShapeName()
{
	return TestShape::NAME;
}
