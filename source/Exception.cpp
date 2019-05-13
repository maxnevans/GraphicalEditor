#include "Exception.h"
#include <sstream>

_Exception::_Exception(std::wstring message)
{
	this->message = message;
	this->line = -1;
	this->filename = L"";
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
