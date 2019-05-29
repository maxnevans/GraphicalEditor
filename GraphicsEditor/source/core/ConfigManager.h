#pragma once
#include <string>
#include <windows.h>
#include <gdiplus.h>

#define CONFIG_MANAGER_DEFAULT_CONFIG_NAME	L"config.xml"
#define CONFIG_MANAGER_MIN_VERSION			L"1.0.0"
#define CONFIG_MANAGER_CURRENT_VERSION		L"1.0.0"

class ConfigManager
{
public:
	typedef struct _Config
	{
		typedef struct _Color
		{
			Gdiplus::ARGB background;
			Gdiplus::ARGB shape;
			Gdiplus::ARGB selection;
		} Color;

		typedef struct _Dir
		{
			std::wstring working;
			std::wstring userShapes;
			std::wstring plugins;
		} Directory;

		std::wstring configVersion;
		std::wstring configName;
		Color color;
		Directory directory;

	} Config;

public:
	ConfigManager(std::wstring directory);
	inline const Config* GetConfig() const
	{
		return &this->config;
	}
	void CreateConfig() const;

private:
	static const Config DEFAULT_CONFIG;
	
	std::wstring directory;
	Config config;
};

