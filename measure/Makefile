
#all: $(SOURCES) $(EXECUTABLE)	
#$(EXECUTABLE): $(OBJECTS) 
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
#.cpp.o:
#	$(CC) $(CFLAGS) $< -o $@
COMPILER = mpiCC#g++
SOURCES = AdjacencyMatrix.cpp Graph.cpp Stack.cpp
#OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE = main
SOURCEFILE = $(EXECUTABLE).cpp
CFLAGS = -Wall -pedantic
#CFLAGS += -c # not to run linker
#CFLAGS += -g # debug information
#CFLAGS += -D DEBUG_INPUT -D DEBUG_DIST -D DEBUG_STACK -D DEBUG_NET -D DEBUG_PERM

# MPI
MPIEXEC =  mpiexec
PROCESSORS = 1
MPIFLAGS = -n $(PROCESSORS)

# INPUT
#TESTFILE = graphs/graph6_full.txt
TESTFILE = graphs/graph5.txt
#TESTFILE = graphs/graph2.txt
#TESTLOG = $(subst txt,log,$(TESTFILE))


all: clean build run

run: $(EXECUTABLE) $(TESTFILE)
	$(MPIEXEC) $(MPIFLAGS) $(EXECUTABLE) $(TESTFILE)
	cat output.txt
#	time mpirun $(MPIFLAGS) $(EXECUTABLE) $(TESTFILE) |  tee -a $(TESTLOG)
#	echo "=========================" >> $(TESTLOG)
        
build: $(SOURCEFILE)
	$(COMPILER) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) $<

clean:
	rm -f $(EXECUTABLE) *.log

