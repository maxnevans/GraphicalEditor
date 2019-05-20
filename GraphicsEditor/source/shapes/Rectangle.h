#pragma once
#include "Line.h"

namespace Custom {
	class Rectangle :
		public Line
	{
	private:
		class RectangleFactoryFunctor
			:
			public IShapeFactoryFunctor
		{
		public:
			virtual BaseShape* operator()() override;
		};

	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		static IShapeFactoryFunctor* CreateFactoryFunctor();
		virtual const wchar_t* GetName() override;

	public:
		static constexpr const wchar_t* NAME = L"Rectangle";
	};
}

