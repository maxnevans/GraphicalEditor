#pragma once
#include <string>

class Exception
{
public:
	Exception(std::wstring message = L"");
	virtual std::wstring what() noexcept;
private:

	std::wstring message;
};

