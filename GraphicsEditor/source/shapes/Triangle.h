#pragma once
#include "Rectangle.h"

namespace Custom {
	class Triangle :
		public Line
	{
	private:
		class TriangleFactoryFunctor
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
		static constexpr const wchar_t* NAME = L"Triangle";
	};
}

