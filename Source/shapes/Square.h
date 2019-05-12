#pragma once
#include "Rectangle.h"

namespace Custom {
	class Square :
		public Rectangle
	{
	public:
		Square();
		Square(const wchar_t* name);
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};

}
