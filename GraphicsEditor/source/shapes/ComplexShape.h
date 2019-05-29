#pragma once
#include "../../../PluginSupporter/source/PluginSupporter.h"
#include "../../../Exceptions/source/Exceptions.h"
#include <vector>
#include <string>

class ComplexShape :
	public BaseShape
{
private:
	class ComplexShapeFactoryFunctor
		:
		public IShapeFactoryFunctor
	{
	public:
		inline ComplexShapeFactoryFunctor(std::wstring name, std::vector<const BaseShape*>& shapes)
			:
			name(name),
			shapes(shapes)
		{
		}
		inline virtual BaseShape* operator()() override
		{
			return new ComplexShape(this->name, this->shapes);
		}
		virtual ~ComplexShapeFactoryFunctor() override;

	private:
		const std::vector<const BaseShape*> shapes;
		const std::wstring name;
	};

public:
	ComplexShape(const std::wstring& name, const std::vector<const BaseShape*>& shapes);
	virtual ~ComplexShape() override;
	inline virtual BaseShape* Clone() const override
	{
		throw Exception(L"Clone is not implemented");
		return nullptr;
	}
	virtual void Redraw(Gdiplus::Graphics* const graphics);
	inline virtual const wchar_t* GetName() const override
	{
		return this->name.c_str();
	}
	virtual void SetPoints(int x1, int y1, int x2, int y2) override;
	virtual void SetColor(Gdiplus::ARGB color) override;
	inline static IShapeFactoryFunctor* CreateFactoryFunctor(std::wstring name, std::vector<const BaseShape*> shapes)
	{
		return new ComplexShapeFactoryFunctor(name, shapes);
	}
	
public:
	static constexpr const wchar_t* NAME = L"Complex Shape";

private:
	const std::wstring& name;
	const std::vector<const BaseShape*>& shapes;
	std::vector<BaseShape*> gfxShapes;
	int refWidth;
	int refHeight;
};

