#pragma once
#include "Rectangle.h"

namespace Custom {
	class Triangle :
		public Line
	{
	public:
		Triangle();
		Triangle(const wchar_t* name);
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}

