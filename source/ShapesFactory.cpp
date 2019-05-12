#include "ShapesFactory.h"
#include <assert.h>

Custom::BaseShape* ShapesFactory::CreateShape(ShapeID shapeId) const
{
	assert(shapeId >= 0);
	assert(shapeId < regList.size());

	return regList[shapeId]();
}

ShapeID ShapesFactory::RegisterShape(ShapeFactoryMethod factoryMethod)
{
	ShapeID ret = regList.size();
	regList.push_back(factoryMethod);
	return ret;
}
