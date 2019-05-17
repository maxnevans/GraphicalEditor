#pragma once
#include <vector>
#include "../DLLSupport.h"
#include "../shapes/BaseShape.h"

typedef BaseShape* (*ShapeFactoryMethod)(void);
typedef unsigned int ShapeID;

class __dll ShapesFactory
{
private:
	typedef struct _RegEntry {
		ShapeFactoryMethod fm;
		std::wstring name;
	} RegEntry;
public:
	BaseShape* CreateShape(ShapeID shapeId) const;
	BaseShape* CreateShape(std::wstring name) const;
	ShapeID RegisterShape(const std::wstring name, ShapeFactoryMethod factoryMethod);

private:
	std::vector<RegEntry> regList;
};

