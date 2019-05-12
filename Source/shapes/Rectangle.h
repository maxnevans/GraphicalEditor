#pragma once
#include "Line.h"

namespace Custom {
	class Rectangle :
		public Line
	{
	public:
		Rectangle();
		Rectangle(const wchar_t* name);
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}

