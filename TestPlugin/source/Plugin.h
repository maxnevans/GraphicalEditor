#pragma once
#include "../../PluginSupporter/source/PluginSupporter.h"

class Plugin
	:
	public IPluginLibrary
{
public:
	virtual ShapeFactoryMethod GetShapeFactory();
	virtual std::wstring GetShapeName();
};

