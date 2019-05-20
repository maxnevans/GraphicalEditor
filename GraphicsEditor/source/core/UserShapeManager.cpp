#include "UserShapeManager.h"
#include "../../../Exceptions/source/Exceptions.h"
#include <windows.h>
#include <assert.h>
#include <shlwapi.h>
#include "../shapes/ComplexShape.h"

UserShapeManager::UserShapeManager(const ShapesFactory* sf, std::wstring directory)
{
	std::wstring searchQuery = directory + L"\\*";
	assert(searchQuery.size() < MAX_PATH);

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(fd));
	HANDLE hUserShapeFile = FindFirstFile(searchQuery.c_str(), &fd);

	if (hUserShapeFile == INVALID_HANDLE_VALUE)
		throw Exception(std::wstring(L"Failed to open user shapes dir: " + directory).c_str());

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
		us.factoryFunctor = ComplexShape::CreateFactoryFunctor(shapes);

		this->userShapes.push_back(us);

	} while (FindNextFile(hUserShapeFile, &fd));
}

const std::vector<UserShape> UserShapeManager::GetUserShapes() const
{
	return this->userShapes;
}
