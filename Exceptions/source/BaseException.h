#pragma once
#include "DLLSupport.h"
#include <string>

class __dll BaseException
{
public:
	BaseException(std::wstring message = L"");
	virtual std::wstring what() noexcept;
private:

	std::wstring message;
	int line;
	std::wstring filename;
};
