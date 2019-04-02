#pragma once
class ISerializable
{
public:
	virtual const wchar_t* SerializeText() = 0;
};

