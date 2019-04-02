#pragma once
class IDeserializable
{
public:
	virtual void DeserializeText(const wchar_t* text) = 0;
};

