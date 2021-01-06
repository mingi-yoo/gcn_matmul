all: gcn
gcn : gcn.o
	g++ -o gcn gcn.o
gcn.o : gcn.cpp
	g++ -std=c++11 -c -o gcn.o gcn.cpp gcn.h
clean:
	rm gcn.o gcn