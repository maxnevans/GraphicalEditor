#include "WinException.h"
#include <sstream>

WinException::WinException(const wchar_t* message)
	:
	BaseException(message)
{
	this->code = GetLastError();
}

const wchar_t* WinException::what() const noexcept
{
	std::wstringstream ss;
	ss << BaseException::what() << L"with code [" << this->code << "]";
	return ss.str().c_str();
}
