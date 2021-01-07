all: gcn
gcn : gcn.cpp
	g++ -o gcn gcn.cpp gcn.h
clean:
	rm -r -f gcn