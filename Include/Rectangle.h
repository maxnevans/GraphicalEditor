#pragma once
#include "Line.h"

namespace Custom {
	class Rectangle :
		public Line
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}

