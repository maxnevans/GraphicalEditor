#include "FileManager.h"
#include "../Exception.h"
#include "../ShapesFactory.h"
#include <fstream>
#include <sstream>

void FileManager::SaveText(ListShapes* shapes, std::wstring filename)
{
	std::wofstream file(filename);

	if (!file.is_open())
	{
		std::wstringstream ss;
		ss << L"Failed to open file [" << filename << L"]";
		throw Exception(ss.str());
	}

	ListShapes* temp = new ListShapes;
	while (!shapes->IsEmpty())
	{
		Custom::BaseShape* shape = shapes->Pop();
		file << shape->SerializeText() << std::endl;
		temp->Push(shape);
	}

	while (!temp->IsEmpty()) shapes->Push(temp->Pop());
	delete temp;

	file.close();
}

void FileManager::LoadText(ListShapes* shapes, std::wstring filename)
{
	std::wifstream file(filename);

	if (!file.is_open())
	{
		std::wstringstream ss;
		ss << L"Failed to open file [" << filename << L"]";
		throw Exception(ss.str());
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

		Custom::BaseShape* shape = ShapesFactory::CreateShape(shapeName);
		shape->DeserializeText(shapeText.substr(shapeTextStart));

		shapes->Push(shape);

		std::getline(buffer, shapeText);
	}

}
