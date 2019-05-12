#pragma once
#include <string>
#include "shapes/BaseShape.h"

class ShapesFactory
{
public:
	static Custom::BaseShape* CreateShape(std::wstring name);

public:
	static constexpr const wchar_t* LINE		= L"Line";
	static constexpr const wchar_t* RECTANGLE	= L"Rectangle";
	static constexpr const wchar_t* SQUARE		= L"Square";
	static constexpr const wchar_t* TRIANGLE	= L"Triangle";
	static constexpr const wchar_t* POLYGON		= L"Polygon";
	static constexpr const wchar_t* ELLIPSE		= L"Ellipse";
	static constexpr const wchar_t* CIRCLE		= L"Circle";
};

