#pragma once
#include <string>

class ISerializable
{
public:
	virtual std::wstring SerializeText() = 0;
};

