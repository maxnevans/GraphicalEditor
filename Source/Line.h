#pragma once
#include "BaseShape.h"
namespace Custom {
	class Line :
		public BaseShape
	{
	public:
		Line();
		Line(const wchar_t* name);
		virtual void Redraw(Gdiplus::Graphics* const graphics);
	};
}

