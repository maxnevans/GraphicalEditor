#include "FileManager.h"
#include <stdio.h>
#include <exception>

void FileManager::SaveText(ISerializable * object, const wchar_t* filename)
{
	FILE* file;
	int error = _wfopen_s(&file, filename, L"w");
	if (error) throw std::exception("file open failed");
	fputws(object->SerializeText(), file);
	fclose(file);
}

void FileManager::LoadText(IDeserializable * object, const wchar_t* filename)
{
	FILE* file;
	int error = _wfopen_s(&file, filename, L"r");
	if (error) throw std::exception("file does not exists or any other fail occured");

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	rewind(file);

	wchar_t* buffer = (wchar_t*)malloc(fileSize);
	if (!buffer) throw std::exception("malloc fail");
	try
	{
		fgetws(buffer, fileSize / 2, file);
		fclose(file);
		object->DeserializeText(buffer);
	}
	catch (...)
	{
		free(buffer);
		throw;
	}

	free(buffer);
}
