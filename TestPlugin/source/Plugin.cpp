#include "Plugin.h"
#include "TestShape.h"

ShapeFactoryMethod Plugin::GetShapeFactory()
{
	return TestShape::ShapeFactory;
}

const wchar_t* Plugin::GetShapeName()
{
	return TestShape::NAME;
}
