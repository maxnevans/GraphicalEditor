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
	void push(void* buffer);
	void* pop();
	bool is_empty();
	~List();

private:
	Node* header;
};

