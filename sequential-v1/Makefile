#debugging (libovolna hodnota), jinak zakomentovat

#ktery soubor pouzivame
EXECUTABLE = main
PROCESSORS = 2

SOURCEFILE = $(EXECUTABLE).cpp
CFLAGS = -Wall -pedantic
#CFLAGS += -D DEBUG_INPUT
#CFLAGS += -D DEBUG_DIST
#CFLAGS += -D DEBUG_STACK
#CFLAGS += -D DEBUG_NET
#CFLAGS += -D DEBUG_PERM
MPIFLAGS = -n $(PROCESSORS)
TESTFILE = test.txt
#TESTFILE = test2.txt
TESTLOG = $(subst txt,log,$(TESTFILE))



all: clean build run

run: $(EXECUTABLE) $(TESTFILE)
#	time mpirun $(MPIFLAGS) $(EXECUTABLE) $(TESTFILE) |  tee -a $(TESTLOG)
#	echo "=========================" >> $(TESTLOG)

        
build: $(SOURCEFILE)
#	mpic++ $(CFLAGS) -o $(EXECUTABLE) $<
	g++ $(CFLAGS) -o $(EXECUTABLE) $<

clean:
	rm -f $(EXECUTABLE) *.log

