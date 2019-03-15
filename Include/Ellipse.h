#pragma once
#include "Rectangle.h"
namespace Custom {
	class Ellipse :
		public Rectangle
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}

