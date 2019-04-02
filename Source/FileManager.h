#pragma once
#include "ISerializable.h"
#include "IDeserializable.h"

class FileManager
{
public:
	static void SaveText(ISerializable* object, const wchar_t* filename);
	static void LoadText(IDeserializable* object, const wchar_t* filename);
};

