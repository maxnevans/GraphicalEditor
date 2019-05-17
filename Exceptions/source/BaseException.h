#pragma once
#include "DLLSupport.h"
#include <string>

class __dll IBaseException
{
public:
	virtual const wchar_t* what() const noexcept = 0;
};

class BaseException :
	public IBaseException
{
public:
	__dll BaseException(const wchar_t* message = L"");
	__dll virtual const wchar_t* what() const noexcept;

private:
	std::wstring message;
};
