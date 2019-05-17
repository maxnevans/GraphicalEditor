#include "WinException.h"
#include <sstream>

WinException::WinException(std::wstring message)
	:
	Exception(message)
{
	this->code = GetLastError();
}

std::wstring WinException::what() noexcept
{
	std::wstringstream ss;
	ss << Exception::what() << L"with code [" << this->code << "]";
	return ss.str();
}
