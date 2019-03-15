#pragma once
#include "Ellipse.h"
namespace Custom {
	class Circle :
		public Ellipse
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}


