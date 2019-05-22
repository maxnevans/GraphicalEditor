#pragma once
#include "Rectangle.h"

namespace Custom {
	class Square :
		public Rectangle
	{
	private:
		class SquareFactoryFunctor
			:
			public IShapeFactoryFunctor
		{
		public:
			inline virtual BaseShape* operator()() override
			{
				return new Square();
			}
		};

	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		inline static IShapeFactoryFunctor* CreateFactoryFunctor()
		{
			return new SquareFactoryFunctor();
		}
		inline virtual const wchar_t* GetName() const override
		{
			return this->NAME;
		}

	public:
		static constexpr const wchar_t* NAME = L"Square";
	};

}
