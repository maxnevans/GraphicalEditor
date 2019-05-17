#include "WinException.h"
#include <sstream>

WinException::WinException(std::wstring message)
	:
	BaseException(message)
{
	this->code = GetLastError();
}

std::wstring WinException::what() noexcept
{
	std::wstringstream ss;
	ss << BaseException::what() << L"with code [" << this->code << "]";
	return ss.str();
}
