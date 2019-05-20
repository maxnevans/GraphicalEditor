#pragma once
#include "../../../PluginSupporter/source/core/ShapesFactory.h"
#include <string>
#include <vector>

class FileManager
{
public:
	static void SaveText(const std::vector<BaseShape*>* shapes, std::wstring filename);
	static void LoadText(const ShapesFactory* shapesFactory, std::vector<BaseShape*>* shapes, std::wstring filename);
};

