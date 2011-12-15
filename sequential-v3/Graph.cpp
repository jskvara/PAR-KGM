	#include <iostream>
	#include <fstream>
	#include <cstdlib>
	#include "Graph.h"

	using namespace std;

	//konstruktor
	Graph::Graph(int vertexCount) {
	    matrix = new AdjacencyMatrix(vertexCount);
		seen = new bool[vertexCount];
		degrees = new int[vertexCount];

		for(int i=0; i< vertexCount; i++)
		{
			seen[i] = false;
			degrees[i] = 0;
		}

	}
	//kopirujici konstruktor
	Graph::Graph(const Graph& original) {
		matrix = new AdjacencyMatrix(*original.matrix);
		seen = new bool[matrix->getVertexCount()];
		degrees = new int[matrix->getVertexCount()];

		for(int i=0; i< matrix->getVertexCount(); i++)
		{
			seen[i] = false;
			degrees[i] = 0;
		}
	}
	//destruktor
	Graph::~Graph() {
	    delete matrix;
		delete[] seen;
		delete[] degrees;
	}
	
	//staticka metoda pro nacteni grafu ze souboru
	Graph* Graph::loadGraph(const char* fileName) {
	    
		ifstream inputFile(fileName);
	    
		ofstream output("output.txt");

		if (!inputFile.is_open()) {
			if(output.is_open())
			{
				output << "Cannot open the input file " << fileName << endl;
				output.close();
			}
			return NULL;
		}
	    
		char str_vertexCount[25];
	    inputFile.getline(str_vertexCount, 25);
	    int vertexCount = atoi(str_vertexCount);
	    
		Graph* graph = new Graph(vertexCount);
	    
		char c;
		for (int i = 0; i < vertexCount; i++) {
	        for (int j = 0; j < vertexCount; j++) {
                inputFile.get(c);
	            if(c==' ' || c=='\n') {
	            	inputFile.get(c);
	            }
	            
				if(c!='0' && c!='1')
				{
					if(output.is_open())
					{
						cout << "Invalid character '" << c << "'  at [" << i << "," << j << "]"<< endl;
						output.close();
					}
					
					return NULL;
				}

				if (graph->matrix->validateCoord(i, j)) 
				{
					graph->setEdge(i, j, c=='1');
					graph->setEdge(j, i, c=='1');
				}
				
				//nacteme mezeru / enter
				inputFile.get();
			}
	    }
	    inputFile.close();
		
		

		if(output.is_open())
		{
			output << "Input graph matrix" << endl << endl;
			output.close();
		}

		graph->print();
	    return graph;
	}
	
	//vraci obsazenost hrany na [x,y]	
	bool Graph::getEdge(int x, int y) const {
	    return matrix->get(x, y);
	}
	//nastavuje obsazenost hrany na [x,y]
	void Graph::setEdge(int x, int y, bool connected) {
	    matrix->set(x, y, connected);
	}
	//vraci pocet vrcholu grafu
	int Graph::getVertexCount() const {
	    return matrix->getVertexCount();
	}
	//vraci pocet hran grafu
	int Graph::getEdgeCount() const {
		return matrix->getEdgeCount();
	}
	
	//prohledava graf do hlouky, vraci jestli je acyklicky
	bool Graph::dfs(int vertex) {
		bool res = true;
		if(vertex !=0)
			seen[vertex] = true;
		int vertexCount = matrix->getVertexCount();
		
		for(int i=vertex; i<vertexCount; i++)
			if((!seen[i]) && (matrix->get(vertex,i)))
			{
				if(vertex == 0)
					seen[0] = true;
				dfs(i);
			}
			else if (vertex!=0)
				res = false;

		return res;
	}
	
	//vraci zda je aktualni instance kostrou grafu (spojity graf bez cyklu)	
	bool Graph::isSpanningTree() {	    
	    
		int vertexCount = matrix->getVertexCount();
		
		if(vertexCount == 0)
			return false;

		bool res = dfs(0);

		for(int i=0; i>vertexCount; i++)
		{
			if(!seen[i])
				res = false;
			seen[i] = false;
		}
		
		return res;
	}

	//vraci zda je aktualni instance spojity graf	
	bool Graph::isContinuous() {	
		int vertexCount = matrix->getVertexCount();
		
		if(vertexCount == 0)
			return false;

		bool res = dfs(0);
		res = true;

		for(int i=0; i<vertexCount; i++)
		{
			if(!seen[i])
			{
				res = false;
			}
			seen[i] = false;
					
		}

		return res;
	}

	//vraci stupen grafu
	int Graph::getDegree() const {
		int vertexCount = matrix->getVertexCount();

		int max = 0;
		
		for (int i = 0; i < vertexCount; i++) {
	        for (int j = 0; j < vertexCount; j++) {
				if(matrix->get(i,j))
				{
					degrees[i] ++;
				}
			}
		}

		for(int i =0; i<vertexCount; i++)
		{
			if(degrees[i] > max)
			{
				max = degrees[i];
			}
			degrees[i] = 0;
		}
		
		return max;
	}

	//vytiskne graf na standardni vystup
	void Graph::print() const {
	    
		ofstream output("output.txt", ofstream::app);

		if(output.is_open())
		{
			int vertexCount = matrix->getVertexCount();
			for (int i = 0; i < vertexCount; ++i) {
				for (int j = 0; j < vertexCount; ++j) {				
					if (matrix->validateCoord(i, j)) {
						output << (matrix->get(i,j) ? "1 " : "0 ");
					} 
				}
				output << endl;
			}

			output << endl;

			output.close();
		}		
	}
