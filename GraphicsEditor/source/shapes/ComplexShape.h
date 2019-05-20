#pragma once
#include "../../../PluginSupporter/source/PluginSupporter.h"
#include <vector>

class ComplexShape :
	public BaseShape
{
private:
	class ComplexShapeFactoryFunctor
		:
		public IShapeFactoryFunctor
	{
	public:
		ComplexShapeFactoryFunctor(std::vector<BaseShape*> shapes);
		virtual BaseShape* operator()() override;

	private:
		std::vector<BaseShape*> shapes;
	};

public:
	ComplexShape(std::vector<BaseShape*> shapes);
	virtual void Redraw(Gdiplus::Graphics* const graphics);
	virtual const wchar_t* GetName() override;
	static IShapeFactoryFunctor* CreateFactoryFunctor(std::vector<BaseShape*> shapes);
	
public:
	static constexpr const wchar_t* NAME = L"Complex Shape";

private:
	std::vector<BaseShape*> shapes;
};

