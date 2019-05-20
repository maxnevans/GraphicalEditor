#pragma once
#include "../DLLSupport.h"
#include "../shapes/BaseShape.h"

class __dll IShapeFactoryFunctor
{
public:
	virtual BaseShape* operator()() = 0;
};