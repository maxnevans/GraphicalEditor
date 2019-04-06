#pragma once
#include "BaseShape.h"
namespace Custom {
	class Line :
		public BaseShape
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		virtual std::wstring SerializeText() override;
	};
}

