#pragma once
#include "Line.h"

namespace Custom {
	class Rectangle :
		public Line
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		static BaseShape* ShapeFactory();
		virtual std::wstring GetName() override;

	public:
		static constexpr const wchar_t* NAME = L"Rectangle";
	};
}
