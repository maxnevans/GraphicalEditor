#include "UserShapeManager.h"
#include "../../../Exceptions/source/Exceptions.h"
#include <windows.h>
#include <assert.h>
#include <shlwapi.h>
#include "../shapes/ComplexShape.h"
#include <filesystem>

UserShapeManager::UserShapeManager(const ShapesFactory* sf, std::wstring directory)
{
	std::wstring searchQuery = directory + L"\\*";
	assert(searchQuery.size() < MAX_PATH);

	this->directory = directory;

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(fd));
	HANDLE hUserShapeFile = FindFirstFile(searchQuery.c_str(), &fd);

	if (hUserShapeFile == INVALID_HANDLE_VALUE)
		return;

	do
	{
		std::wstring fileName = PathFindFileName(fd.cFileName);
		std::wstring fileExtension = PathFindExtension(fd.cFileName);

		if (fileExtension != USER_SHAPES_EXTENSION)
			continue;

		std::wstring userShapeFilePath = directory + L"\\" + fileName;
		
		std::vector<BaseShape*> shapes;
		FileManager::LoadText(sf, &shapes, userShapeFilePath);

		UserShape us;
		ZeroMemory(&us, sizeof(us));
		us.name = fileName;
		us.factoryFunctor = ComplexShape::CreateFactoryFunctor(us.name, 
			std::vector<const BaseShape*>(shapes.begin(), shapes.end()));

		this->userShapes.push_back(us);

	} while (FindNextFile(hUserShapeFile, &fd));

	FindClose(hUserShapeFile);
}

void UserShapeManager::SaveUserShape(std::wstring fileName, std::wstring shapeName, const std::vector<const BaseShape*>& shapes) const
{
	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(fd));
	HANDLE hDirectory = FindFirstFile(this->directory.c_str(), &fd);
	if (hDirectory == INVALID_HANDLE_VALUE)
		std::filesystem::create_directory(this->directory.c_str());

	ZeroMemory(&fd, sizeof(fd));
	hDirectory = FindFirstFile(this->directory.c_str(), &fd);
	if (hDirectory == INVALID_HANDLE_VALUE)
		throw Exception(std::wstring(L"Failed to create UserShape directory: " + this->directory).c_str());

	FindClose(hDirectory);

	std::vector<BaseShape*> shapesToNormalize;
	for (const BaseShape* shape : shapes)
		shapesToNormalize.push_back(shape->Clone());
	this->NormalizeShapes(shapesToNormalize);

	std::vector<const BaseShape*> normalized;
	for (BaseShape* normal : shapesToNormalize)
		normalized.push_back(normal);

	FileManager::SaveText(&normalized, this->directory + L"\\" + fileName + USER_SHAPES_EXTENSION);

	for (const BaseShape* normal : shapesToNormalize)
		delete normal;
}

void UserShapeManager::NormalizeShapes(std::vector<BaseShape*>& shapes)
{
	assert(shapes.size() > 0);
	int minX = shapes[0]->GetX();
	int minY = shapes[0]->GetY();

	for (const BaseShape* shape : shapes)
	{
		minX = minX < shape->GetX() ? minX : shape->GetX();
		minY = minY < shape->GetY() ? minY : shape->GetY();
	}

	for (BaseShape* shape : shapes)
	{
		int x1, y1, x2, y2;
		shape->GetPoints(x1, y1, x2, y2);

		x1 -= minX;
		x2 -= minX;
		y1 -= minY;
		y2 -= minY;

		shape->SetPoints(x1, y1, x2, y2);
	}
}
