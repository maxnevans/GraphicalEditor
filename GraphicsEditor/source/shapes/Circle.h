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
			inline virtual BaseShape* operator()() override
			{
				return new Circle();
			}
		};

	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		inline static IShapeFactoryFunctor* CreateFactoryFunctor()
		{
			return new CircleFactoryFunctor();
		}
		inline virtual const wchar_t* GetName() const override
		{
			return this->NAME;
		}

	public:
		static constexpr const wchar_t* NAME = L"Circle";
	};
}


