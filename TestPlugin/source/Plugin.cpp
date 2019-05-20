#include "Plugin.h"
#include "TestShape.h"

IShapeFactoryFunctor* Plugin::GetShapeFactory()
{
	return TestShape::CreateFactoryFunctor();
}

const wchar_t* Plugin::GetShapeName()
{
	return TestShape::NAME;
}
