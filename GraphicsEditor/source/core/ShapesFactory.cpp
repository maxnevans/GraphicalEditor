#include "ShapesFactory.h"
#include "../Exception.h"
#include <assert.h>

Custom::BaseShape* ShapesFactory::CreateShape(ShapeID shapeId) const
{
	assert(shapeId >= 0);
	assert(shapeId < regList.size());

	return regList[shapeId].fm();
}

Custom::BaseShape* ShapesFactory::CreateShape(std::wstring name) const
{
	for (RegEntry re : regList)
		if (re.name == name)
			return re.fm();

	throw Exception(L"Trying to create undefined shape: { " + name + L" }");
	return nullptr;
}

ShapeID ShapesFactory::RegisterShape(const std::wstring name, ShapeFactoryMethod factoryMethod)
{
	ShapeID ret = static_cast<ShapeID>(regList.size());
	RegEntry re;
	re.fm = factoryMethod;
	re.name = name;
	regList.push_back(re);
	return ret;
}
