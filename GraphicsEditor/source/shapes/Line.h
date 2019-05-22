#pragma once
#include "../../../PluginSupporter/source/PluginSupporter.h"

namespace Custom {
	class Line :
		public ::BaseShape
	{
	private:
		class LineFactoryFunctor
			:
			public IShapeFactoryFunctor
		{
		public:
			inline virtual BaseShape* operator()() override
			{
				return new Line();
			}
		};

	public:
		virtual void Redraw(Gdiplus::Graphics* const graphics);
		inline static IShapeFactoryFunctor* CreateFactoryFunctor()
		{
			return new LineFactoryFunctor();
		}
		inline virtual const wchar_t* GetName() const override
		{
			return this->NAME;
		}

	public:
		static constexpr const wchar_t* NAME = L"Line";
	};
}

