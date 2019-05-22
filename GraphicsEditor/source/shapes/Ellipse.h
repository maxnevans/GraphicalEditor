#pragma once
#include "../../../PluginSupporter/source/PluginSupporter.h"

namespace Custom {
	class Ellipse :
		public ::BaseShape
	{
	private:
		class EllipseFactoryFunctor
			:
			public IShapeFactoryFunctor
		{
		public:
			inline virtual BaseShape* operator()() override
			{
				return new Ellipse();
			}
		};

	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		inline static IShapeFactoryFunctor* CreateFactoryFunctor()
		{
			return new EllipseFactoryFunctor();
		}
		inline virtual const wchar_t* GetName() const override
		{
			return this->NAME;
		}

	public:
		static constexpr const wchar_t* NAME = L"Ellipse";
	};
}

