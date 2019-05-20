#pragma once
#include "../DLLSupport.h"
#include "../core/ShapesFactory.h"

class __dll IPluginLibrary
{
public:
	virtual IShapeFactoryFunctor* GetShapeFactory() = 0;
	virtual const wchar_t* GetShapeName() = 0;

public:
	static constexpr const char* ENTRY_POINT_NAME = "GetPluginFactory";
};

