#include "FileManager.h"
#include "Exception.h"
#include <fstream>
#include <sstream>

void FileManager::SaveText(ISerializable * object, std::wstring filename)
{
	std::wofstream file(filename);

	if (!file.is_open())
	{
		std::wstringstream ss;
		ss << L"Failed to open file [" << filename << L"]";
		throw Exception(ss.str());
	}

	file << object->SerializeText();
	file.flush();
}

void FileManager::LoadText(IDeserializable * object, std::wstring filename)
{
	std::wifstream file(filename);

	if (!file.is_open())
	{
		std::wstringstream ss;
		ss << L"Failed to open file [" << filename << L"]";
		throw Exception(ss.str());
	}

	std::wstring buffer;
	for (wchar_t c = file.get(); file.good(); c = file.get())
	{
		buffer.push_back(c);
	}

	object->DeserializeText(buffer);

}
