#include "Exception.h"

Exception::~Exception()
{
}

Exception::Exception(std::wstring message)
{
	this->message = message;
}

std::wstring Exception::what()
{
	return this->message;
}
