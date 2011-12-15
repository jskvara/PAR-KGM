	#pragma once
	
	#include "AdjacencyMatrix.h"
	
	class Graph {

	        private:
	                //matice sousednosti
					AdjacencyMatrix* matrix; 					
					//pomocne pole pri prohledavani do hloubky
					bool* seen;
					//pomocne pole pro vypocet stupne grafu
					int* degrees;
	
	        public:
					//konstruktor
	                Graph(int vertexCount);
					//kopirujici konstruktor
	                Graph(const Graph& original);
					//destruktor
	                ~Graph();
					//staticka metoda pro nacteni grafu ze souboru
					static Graph* loadGraph(const char* fileName);
					//vraci obsazenost hrany na [x,y]
	                bool getEdge(int x, int y) const;
					//nastavuje obsazenost hrany na [x,y]
					void setEdge(int x, int y, bool value);
					//vraci pocet vrcholu grafu
					int getVertexCount() const;
					//vraci pocet hran grafu
	                int getEdgeCount() const;
       				//vraci zda je aktualni instance kostrou grafu (spojity graf bez cyklu)	
					bool isSpanningTree();
					//vraci zda je aktualni instance spojity graf
					bool isContinuous();
					//prohledava graf do hloubky, vraci jestli je graf acyklicky					
					bool dfs(int vertex);
					//vraci stupen grafu
					int getDegree() const;
					//vytiskne graf na standardni vystup
	                void print() const;
		};
