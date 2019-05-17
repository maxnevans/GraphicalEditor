#pragma once
#include "Rectangle.h"

namespace Custom {
	class Square :
		public Rectangle
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		static ::BaseShape* ShapeFactory();
		virtual const wchar_t* GetName() override;

	public:
		static constexpr const wchar_t* NAME = L"Square";
	};

}
