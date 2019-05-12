#pragma once
#include "../ListShapes.h"
#include "../ShapesFactory.h"

class FileManager
{
public:
	static void SaveText(ListShapes* shapes, std::wstring filename);
	static void LoadText(const ShapesFactory* shapesFactory, ListShapes* shapes, std::wstring filename);
};

