#include <cstdio>
#include <cstdlib>
#include <stack>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

//Hrana
struct Edge
{
	int refNodeId; //Id uzlu, ke kteremu vede hrana
	Edge* next; //Odkaz na dalsi hranu (linked list)
};

//Uzel 
struct Node
{
	int id; //Id uzlu
	bool visited; //Priznak, zda byl navstiven (pri prohledavani)
	Node* next; //Odkaz na dalsi uzel (linked list)
	Edge* firstEdge; //Odkaz na prvni hranu, ktera vede z aktualniho uzlu
};

//Vytvori datovou strukturu graf z matice incidence nactene ze souboru
Node* CreateGraph(const char* fileName, int &nodeCount)
{
	FILE* file;
	
	//Nezadano jmeno souboru
	if (fileName == NULL)
	{
		cout << "Soubor pro nacitani neni zadan." << endl;
		return NULL;
	}

	//Soubor nelze z nejakeho duvodu nalezt nebo otevrit ke cteni
	file = fopen(fileName,"r");
	if (file == NULL)
	{
		cout << "Soubor " << fileName << " se nepodarilo otevrit." << endl;
		return NULL;
	}

	//Nacteme prvni radek souboru - pocet uzlu je spocten pozdeji z faktualnich dat
	int c = getc(file);
	while (c != EOF && c != '\n')
	{
        c = getc(file);
	}
	
	//Odkaz na prvni uzel grafu
	Node* first = NULL;
	
	//Pomocne ukazatele pro stavbu grafu
	Edge* currentEdge = NULL;
	Node* currentNode = NULL;
	
	//Priznak, ze zacina novy radek matice = definovan novy uzel
	bool newNode = true;
	
	//Pocitani radku, sloupcu
	int rowCounter = 1;
	int colCounter = 1;

	//Nacitame znak po znaku
	while ((c = getc(file)) != EOF)
	{
        //Mezeru preskakujem
		if ( c == ' ') 
			continue;
		
		//Newline
		if ( c == '\n')
		{
			newNode = true;
			colCounter = 1;
			rowCounter ++;
		}
		else
		{
			//Ignorujeme - zpetna hrana 
			if (rowCounter == colCounter)
			{
				colCounter++;
				continue;
			}
			//Pokud zacal novy radek
			if (newNode)
			{
				//naalokujeme novy uzel
				newNode  = false;
				Node* node = new Node;
				node->next = NULL;
				node->firstEdge = NULL;
				node->id = rowCounter;
				node->visited = false;

				//a budujeme spojovy seznam 
				if (first == NULL)
				{
					first = node;
					currentNode = node;
				}
				else
				{
					currentNode->next = node;
					currentNode = node;
				}

				nodeCount++;
			}
			//hrana
			if (c == '1')
			{
				//Vytvoreni nove hrany
				Edge* edge = new Edge;
				edge->next = NULL;
				edge->refNodeId = colCounter;
                
				//Pripojime k aktualnimu uzlu (spoj. seznam hran je 2. dimenze spojoveho seznamu uzlu)
				if(currentNode->firstEdge == NULL)
				{
					currentNode->firstEdge = edge;				
				}
				else
				{
					currentEdge->next = edge;
				}

				currentEdge = edge;
			}
			
			colCounter ++;
		}
	}
	
	//uzavreni souboru
	fclose(file);

	//vracime odkaz na prvni uzel v seznamu
	return first;
}

//Spocte dosud nenavstivene uzly v grafu pri prohledavani
int CountUnvisitedNodes(Node* first)
{
	Node* pom = first;
	int res = 0;

	while(pom != NULL)
	{
		if(!pom->visited)
			res++;
		pom = pom->next;
	}

	return res;
}

//Zjistit zda byl dany uzel navstiven pri prohledavani
bool IsVisited(int nodeId, Node* first) {
	
	Node* pom = first;

	while (pom != NULL) 
	{
       if (pom->id == nodeId) 
	   {
 			break;
	   }
	   pom = pom->next;
	}

	if (pom != NULL)
	{
		return pom->visited;
	}
        return true;
}

//Nastavi zda byl dany uzel navstiven pri prohledavani
void SetVisited(bool visited, int nodeId, Node* first) {

	Node* pom = first;
	while (pom != NULL) 
	{
         if (pom->id == nodeId)
		 {
 			break;
		 }
         pom = pom->next;
	}
	if (pom != NULL)
	{
		pom->visited = visited;
	}
}

//Resetuje vsechny uzly v grafu jako nenavstivene
void RegenerateGraph(Node *first)
{
	Node* pom = first;
	
	while (pom != NULL) 
	{
        pom->visited = false; 
		pom = pom->next;
	}
}

//Vlozi uzel na zasobnik
void PushToStack(stack<Node*> &stack, int nodeId, Node *first)
{
	Node* pom = first;
	
	while (pom != NULL) 
	{
         if (pom->id == nodeId)
		 {
 			break;
		 }
         pom = pom->next;
	}
	if (pom != NULL)
	{
		stack.push(pom);
	}
}

//Konverze int na string
string iToStr(int number)
{
   stringstream ss;
   ss << number;
   return ss.str();
}

//Vytiskne reseni ze zasobniku (2.zasobnik)
void PrintStack(stack<string> &s)
{
	stack<string> temp;
	
	string solutionText = "";

	while (!s.empty())
	{		
		temp.push(s.top());
		solutionText += s.top();
		s.pop();
	}
	
	while(!temp.empty())
	{	
		s.push(temp.top());
		temp.pop();
	}

	cout << "Nalezena kostra: " << endl;
	cout << solutionText;
}

//Vynuluje vektor 
void ResetVector(vector<int> &vec)
{
	for(unsigned int i = 0; i< vec.size(); i++)
		vec.at(i) = 0;
}

//Vrati maximalni stupen kostry grafu - ve vektoru evidovany cetnosti jednotlivych uzlu v kostre
int GetMaxDegree(vector<int> &vec)
{
	int max = 0;
	
	for(unsigned int i = 0; i< vec.size(); i++)
	{
		if(vec.at(i) > max)
			max = vec.at(i);
	}

	return max;
}

//dealokace grafu
void DeallocateGraph(Node *first) {

  Node* pomNode = first;
  Edge* pomEdge1 = NULL;
  Edge* pomEdge2 = NULL;

	while(first != NULL) 
	{
	  if (first->firstEdge != NULL) 
	  {
		  pomEdge1 = first->firstEdge;
		  while(pomEdge1 != NULL)
		  {
			 pomEdge2 = pomEdge1;
			 pomEdge1 = pomEdge1->next;
			 free(pomEdge2);
		  }
	  }

	  pomNode = first;
	  first = first->next;

	  free(pomNode);
  }
}

//main
int main(int argc, char* argv[])
{
	int nodeCount = 0;
	int maxDegree = 9999999;

	//Nacteni grafu
	Node* first = CreateGraph("test.txt", nodeCount);

	//Vektor pro evidenci stupnu uzlu
	vector<int> nodeDegrees(nodeCount);

	//Jeho vynulovani
	ResetVector(nodeDegrees);

	//Zasobnik textu reseni
	stack<string> solution;
	
	//Hlavni zasobnik
	stack<Node*> stack;

	//Uzel v kterem se zacne prohledavat
	Node* startNode = first;

	//Postupne zaciname prohledavani ze vsech uzlu
	while(startNode != NULL)
	{
		//Startovni uzel navstiven
		startNode->visited = true;
		
		//A vlozen na zasobnik
		stack.push(startNode);

		while(!stack.empty())
		{
			//Vememe uzel z vrcholu zasobniku
			Node* currentNode = stack.top(); stack.pop();

			//Vememe z neho jdouci hranu
			Edge* currentEdge = currentNode->firstEdge;
			
			//Priznak zda-li vsechny sousedici uzly aktualniho byly jiz navstiveny pri prohledavani, zatim true
			bool allVisited = true;

			//dokud neprojdeme vsechny hrany jdouci z aktualniho uzlu
			while(currentEdge != NULL)
			{
				//pokud aktualni soused nebyl jeste navstiven
				if(!IsVisited(currentEdge->refNodeId,first))
				{				
					//pridame spojujici hranu do zasobniku s resenim (text)
					solution.push(iToStr(currentNode->id) + "--" + iToStr(currentEdge->refNodeId) + "\n");
					
					//a zvysime stupen uzlu pridanych do reseni v evidenci (vektoru)
					nodeDegrees.at(currentNode->id - 1) ++; 
					nodeDegrees.at(currentEdge->refNodeId - 1) ++;

					//aktualniho souseda pridame na zasobnik
					PushToStack(stack,currentEdge->refNodeId,first);
				
					//nastavime priznak ze byl navstiven
					SetVisited(true,currentEdge->refNodeId,first);
					
					//pokud byly navstiveny vsechny uzly, mame jednu z koster
					if(CountUnvisitedNodes(first) == 0)
					{
						//vytiskneme ho
						PrintStack(solution);
						
						//spocteme stupen kostry
						int deg = GetMaxDegree(nodeDegrees);
						
						//porovname s aktualnim stupnem kostry
						if(deg < maxDegree)
							maxDegree = deg;

						cout << "Stupen kostry : " << deg << endl;


						//vynulujeme vektor evidujici stupne uzlu v kostre
						ResetVector(nodeDegrees);
					}

					//exituje alespon jeden soused aktualniho uzlu ktery nebyl jeste navstiven... jinak by tato cast kodu neprobehla
					allVisited = false;
				}
				
				//prejdeme na dalsiho souseda
				currentEdge = currentEdge->next;
			}

			//pokud vsichni sousedi jiz navstiveni (slepa ulicka)
			if(allVisited)
			{
				//vracime se zpatky, aktualni uzel oddelame z reseni, a nastavime ho jako nenavstiven 
				currentNode->visited = false;
				solution.pop();
			}
		}

		//budeme zacinat prohledavani z jineho uzlu

		//vyprazdnime zasobnik reseni
		while(!solution.empty())
			solution.pop();

		//resetujeme vektor evidujici stupne uzlu v kostre
		ResetVector(nodeDegrees);

		//vsechny uzly v grafu nastavime jako nenavstivene
		RegenerateGraph(first);

		//a zacneme prohledavat z dalsiho uzlu
		startNode = startNode->next;
	}

	cout << "Kostra s minimalnim stupnem ma stupen " << maxDegree << endl; 

	DeallocateGraph(first);

    return 0;
}


