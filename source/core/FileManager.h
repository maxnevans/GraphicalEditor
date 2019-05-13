#pragma once
#include "ShapesFactory.h"
#include <vector>

class FileManager
{
public:
	static void SaveText(const std::vector<Custom::BaseShape*>* shapes, std::wstring filename);
	static void LoadText(const ShapesFactory* shapesFactory, std::vector<Custom::BaseShape*>* shapes, std::wstring filename);
};

