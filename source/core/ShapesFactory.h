#pragma once
#include <vector>
#include "../shapes/BaseShape.h"

typedef Custom::BaseShape* (*ShapeFactoryMethod)(void);
typedef unsigned int ShapeID;

class ShapesFactory
{
private:
	typedef struct _RegEntry {
		ShapeFactoryMethod fm;
		std::wstring name;
	} RegEntry;
public:
	Custom::BaseShape* CreateShape(ShapeID shapeId) const;
	Custom::BaseShape* CreateShape(std::wstring name) const;
	ShapeID RegisterShape(const std::wstring name, ShapeFactoryMethod factoryMethod);

private:
	std::vector<RegEntry> regList;
};

