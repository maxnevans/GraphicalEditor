#pragma once
#include "BaseShape.h"

namespace Custom {
	class Ellipse :
		public BaseShape
	{
	public:
		Ellipse();
		Ellipse(const wchar_t* name);
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}

