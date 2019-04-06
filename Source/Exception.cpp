#include "Exception.h"
#include <sstream>

_Exception::_Exception(std::wstring message)
{
	this->message = message;
	this->line = -1;
	this->filename = L"";
}

_Exception::_Exception(std::wstring message, int line, std::wstring filename)
{
	this->message = message;
	this->line = line;
	this->filename = filename;
}

std::wstring _Exception::what() noexcept
{
	std::wstringstream ss;
	ss << this->message;
	if (this->line != -1)
	{
		ss << L" on line " << this->line << L" in file " << this->filename;
	}
	return ss.str();
}
