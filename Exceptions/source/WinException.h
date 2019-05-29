#pragma once
#include "DLLSupport.h"
#include "BaseException.h"
#include "LightWindows.h"

class WinException :
	public BaseException
{
public:
	__dll WinException(const wchar_t* message = L"");
	__dll virtual const wchar_t* what() const noexcept override;

private:
	unsigned long code;
	std::wstring message;
};

