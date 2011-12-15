#include "Graph.h"
#include "Stack.h"
#include "mpi.h"
#include <cstring>
#include <fstream>
#include <cstdlib>

using namespace std;

enum State {
	BUSY, // working
	SEEK, // need work
	LISTEN, // 
	TERMINATING, // best solution
	DONE //
};

enum Tag {
	SIZE, // message size
	STACK,
	REQUEST, // request work
	REJECT, // reject request
	TOKEN,
	BEST // best soultion
};

enum Color {
	NONE,
	WHITE,
	BLACK
};

int my_rank = 0; // process id
int next_rank; // next rank in the circle
int process_count; // process number
int request_counter; // request for work process number
int init_counter; // process without work
State current_state; // process current state
MPI_Status status; // MPI status
Stack stack; // stack with states
Color my_color; // process color
Color my_token; // token color
int ZERO = 1;
int ONE = 1;
Graph* best = NULL;// nejlepsi reseni
int max_degree = 9999999;// stupen nejlepsiho reseni
int graph_size;// velikost grafu
int vertexCount;// pocet vrcholu

void log(const std::string& message) {
	cout << "[" << my_rank << "] " << message << '\n' << flush;
}

// Nastavi MPI a promenne
void init(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	
	if (my_rank == 0) {
		current_state = BUSY;
		init_counter = 1;
		my_token = WHITE;
	} else {
		current_state = LISTEN;
		init_counter = process_count;
		my_token = NONE;
	}
	next_rank = (my_rank + 1) % process_count;
	request_counter = next_rank;
	my_color = WHITE;
}

void work() {
	while (!stack.is_empty()) {
		Graph* subgraph = stack.pop();
		int vertexCount = subgraph->getVertexCount();
		int edgeLimit = vertexCount - 1;//pocet hran kostry

		//pro nalezeni lepsiho reseni musi platit, ze je kostrou vstupniho grafu ma mensi stupen nez dosud nejlepsi reseni
		if (subgraph->getEdgeCount() == edgeLimit && 
				subgraph->isSpanningTree() &&
				((best != NULL && best->getDegree() > subgraph->getDegree()) || best == NULL)) {
			//nahrazeni nejlepsiho reseni
			delete best;
			best = new Graph(*subgraph);
		} else {
			//rozvijet stavovy prostor budeme dokud podgrafy maji vetsi nebo stejny pocet hran
			//ktery je potreba k vytvoreni kostry a dale pokud jsou spojite
			if (subgraph->getEdgeCount() > edgeLimit && subgraph->isContinuous()) {
				for (int i = 0; i < (vertexCount - 1); i++) {
					for (int j = i + 1; j < vertexCount; j++) {
						if (subgraph->getEdge(i, j) && subgraph->getEdge(j, i)) {
							Graph* temp = new Graph(*subgraph);
							temp->setEdge(i, j, false);
							temp->setEdge(j, i, false);

							stack.push(temp);
						} else {
							continue;
						}
					}
				}
			}
		}

		delete subgraph;
	}
}

void request_work() {
	MPI_Send(&ZERO, 1, MPI_INT, request_counter, SIZE, MPI_COMM_WORLD);
	MPI_Send(&ZERO, 1, MPI_CHAR, request_counter, REQUEST, MPI_COMM_WORLD);
	do {
		request_counter = (request_counter + 1) % process_count;
	} while (request_counter == my_rank);
}

void send_token() {
	char data = my_token == WHITE ? 'w' : 'b';
	MPI_Send(&ONE, 1, MPI_INT, next_rank, SIZE, MPI_COMM_WORLD);
	MPI_Send(&data, 1, MPI_CHAR, next_rank, TOKEN, MPI_COMM_WORLD);
	my_token = NONE;
	my_color = WHITE;
}

void send_stack(Stack* _stack, Tag _tag, int _dest) {
	if (_stack->is_empty()) {
		return;
	}
	int stack_size = _stack->count();
	Graph* graph = _stack->pop();
	int buffer_size = graph_size * (stack_size);
	char* buffer = new char[buffer_size];
	const char* temp = graph->serialize();
	int i, j;
	for (i = 0, j = 0; i < graph_size; ++i, ++j) {
		buffer[i] = temp[i];
	}
	delete graph;
	while (!_stack->is_empty()) {
		graph = _stack->pop(); 
		temp = graph->serialize();
		for (i = 0; i < graph_size; ++i, ++j) {
			buffer[j] = temp[i];
		}
		delete graph;
	}
	MPI_Send(&buffer_size, 1, MPI_INT, _dest, SIZE, MPI_COMM_WORLD);
	MPI_Request request;
	MPI_Isend(buffer, buffer_size, MPI_CHAR, _dest, _tag, MPI_COMM_WORLD, &request);
	int flag = 0;
	while (!flag) {
		MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
	}
	delete[] buffer;
}

void fill_stack(char* _buffer, int _buffer_size) {
	int stack_size = 0;
	Graph* graph;
	for (char* pointer = _buffer; pointer < _buffer + _buffer_size; pointer += graph_size) {
		graph = new Graph(vertexCount);
		graph->deserialize(pointer);
		stack.push(graph);
		++stack_size;
	}
}

void process_request(int _dest) {
	Stack* splited = stack.split();
	if (splited==0) {
		MPI_Send(&ZERO, 1, MPI_INT, _dest, SIZE, MPI_COMM_WORLD);
		MPI_Send(&ZERO, 1, MPI_CHAR, _dest, REJECT, MPI_COMM_WORLD);
 	} else {		
		send_stack(splited, STACK, _dest);
		if (my_rank > _dest) {
			my_color = BLACK;
		}
	}
}

void terminate_next() {
	Stack bestStack;
	bestStack.push(new Graph(*best));
	send_stack(&bestStack, BEST, next_rank);
}

void process_token(Color _token) {
	if (my_rank == 0) {
		if (_token == WHITE) {
			terminate_next();
			current_state = TERMINATING;
		} else {
			my_token = WHITE;
		}
	} else {
		if (my_color == BLACK) {
			my_token = BLACK;
		} else {
			my_token = _token;
		}
	}
}

void listen() {
	if (!stack.is_empty()) {
		int flag = 0;
		MPI_Iprobe(MPI_ANY_SOURCE, SIZE, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
		if (!flag) {
			return;
		}
	}
	
	int buffer_size;
	MPI_Recv(&buffer_size, 1, MPI_INT, MPI_ANY_SOURCE, SIZE, MPI_COMM_WORLD, &status);
	char* buffer = new char[buffer_size];
	MPI_Recv(buffer, buffer_size, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	//cout << buffer_size << " " << buffer << endl;

	Graph* temp;
	int temp_degree;
	switch ((Tag)status.MPI_TAG) {
		case STACK:
			log("listen stack");
			fill_stack(buffer, buffer_size);
			current_state = BUSY;
			break;
		case REQUEST:
			log("listen request");
			if (current_state != TERMINATING) {
				process_request(status.MPI_SOURCE);
			}
			break;
		case REJECT:
			log("listen reject");
			if (current_state != TERMINATING) {
				current_state = SEEK;
			}
			break;
		case TOKEN:
			log("listen token");
			process_token(*buffer == 'w' ? WHITE : BLACK);
			break;
		case BEST:
			log("listen best");
			//cout << "listen best" << endl;
			fill_stack(buffer, buffer_size);
			//cout << "fill stack" << endl;
			temp = stack.pop();
			temp_degree = temp->getDegree();
			//cout << temp->toString() << endl;
			//cout << temp_degree << endl;
			if (temp_degree < max_degree) {
				delete best;
				best = temp;
				max_degree = temp_degree;
			} else {
				delete temp;
			}
			current_state = DONE;
			break;
		default:
			throw "Unknown tag.";
	}
	delete[] buffer;
}

void run_parallel() {
	bool running = true;
	while (running) {
		switch (current_state) {
			case BUSY:
				log("busy");
				work();
				if (stack.is_empty()) {
					current_state = SEEK;
				} else {
					current_state = LISTEN;
				}
				break;
			case SEEK:
				log("seek");
 				request_work();
				current_state = LISTEN;
				break;
			case LISTEN:
				log("listen");
				if (stack.is_empty() && my_token != NONE) {
					send_token();
				}
			case TERMINATING:
				log("terminating");
				listen();
				if (!stack.is_empty()) {
					current_state = BUSY;
				}
				break;
			case DONE:
				log("done");
				if (my_rank != 0) {
					terminate_next();
				}
				running = false;
				break;
			default:
				throw "Unknown state.";
		}
	}
}

Graph* run_sequential(Graph* graph) {
	int vertexCount = graph->getVertexCount();
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
			((best != NULL && best->getDegree() > subgraph->getDegree()) || best == NULL)) {
			//nahrazeni nejlepsiho reseni
			delete best;
			best = new Graph(*subgraph);
		} else {
			//rozvijet stavovy prostor budeme dokud podgrafy maji vetsi nebo stejny pocet hran 
			//ktery je potreba k vytvoreni kostry a dale pokud jsou spojite 
			if(subgraph->getEdgeCount() > edgeLimit && subgraph->isContinuous()) {
				for (int i = 0; i < (vertexCount - 1); i++) {
					for (int j = i + 1; j < vertexCount; j++) {
						if (subgraph->getEdge(i, j) && subgraph->getEdge(j, i)) {	
							Graph* temp = new Graph(*subgraph);
							temp->setEdge(i, j, false);
							temp->setEdge(j, i, false);
							s.push(temp);
						} else {
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

static Graph* solve(Graph* graph) {
	//pokud neni nic nacteno, vraceno NULL
	if (graph == NULL) {
		return NULL;
	}

	vertexCount = graph->getVertexCount();
	
	//pokud ma nacteny graf mene nez jeden vrchol, vraceno NULL
	if (vertexCount < 2) {
		return NULL;
	}

	//pokud neni vstupni graf spojity, vraceno NULL
	if (!graph->isContinuous()) {
		return NULL;
	}

	graph_size = graph->getVertexCount() * graph->getVertexCount() + 1;
	if (my_rank == 0) {
		stack.push(graph);
	} else {
		delete graph;
	}
	
	if (process_count > 1) {
		run_parallel();
		if (max_degree == 9999999) {
			delete best;
			best = NULL;
		}
	} else {
		return run_sequential(graph);
	}

	return best;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		ofstream err("output.txt");
		if (err.is_open()) {
			err << "Path to graph file required in argument" << endl;
			err.close();
		}
		cout << "Usage: " << argv[0] << " FILENAME" << endl;
		exit(-1);
	}
	
	try {
		init(argc, argv);
		
		// Mereni casu - zacatek
		double time;
//		MPI_Barrier(MPI_COMM_WORLD); // mereni casu - cekam na spusteni vsech procesu, spusteno v init()
		if (my_rank == 0) {
			time = MPI_Wtime();
		}
	
		//nacteni grafu
		Graph* loaded_graph = Graph::loadGraph(argv[1], my_rank);
		Graph* solution = solve(loaded_graph);

		// Mereni casu - konec
		if (my_rank == 0) {
			time = MPI_Wtime() - time;
		}

		//vypsani vysledku
		if (my_rank == 0) {
			ofstream output("output.txt", ofstream::app);
			if (solution != NULL) {
				if (output.is_open()) {
					output << "Min degree spanning tree's matrix: " << endl << endl;
					output << "Degree: " << solution->getDegree() << endl;
					output << "Time: " << time << endl;
					output.close();
				}
				solution->print();
			} else {
				if (output.is_open()) {
					output << "No solution found" << endl;
					output.close();
				}
			}
		}
		delete solution;
	} catch(const char* message) {
		cout << message << endl;
		MPI_Finalize();
		return -1;
	}
	MPI_Finalize();

	return 0;
}
