#pragma once
#include "Rectangle.h"

namespace Custom {
	class Square :
		public Rectangle
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};

}
