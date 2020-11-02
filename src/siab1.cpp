/*
This is a simple program to get a basic idea of how the naive algorithm
performs. Inputs 1-5 finish essentially instantly, 6 finishes in about
5 seconds, see no need to test any further with this algorithm.
*/

#include <iostream>
#include "hypercube.hpp"

// A macro named "MAX_DIM" will be compiled in.

// Does a naive depth-first search for the largest induced path
void enumerate(hypercube<MAX_DIM>& h, unsigned lastAddition)
{
	static unsigned maxNumInduced = 0;
	
	if (h.numInduced > maxNumInduced)
	{
		maxNumInduced = h.numInduced;
		std::cout << h;
	}

	for (unsigned adj : hypercube<MAX_DIM>::adjLists[lastAddition])
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

int main()
{
	hypercube<MAX_DIM> h;
	
	h.induce(0);

	enumerate(h,0);
}
