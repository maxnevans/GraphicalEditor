#pragma once
#include "FileManager.h"
#include "../../../PluginSupporter/source/PluginSupporter.h"
#include <string>

#define USER_SHAPES_EXTENSION	L".ushp"
#define METADATA_LINES_COUNT	1

typedef struct _UserShape
{
	std::wstring name;
	IShapeFactoryFunctor* factoryFunctor;
} UserShape;

class UserShapeManager
{
private:
	struct MetaData
	{
		std::wstring shapeName;
	};
public:
	UserShapeManager(const ShapesFactory* sf, std::wstring directory);
	void SaveUserShape(std::wstring fileName, std::wstring shapeName, const std::vector<const BaseShape*>& shapes) const;
	inline const std::vector<UserShape> GetUserShapes() const
	{
		return this->userShapes;
	}

private:
	static void NormalizeShapes(std::vector<BaseShape*>& shapes);
	void WriteMetaToFile(std::wstring fileName, MetaData& meta) const;
	void ReadMetaFromFile(std::wstring fileName, MetaData& meta);

private:
	std::vector<UserShape> userShapes;
	std::wstring directory;
};

