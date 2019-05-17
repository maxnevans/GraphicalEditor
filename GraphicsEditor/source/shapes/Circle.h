#pragma once
#include "Ellipse.h"

namespace Custom {
	class Circle :
		public Ellipse
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		static BaseShape* ShapeFactory();
		virtual std::wstring GetName() override;

	public:
		static constexpr const wchar_t* NAME = L"Circle";
	};
}


