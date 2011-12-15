	#include "Graph.h"
	#include "Stack.h"
	#include <cstring>
	#include <fstream>
	#include <cstdlib>

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
				//rozvijet stavovy prostor budeme dokud podgrafy maji vetsi nebo stejny pocet hran 
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
		
		if (argc != 2) 
		{
	        ofstream err ("output.txt");
			
			if(err.is_open())
			{
				err << "Path to graph file required in argument" << endl;
				err.close();
			}
			//cout << "Path to graph file required in argument" << endl;

	        exit(1);
	    }
	

		//nacteni grafu											
	    Graph* solution = solve(Graph::loadGraph(argv[1]));
	    
		ofstream output ("output.txt", ofstream::app);

		//vypsani vysledku
		if (solution != NULL) 
		{
			if(output.is_open())
			{
				output << "Min degree spanning tree's matrix: " << endl << endl;
			
				output << "Degree: " << solution->getDegree() << endl;
				
				output.close();
			}
			solution->print();			
	    } 
		else 
		{
	        if(output.is_open())
			{
				output << "No solution found" << endl;
				output.close();
			}
	    }
	
	    delete solution;

    return 0;
}


