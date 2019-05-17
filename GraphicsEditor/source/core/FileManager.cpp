#include "FileManager.h"
#include "../../../Exceptions/source/Exceptions.h"
#include <fstream>
#include <sstream>

void FileManager::SaveText(const std::vector<BaseShape*>* shapes, std::wstring filename)
{
	std::wofstream file(filename);

	if (!file.is_open())
	{
		std::wstringstream ss;
		ss << L"Failed to open file [" << filename << L"]";
		throw Exception(ss.str().c_str());
	}

	for (BaseShape* shape : *shapes)
		file << shape->SerializeText() << std::endl;

	file.close();
}

void FileManager::LoadText(const ShapesFactory* shapesFactory, 
	std::vector<BaseShape*>* shapes, std::wstring filename)
{
	std::wifstream file(filename);

	if (!file.is_open())
	{
		std::wstringstream ss;
		ss << L"Failed to open file [" << filename << L"]";
		throw Exception(ss.str().c_str());
	}

	std::wstringstream buffer;
	for (wchar_t c = file.get(); file.good(); c = file.get())
	{
		buffer << c;
	}

	std::wstring shapeText;
	std::getline(buffer, shapeText);
	while (buffer.good())
	{

		size_t shapeTextStart = shapeText.find(L"Coordinates:");
		size_t endOfShapeName = shapeTextStart - 1;
		std::wstring shapeName = shapeText.substr(0, endOfShapeName);

		BaseShape* shape = shapesFactory->CreateShape(shapeName.c_str());
		shape->DeserializeText(shapeText.substr(shapeTextStart).c_str());

		shapes->push_back(shape);

		std::getline(buffer, shapeText);
	}

}
