#pragma once
#include "../DLLSupport.h"
#include <string>

class __dll IDeserializable
{
public:
	virtual void DeserializeText(std::wstring text) = 0;
};

