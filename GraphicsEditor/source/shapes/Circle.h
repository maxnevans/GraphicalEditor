#pragma once
#include "Ellipse.h"

namespace Custom {
	class Circle :
		public Ellipse
	{
	private:
		class CircleFactoryFunctor
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
		static constexpr const wchar_t* NAME = L"Circle";
	};
}


