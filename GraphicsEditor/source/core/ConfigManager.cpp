#include "ConfigManager.h"
#include "../../../libs/pugixml/src/pugixml.hpp"
#include "../../../Exceptions/source/Exceptions.h"
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

const ConfigManager::Config ConfigManager::DEFAULT_CONFIG = {
	.configVersion = CONFIG_MANAGER_MIN_VERSION,
	.configName = L"global",
	.color = {
		.background = Gdiplus::ARGB(0xFF1A2856),
		.shape = Gdiplus::ARGB(0xFFFFFFFF),
		.selection = Gdiplus::ARGB(0xFF4D77FF)
	},
	.directory = {
		.working = L"./",
		.userShapes = L"user",
		.plugins = L"plugins"
	}
};

ConfigManager::ConfigManager(std::wstring directory)
{
	std::wstring fullPath = directory + L"\\" + CONFIG_MANAGER_DEFAULT_CONFIG_NAME;
	assert(fullPath.size() < MAX_PATH);
	this->directory = directory;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fullPath.c_str());

	if (result.status != pugi::xml_parse_status::status_ok && result.status != pugi::xml_parse_status::status_file_not_found)
		throw Exception(L"config manager failed on xml parser startup");

	this->config = ConfigManager::DEFAULT_CONFIG;

	if (result.status == pugi::xml_parse_status::status_ok)
	{
		auto settingsNode = doc.child(L"config");
		this->config.configVersion = settingsNode.attribute(L"version").as_string(ConfigManager::DEFAULT_CONFIG.configVersion.c_str());
		this->config.configName = settingsNode.attribute(L"name").as_string(ConfigManager::DEFAULT_CONFIG.configName.c_str());

		auto colorNode = settingsNode.child(L"color");
		this->config.color.background = colorNode.child(L"background").text().as_uint(ConfigManager::DEFAULT_CONFIG.color.background);
		this->config.color.shape = colorNode.child(L"shape").text().as_uint(ConfigManager::DEFAULT_CONFIG.color.shape);
		this->config.color.selection = colorNode.child(L"selection").text().as_uint(ConfigManager::DEFAULT_CONFIG.color.selection);

		auto dirNode = settingsNode.child(L"directory");
		this->config.directory.working = dirNode.child(L"working").text().as_string(ConfigManager::DEFAULT_CONFIG.directory.working.c_str());
		this->config.directory.userShapes = dirNode.child(L"user-shapes").text().as_string(ConfigManager::DEFAULT_CONFIG.directory.userShapes.c_str());
		this->config.directory.plugins = dirNode.child(L"plugins").text().as_string(ConfigManager::DEFAULT_CONFIG.directory.plugins.c_str());
	}
	else if (result.status == pugi::xml_parse_status::status_file_not_found)
		this->CreateConfig();
}

void ConfigManager::CreateConfig() const
{
	std::wstring fullPath = this->directory + L"\\" + CONFIG_MANAGER_DEFAULT_CONFIG_NAME;
	pugi::xml_document doc;

	auto configNode = doc.append_child(L"config");
	configNode.append_attribute(L"version").set_value(CONFIG_MANAGER_CURRENT_VERSION);
	configNode.append_attribute(L"name").set_value(ConfigManager::DEFAULT_CONFIG.configName.c_str());
	
	auto colorNode = configNode.append_child(L"color");
	colorNode.append_child(L"background").text().set(ConfigManager::DEFAULT_CONFIG.color.background);
	colorNode.append_child(L"shape").text().set(ConfigManager::DEFAULT_CONFIG.color.shape);
	colorNode.append_child(L"selection").text().set(ConfigManager::DEFAULT_CONFIG.color.selection);

	auto dirNode = configNode.append_child(L"directory");
	dirNode.append_child(L"working").text().set(ConfigManager::DEFAULT_CONFIG.directory.working.c_str());
	dirNode.append_child(L"user-shapes").text().set(ConfigManager::DEFAULT_CONFIG.directory.userShapes.c_str());
	dirNode.append_child(L"plugins").text().set(ConfigManager::DEFAULT_CONFIG.directory.plugins.c_str());

	doc.save_file(fullPath.c_str());
}
