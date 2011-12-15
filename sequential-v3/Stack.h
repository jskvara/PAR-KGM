#include "Graph.h"

#pragma once

	//pomocna struktura pro reprezentaci prvku v zasobniku -> spojovy seznam
	struct StackNode {
	        Graph *graph;
	        StackNode* next;
	};

	class Stack {
	        private:
	                //odkaz na prvni prvek
					StackNode* m_top; 
	                //velikost zasobniku
					int size;
	
	        public:
	                //konstruktor
					Stack();
	                //destruktor
					~Stack();
	                //prida prvek na zasobnik
					void push(Graph* graph);
	                //odebere prvek ze zasobniku
					Graph* pop();
	                //kontrola zda je zasobnik prazdny
					bool is_empty() const;
	                //vyprazdni zasobnik
					void clear();
					//vrati pocet prvku v zasobniku
	                int count() const;
	};
	
	
