#pragma once
#include <string>
#include "../DLLSupport.h"

class __dll ISerializable
{
public:
	virtual std::wstring SerializeText() = 0;
};

