#pragma once
#include "../DLLSupport.h"

class __dll IDeserializable
{
public:
	virtual void DeserializeText(const wchar_t* text) = 0;
};

