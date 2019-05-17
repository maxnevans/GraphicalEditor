#include "TestShape.h"
#include <string>

void TestShape::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;
	std::wstring test = L"Test Shape";
	Gdiplus::FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, 16, FontStyleRegular, UnitPixel);
	PointF origin;
	origin.X = static_cast<float>(this->x);
	origin.Y = static_cast<float>(this->y);
	Gdiplus::SolidBrush brush(Gdiplus::Color::Red);
	graphics->DrawString(test.c_str(), -1,  &font, origin, &brush);
}

::BaseShape* TestShape::ShapeFactory()
{
	return new TestShape();
}

std::wstring TestShape::GetName()
{
	return this->NAME;
}
