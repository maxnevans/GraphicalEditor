#pragma once
#include "List.h"
#include "shapes/BaseShape.h"

class ListShapes
{
public:
	ListShapes();
	~ListShapes();
	void Push(Custom::BaseShape* shape);
	Custom::BaseShape* Pop();
	bool IsEmpty();
private:
	List* list;
};

