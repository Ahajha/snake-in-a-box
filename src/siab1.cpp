/*
This is a hastily thrown together program to get a basic idea of how the
naive algorithm performs.
Inputs 1-5 finish essentially instantly (the result for 1 is not correct,
though we don't really care here), 6 might finish eventually but would
take a little bit, not sure how long exactly.
*/

#include <vector>
#include <iostream>
#include "hypercube.hpp"

// Does a naive depth-first search for the largest induced path
void enumerate(hypercube& h, unsigned lastAddition)
{
	static unsigned maxNumInduced = 0;
	
	if (h.numInduced > maxNumInduced)
	{
		maxNumInduced = h.numInduced;
		std::cout << h;
	}

	for (unsigned adj : h.vertices[lastAddition].adjList)
	{
		// We need to check that the neighbor isn't induced specifically
		// for the case of the starting vertex's first expansion.
		if (h.vertices[adj].effectiveDegree == 1 && !h.vertices[adj].induced)
		{
			h.induce(adj);
			enumerate(h,adj);
			h.reduce(adj);
		}
	}
}

int main(int argn, char** args)
{
	if (argn == 1)
	{
		std::cerr << "Error: requires a nonnegative integer as an argument"
			<< std::endl;
		return 1;
	}
	
	unsigned d = atoi(args[1]);
	
	hypercube h(d);
	
	h.induce(0);

	enumerate(h,0);
}
