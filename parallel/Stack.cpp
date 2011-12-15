#include "Stack.h"

//konstruktor
Stack::Stack() : m_top(0), size(0) {
}

//destruktor
Stack::~Stack() {
	clear();
}

//pridani prvku na zasobnik
void Stack::push(Graph* graph) {
	StackNode* node = new StackNode();
	
	node->graph = graph;
	node->next = m_top;
	m_top = node;
	size++;
}

//odebrani prvku ze zasobniku
Graph* Stack::pop() {
	StackNode* node = m_top;
	m_top = node->next;
	
	Graph* graph = node->graph;
		
	size--;
	delete node;
	
	return graph;
}

//vyprazdneni zasobniku
void Stack::clear() {
	while (!is_empty()) {
		pop();
	}
}

//kontrola zda je zasobnik prazdny
bool Stack::is_empty() const {
	return m_top == 0;
}

//pocet prvku v zasobniku
int Stack::count() const {
	return size;
}

//rozdeli zasobnik na pulku
Stack* Stack::split() {
	if (this->is_empty()){
		return 0;
	}
	
	Stack* newStack = new Stack();
	int count = size / 2;
	for (int i = 0; i < count; i++) {
		newStack->push(pop());
	}
	
	return newStack;
}
