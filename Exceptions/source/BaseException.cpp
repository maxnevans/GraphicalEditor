#include "BaseException.h"
#include <sstream>
#include <string.h>
#include <stdlib.h>

BaseException::BaseException(const wchar_t* message)
{
	this->message = message;
}

const wchar_t* BaseException::what() const noexcept
{
	return this->message.c_str();
}
