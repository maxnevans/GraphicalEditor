#pragma once
#include "../../../PluginSupporter/source/PluginSupporter.h"

namespace Custom {
	class Ellipse :
		public ::BaseShape
	{
	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		static ::BaseShape* ShapeFactory();
		virtual const wchar_t* GetName() override;

	public:
		static constexpr const wchar_t* NAME = L"Ellipse";
	};
}

