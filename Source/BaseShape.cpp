#include "BaseShape.h"
#include "Exception.h"
#include <sstream>
#include <string>

Custom::BaseShape::BaseShape(const wchar_t* name)
	:
	name(name)
{
}

void Custom::BaseShape::SelectPoint(int x, int y)
{
}

void Custom::BaseShape::SelectArea(int x1, int y1, int x2, int y2)
{
	this->SetPointsSafe(x1, y1, x2, y2);
}

void Custom::BaseShape::SetPoints(int x1, int y1, int x2, int y2)
{
	this->SetPointsSafe(x1, y1, x2, y2);
}

void Custom::BaseShape::SetColor(Gdiplus::Color color)
{
	this->color = color;
}

std::wstring Custom::BaseShape::SerializeText()
{
	std::wstringstream ss;
	ss << this->name << " ";
	ss << "Coordinates: (" << this->x1 << ", " << this->y1 << ") (" << this->x2 << ", " << this->y2 << ") Color: " << this->color.GetValue();
	return ss.str();
}

void Custom::BaseShape::DeserializeText(std::wstring text)
{
	std::wstringstream ss(text);

	ss.ignore(wcslen(L"Coordinates: ("));
	ss >> this->x1;
	ss.ignore(2);
	ss >> this->y1;
	ss.ignore(wcslen(L") ("));
	ss >> this->x2;
	ss.ignore(2);
	ss >> this->y2;
	ss.ignore(wcslen(L") Color: "));

	Gdiplus::ARGB colorValue;
	ss >> colorValue;
	this->color.SetValue(colorValue);

	if (ss.fail()) throw Exception(L"undefined text format");

	this->x = this->x1 < this->x2 ? this->x1 : this->x2;
	this->y = this->y1 < this->y2 ? this->y1 : this->y2;
	this->width = this->x1 < this->x2 ? this->x2 - this->x1 : this->x1 - this->x2;
	this->height = this->y1 < this->y2 ? this->y2 - this->y1 : this->y1 - this->y2;
}

void Custom::BaseShape::SetPointsSafe(int x1, int y1, int x2, int y2)
{
	this->x = x1 < x2 ? x1 : x2;
	this->y = y1 < y2 ? y1 : y2;
	this->width = x1 < x2 ? x2 - x1 : x1 - x2;
	this->height = y1 < y2 ? y2 - y1 : y1 - y2;
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}
