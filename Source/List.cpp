#include "List.h"
#include <stdlib.h>
#include <exception>
#include <assert.h>

List::List()
{
	this->header = nullptr;
}

void List::push(void* buffer)
{
	Node* node = new Node;
	node->next = nullptr;
	node->item = buffer;

	if (this->header)
	{
		Node* tmp = this->header;
		while (tmp->next) tmp = tmp->next;
		tmp->next = node;
	}
	else
	{
		this->header = node;
	}
}

void* List::pop()
{
	if (this->header)
	{
		void* ret_val = this->header->item;
		Node* tmp = this->header;
		this->header = this->header->next;
		delete tmp;
		return ret_val;
	}
	else
	{
		return nullptr;
	}

}

bool List::is_empty()
{
	return this->header == nullptr;
}


List::~List()
{
	while (!this->is_empty())
	{
		this->pop();
	}
}
