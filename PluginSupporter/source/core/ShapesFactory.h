#pragma once
#include <vector>
#include "../DLLSupport.h"
#include "../shapes/BaseShape.h"
#include <vector>
#include <string>

typedef BaseShape* (*ShapeFactoryMethod)(void);
typedef unsigned int ShapeID;

class ShapesFactory
{
private:
	typedef struct _RegEntry {
		ShapeFactoryMethod fm;
		std::wstring name;
	} RegEntry;

public:
	__dll BaseShape* CreateShape(ShapeID shapeId) const;
	__dll BaseShape* CreateShape(const wchar_t* name) const;
	__dll ShapeID RegisterShape(const wchar_t* name, ShapeFactoryMethod factoryMethod);

private:
	std::vector<RegEntry> regList;
};

