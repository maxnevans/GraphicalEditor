#define DLLEXPORT extern "C" __declspec(dllexport)
#include "Plugin.h"

DLLEXPORT IPluginLibrary* GetPluginFactory()
{
	return new Plugin();
}