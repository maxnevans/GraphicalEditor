#pragma once
#include <string>
#include "../DLLSupport.h"

class __dll IDeserializable
{
public:
	virtual void DeserializeText(std::wstring text) = 0;
};

