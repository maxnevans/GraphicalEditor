#pragma once
#include "../DLLSupport.h"
#include <string>

class __dll ISerializable
{
public:
	virtual void SerializeText(std::wstring& text) const = 0;
};

