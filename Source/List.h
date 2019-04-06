#pragma once

class List
{
private:
	struct Node {
		Node* next;
		void* item;
	};
public:
	List();
	void Push(void* buffer);
	void* Pop();
	bool IsEmpty();
	~List();

private:
	Node* header;
};

