#include "ListShapes.h"

ListShapes::ListShapes()
	:
	list(new List)
{
}

ListShapes::~ListShapes()
{
	delete this->list;
}

void ListShapes::Push(Custom::BaseShape * shape)
{
	this->list->Push(shape);
}

Custom::BaseShape * ListShapes::Pop()
{
	return reinterpret_cast<Custom::BaseShape*>(this->list->Pop());
}

bool ListShapes::IsEmpty()
{
	return this->list->IsEmpty();
}
