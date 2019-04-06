#pragma once
#include "ISerializable.h"
#include "IDeserializable.h"

class FileManager
{
public:
	static void SaveText(ISerializable* object, std::wstring filename);
	static void LoadText(IDeserializable* object, std::wstring filename);
};

