#pragma once
#include "../../PluginSupporter/source/PluginSupporter.h"

class Plugin
	:
	public IPluginLibrary
{
public:
	virtual IShapeFactoryFunctor* GetShapeFactory() override;
	virtual const wchar_t* GetShapeName() override;
};

