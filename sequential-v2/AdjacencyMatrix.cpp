		#include "AdjacencyMatrix.h"
	
		using namespace std;

		//konstruktor
		AdjacencyMatrix::AdjacencyMatrix(int vertexCount){
	    
		this->vertexCount = vertexCount;
		this->size = vertexCount * vertexCount;
	    this->edges = new bool[size];		
		}
	
		//kopirovaci konstruktor
		AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix& original){
	    
		this->size = original.getSize();
		this->vertexCount = original.getVertexCount();
		this->edges = new bool[size];

		for(int i=0; i<size; i++)
			this->edges[i] = original.edges[i];				
		}
		
		//destruktor
		AdjacencyMatrix::~AdjacencyMatrix(){
			delete[] edges;
		}
	
		//vraci pocet vrcholu grafu	
		int AdjacencyMatrix::getVertexCount() const{
			return this->vertexCount;
		}
		
		//vraci pocet hran grafu
		int AdjacencyMatrix::getEdgeCount() const{
			int res = 0;
	    
			for(int i = 0; i < this->vertexCount; i++)
				for(int j = 0; j < this->vertexCount; j++)
			{
				if(this->edges[map(i,j)])
					res ++;
			}
			return res / 2;
		}

		//vraci pocet prvku matice
		int AdjacencyMatrix::getSize() const{
			return this->size;
		}

		//vraci jestli je obsazena hrana na pozici x,y
		bool AdjacencyMatrix::get(int x, int y) const {		
				return this->edges[map(x,y)];
		}
	
		//nastavi obsazenost hrany na pozici x,y
		void AdjacencyMatrix::set(int x, int y, bool value) {
			this->edges[map(x,y)] = value;
		}

		//vraci zda je [x,y] platna souradnice v matici
		bool AdjacencyMatrix::validateCoord(int x, int y) const {
			return (x>=0 && y>=0 && x<vertexCount && y<vertexCount);
		}

		//transformuje souradnice vraci index v poli ktere reprezentuje matici
		int AdjacencyMatrix::map(int x, int y) const {
			return (x*vertexCount) + y;
		}

		
