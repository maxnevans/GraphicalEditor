#include "ShapesFactory.h"
#include "../../../Exceptions/source/Exceptions.h"
#include <assert.h>
#include <string>

BaseShape* ShapesFactory::CreateShape(ShapeID shapeId) const
{
	assert(shapeId >= 0);
	assert(shapeId < this->regList.size());

	return (*regList[shapeId].fm)();
}

BaseShape* ShapesFactory::CreateShape(const wchar_t* name) const
{
	std::wstring in_name = name;
	for (const RegEntry& re : this->regList)
		if (in_name == re.name)
			return (*re.fm)();

	throw Exception(std::wstring(L"Trying to create undefined shape: { " + in_name + L" }").c_str());
	return nullptr;
}

ShapeID ShapesFactory::RegisterShape(const wchar_t* name, IShapeFactoryFunctor* func)
{
	ShapeID ret = static_cast<ShapeID>(this->regList.size());
	RegEntry re;
	re.fm = func;
	re.name = name;

	this->regList.push_back(re);

	return ret;
}

ShapesFactory::~ShapesFactory()
{
	for (RegEntry re : this->regList)
		delete re.fm;
}
