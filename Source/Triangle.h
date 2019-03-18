#pragma once
#include "Rectangle.h"

namespace Custom {
	class Triangle :
		public Rectangle
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}

