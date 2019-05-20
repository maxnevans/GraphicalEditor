#pragma once
#include "FileManager.h"
#include "../../../PluginSupporter/source/PluginSupporter.h"
#include <string>

#define USER_SHAPES_EXTENSION	L".ushp"

typedef struct _UserShape
{
	std::wstring name;
	IShapeFactoryFunctor* factoryFunctor;
} UserShape;

class UserShapeManager
{
public:
	UserShapeManager(const ShapesFactory* sf, std::wstring directory);
	const std::vector<UserShape> GetUserShapes() const;

private:
	std::vector<UserShape> userShapes;
};

