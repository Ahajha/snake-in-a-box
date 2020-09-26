#ifndef HYPERCUBE_HPP
#define HYPERCUBE_HPP

#include <vector>
#include <iostream>

// Does a few things.
// 1: Specifically a hypercube graph
// 2: Keeps track of induced vertices
struct hypercube
{
	struct vertex
	{
		std::vector<unsigned> adjList;
		
		bool induced;
		unsigned effectiveDegree;
		
		vertex() : adjList(), induced(false), effectiveDegree(0) {}
	};
	
	std::vector<vertex> vertices;
	unsigned numInduced;
	
	// Constructs a hypercube of a given dimension.
	hypercube(unsigned d);
	
	
	void induce(unsigned i);
	void reduce(unsigned i);
	
	// Prints the adjacency lists, for debugging
	void printAdjList() const;
};

// Prints which vertices are induced as Xs, and those that are not
// as _s, all on one line.
std::ostream& operator<<(std::ostream& stream, const hypercube& h);

#endif
