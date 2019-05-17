#pragma once
#include "DLLSupport.h"
#include "BaseException.h"
#include "LightWindows.h"

class __dll WinException :
	public BaseException
{
public:
	WinException(std::wstring message = L"");
	virtual std::wstring what() noexcept override;

private:
	unsigned long code;
};

