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
			inline virtual BaseShape* operator()() override
			{
				return new Rectangle();
			}
		};

	public:
		inline virtual BaseShape* Clone() const override
		{
			return new Rectangle(*this);
		}
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		inline static IShapeFactoryFunctor* CreateFactoryFunctor()
		{
			return new RectangleFactoryFunctor();
		}
		inline virtual const wchar_t* GetName() const override
		{
			return this->NAME;
		}

	public:
		static constexpr const wchar_t* NAME = L"Rectangle";
	};
}

