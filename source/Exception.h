#pragma once
#include <string>

class _Exception
{
public:
	_Exception(std::wstring message = L"");
	virtual std::wstring what() noexcept;
private:

	std::wstring message;
	int line;
	std::wstring filename;
};

typedef _Exception Exception;
