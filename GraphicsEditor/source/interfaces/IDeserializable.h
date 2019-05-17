#pragma once
#include <string>

class IDeserializable
{
public:
	virtual void DeserializeText(std::wstring text) = 0;
};

