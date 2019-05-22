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
			inline virtual BaseShape* operator()() override
			{
				return new Triangle();
			}
		};

	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		inline static IShapeFactoryFunctor* CreateFactoryFunctor()
		{
			return new TriangleFactoryFunctor();
		}
		inline virtual const wchar_t* GetName() const override
		{
			return this->NAME;
		}

	public:
		static constexpr const wchar_t* NAME = L"Triangle";
	};
}

