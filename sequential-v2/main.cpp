	#include "Graph.h"
	#include "Stack.h"
	#include <iostream> 
	#include <cstring>

	using namespace std;

	static Graph* solve(Graph* graph) {
	
		//pokud neni nic nacteno, vraceno NULL
		if(graph == NULL)
			return NULL;
	
	    int vertexCount = graph->getVertexCount();
		
		//pokud ma nacteny graf mene nez jeden vrchol, vraceno NULL
		if(vertexCount < 2)
			return NULL;

		//pokud neni vstupni graf spojity, vraceno NULL
		if(!graph->isContinuous())
			return NULL;

		//pocet hran kostry
	    int edgeLimit = vertexCount - 1;

		//nejlepsi reseni
	    Graph* best = NULL;

	    Stack s;
	    s.push(graph);
	    while (!s.is_empty()) {
	        Graph* subgraph = s.pop();
//			int subgraphEdgeCount = subgraph->getEdgeCount();
			
			//pro nalezeni lepsiho reseni musi platit, ze je kostrou vstupniho grafu a ma mensi stupen nez dosud nejlepsi reseni
			if (subgraph->getEdgeCount() == edgeLimit && subgraph->isSpanningTree() && 
				((best != NULL && best->getDegree() > subgraph->getDegree()) || best == NULL)) 
			{
	            //nahrazeni nejlepsiho reseni
				delete best;
	            best = new Graph(*subgraph);
	        } 
			else 
			{
				//rozvijej stavovy prostor budeme dokud podgrafy grafy maji vetsi nebo stejny pocet hran 
				//ktery je potreba k vytvoreni kostry a dale pokud jsou spojite 
				if(subgraph->getEdgeCount() > edgeLimit && subgraph->isContinuous()){
	                for (int i = 0; i < (vertexCount - 1); i++) {
	                    for (int j = i + 1; j < vertexCount; j++) {
	                        if (subgraph->getEdge(i, j) && subgraph->getEdge(j, i)) 
							{	
	                            Graph* temp = new Graph(*subgraph);
								temp->setEdge(i, j, false);
								temp->setEdge(j, i, false);

	                            s.push(temp);
	                        } 
							else 
							{
	                            continue;
	                        }
	                    }
	                }
				}
	        }
	        delete subgraph;
	    }
	    return best;
	}

//main
int main(int argc, char* argv[])
{
		//if (argc != 2) 
		//{
	        //cout << "Path to graph file required in argument" << endl;
	        //exit(1);
	    //}
	
		//nacteni grafu											//argv[1]
	    Graph* solution = solve(Graph::loadGraph("graph4_full.txt"));
	    
		//vypsani vysledku
		if (solution != NULL) 
		{
	        cout << "Min degree spanning tree's matrix: " << endl << endl;
			
			cout << "Degree: " << solution->getDegree() << endl;
	        solution->print();
	    } 
		else 
		{
	        cout << "No solution found" << endl;
	    }
	
	    delete solution;

    return 0;
}


