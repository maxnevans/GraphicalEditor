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
			virtual BaseShape* operator()() override;
		};

	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		static IShapeFactoryFunctor* CreateFactoryFunctor();
		virtual const wchar_t* GetName() override;

	public:
		static constexpr const wchar_t* NAME = L"Ellipse";
	};
}

