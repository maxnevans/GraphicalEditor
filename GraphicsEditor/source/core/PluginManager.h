#pragma once
#include <string>
#include <vector>
#include "ShapesFactory.h"
#include "../interfaces/IPluginLibrary.h"

#define PLUGIN_EXTENSION	L".dll"

typedef struct _CustomPlugin
{
	std::wstring name;
	ShapeFactoryMethod factory;
}  CustomPlugin, * PCustomPlugin;

class PluginManager
{
private:
	typedef IPluginLibrary* (*PluginLibraryFactory)(void);

public:
	PluginManager(std::wstring directory);
	const std::vector<CustomPlugin> GetPlugins() const;
	~PluginManager();

private:
	std::vector<CustomPlugin> plugins;
	std::vector<HMODULE> hLibsLoaded;
};