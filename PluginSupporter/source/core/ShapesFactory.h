#pragma once
#include <vector>
#include <string>
#include "../DLLSupport.h"
#include "../shapes/BaseShape.h"
#include "IShapeFactoryFunctor.h"

typedef unsigned int ShapeID;

class ShapesFactory
{
private:
	typedef struct _RegEntry {
		IShapeFactoryFunctor* fm;
		std::wstring name;
	} RegEntry;

public:
	__dll BaseShape* CreateShape(ShapeID shapeId) const;
	__dll BaseShape* CreateShape(const wchar_t* name) const;
	__dll ShapeID RegisterShape(const wchar_t* name, IShapeFactoryFunctor* func);
	__dll ~ShapesFactory();

private:
	std::vector<RegEntry> regList;
};

