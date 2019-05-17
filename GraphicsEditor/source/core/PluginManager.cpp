#include "PluginManager.h"
#include "../Exception.h"
#include <windows.h>
#include <assert.h>
#include <shlwapi.h>

PluginManager::PluginManager(std::wstring directory)
{
	std::wstring searchQuery = directory + L"\\*";
	assert(searchQuery.size() < MAX_PATH);

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(fd));
	HANDLE hPlugin = FindFirstFile(searchQuery.c_str(), &fd);

	if (hPlugin == INVALID_HANDLE_VALUE)
		throw Exception(L"Failed to open plugin dir: " + directory);

	do
	{
		std::wstring fileName = PathFindFileName(fd.cFileName);
		std::wstring fileExtension = PathFindExtension(fd.cFileName);
		
		if (fileExtension != PLUGIN_EXTENSION)
			continue;

		std::wstring libPath = directory + L"\\" + fileName;
		HMODULE hDllPlugin = LoadLibrary(libPath.c_str());
		if (!hDllPlugin)
			throw Exception(L"Failed to load plugin dll!");

		this->hLibsLoaded.push_back(hDllPlugin);

		PluginLibraryFactory plf = reinterpret_cast<PluginLibraryFactory>(GetProcAddress(hDllPlugin, IPluginLibrary::ENTRY_POINT_NAME));
		if (!plf)
			throw Exception(L"Failed to get plugin librarys entry point address");

		IPluginLibrary* pluginLib = plf();

		CustomPlugin cp;
		ZeroMemory(&cp, sizeof(cp));
		cp.factory = pluginLib->GetShapeFactory();
		cp.name = pluginLib->GetShapeName();

		this->plugins.push_back(cp);

		delete pluginLib;

	} while (FindNextFile(hPlugin, &fd));
}

const std::vector<CustomPlugin> PluginManager::GetPlugins() const
{
	return this->plugins;
}

PluginManager::~PluginManager()
{
	this->plugins.clear();

	for (HMODULE hLib : this->hLibsLoaded)
		FreeLibrary(hLib);
}
