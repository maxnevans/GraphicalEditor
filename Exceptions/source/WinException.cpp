#include "WinException.h"
#include <sstream>

WinException::WinException(const wchar_t* message)
	:
	BaseException(message)
{
	this->code = GetLastError();

	std::wstringstream ss;
	ss << BaseException::what() << L" with code [" << this->code << "]";
	this->message = std::wstring(ss.str());
}

const wchar_t* WinException::what() const noexcept
{
	return this->message.c_str();
}
