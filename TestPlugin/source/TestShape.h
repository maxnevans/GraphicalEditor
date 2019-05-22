#pragma once
#include "../../PluginSupporter/source/PluginSupporter.h"

class TestShape :
	public ::BaseShape
{
private:
	class TestFactoryFunctor
		:
		public IShapeFactoryFunctor
	{
	public:
		inline virtual BaseShape* operator()() override
		{
			return new TestShape();
		}
	};
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics) override;
	inline static IShapeFactoryFunctor* CreateFactoryFunctor()
	{
		return new TestFactoryFunctor();
	}
	inline virtual const wchar_t* GetName() const override
	{
		return this->NAME;
	}

public:
	static constexpr const wchar_t* NAME = L"TestShape";
};
