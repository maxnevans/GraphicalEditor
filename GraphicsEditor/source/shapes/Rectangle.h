#pragma once
#include "Line.h"

namespace Custom {
	class Rectangle :
		public Line
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		static ::BaseShape* ShapeFactory();
		virtual const wchar_t* GetName() override;

	public:
		static constexpr const wchar_t* NAME = L"Rectangle";
	};
}

