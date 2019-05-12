#pragma once
#include "Exception.h"
#include <windows.h>

class WinException :
	public Exception
{
public:
	WinException(std::wstring message = L"");
	virtual std::wstring what() noexcept override;

private:
	DWORD code;
};

