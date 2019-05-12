#pragma once
#include <vector>
#include "shapes/BaseShape.h"

typedef Custom::BaseShape* (*ShapeFactoryMethod)(void);
typedef unsigned int ShapeID;

class ShapesFactory
{
public:
	Custom::BaseShape* CreateShape(ShapeID shapeId) const;
	ShapeID RegisterShape(ShapeFactoryMethod factoryMethod);

private:
	std::vector<ShapeFactoryMethod> regList;
};

