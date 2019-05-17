#pragma once
#include "../../PluginSupporter/source/PluginSupporter.h"

class TestShape :
	public ::BaseShape
{
public:
	virtual void Redraw(Gdiplus::Graphics* const graphics) override;
	static ::BaseShape* ShapeFactory();
	virtual std::wstring GetName() override;

public:
	static constexpr const wchar_t* NAME = L"TestShape";
};
