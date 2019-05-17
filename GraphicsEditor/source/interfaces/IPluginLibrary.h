#pragma once
#include "../core/ShapesFactory.h"
#include <string>

class IPluginLibrary
{
public:
	virtual ShapeFactoryMethod GetShapeFactory() = 0;
	virtual std::wstring GetShapeName() = 0;

public:
	static constexpr const char* ENTRY_POINT_NAME = "GetPluginFactory";
};
