#pragma once
#include <string>
#include "../DLLSupport.h"

class __dll ISerializable
{
public:
	virtual const wchar_t* SerializeText() = 0;
};

