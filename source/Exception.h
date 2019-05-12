#pragma once
#include <string>
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define DebugException(message) _Exception(message, __LINE__, WIDE1(__FILE__))

class _Exception
{
public:
	_Exception(std::wstring message = L"");
	_Exception(std::wstring message, int line, std::wstring filename);
	virtual std::wstring what() noexcept;
private:

	std::wstring message;
	int line;
	std::wstring filename;
};

typedef _Exception Exception;
