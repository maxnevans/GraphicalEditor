#pragma once
#include "../ListShapes.h"

class FileManager
{
public:
	static void SaveText(ListShapes* shapes, std::wstring filename);
	static void LoadText(ListShapes* shapes, std::wstring filename);
};

