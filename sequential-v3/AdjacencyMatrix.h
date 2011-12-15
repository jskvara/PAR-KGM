	#include <iostream>		

	#pragma once
	
	//trida reprezentujici matici sousednosti grafu
	class AdjacencyMatrix
	{
	
		private:
			//dvojrozmerne pole pro uchovani hran 
			bool *edges;			
			//pocet vrcholu grafu
			int vertexCount;			
			//pocet prvku matice
			int size;

		public:
			//konstruktor
			AdjacencyMatrix(int vertexCount);
			//kopirovaci konstruktor
			AdjacencyMatrix(const AdjacencyMatrix& original);
			//destruktor
			~AdjacencyMatrix();

			//vraci jestli je obsazena hrana na pozici x,y
			bool get(int x, int y) const;
			//nastavi obsazenost hrany na pozici x,y
			void set(int x, int y, bool value);			
			//vraci pocet vrcholu grafu	
			int getVertexCount() const;
			//vraci pocet hran grafu
			int getEdgeCount() const;
			//vraci pocet prvku matice
			int getSize() const;
			//vraci zda je [x,y] platna souradnice v matici
			bool validateCoord(int x, int y) const;
			//transformuje souradnice vraci index v poli ktere reprezentuje matici
			int map(int x, int y) const;
	};
