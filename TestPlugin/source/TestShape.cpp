#include "TestShape.h"
#include <string>

void TestShape::Redraw(Gdiplus::Graphics* const graphics)
{
	using namespace Gdiplus;
	std::wstring test = L"Test Shape";
	Gdiplus::FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, 40, FontStyleRegular, UnitPixel);
	RectF layout;
	layout.X = static_cast<float>(this->x);
	layout.Y = static_cast<float>(this->y);
	layout.Width = static_cast<float>(this->width);
	layout.Height = static_cast<float>(this->height);
	Color color(this->color);
	SolidBrush brush(color);
	Pen pen(color);
	StringFormat sf;
	sf.SetAlignment(StringAlignment::StringAlignmentCenter);
	graphics->DrawRectangle(&pen, layout);
	graphics->DrawString(test.c_str(), -1, &font, layout, &sf, &brush);
}

::BaseShape* TestShape::ShapeFactory()
{
	return new TestShape();
}

const wchar_t* TestShape::GetName()
{
	return this->NAME;
}
