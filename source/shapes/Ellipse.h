#pragma once
#include "BaseShape.h"

namespace Custom {
	class Ellipse :
		public BaseShape
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		static BaseShape* ShapeFactory();
		virtual std::wstring GetName() override;

	public:
		static constexpr const wchar_t* NAME = L"Ellipse";
	};
}

