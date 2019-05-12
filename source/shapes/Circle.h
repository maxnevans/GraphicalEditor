#pragma once
#include "Ellipse.h"

namespace Custom {
	class Circle :
		public Ellipse
	{
	public:
		Circle();
		Circle(const wchar_t* name);
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}


