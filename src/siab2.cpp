/*
This is another simple program to demonstrate a slightly less naive algorithm,
where we use dimension strings to prune some symmetries present in the first
naive approach.

At each step, we keep track of a 'string' of dimensions. We are only allowed to
move in a given dimension after moving in all previous ones at least once. Thus,
we must also keep track of the maximum dimension already used.

This is a bit better, d=6 finishes much more quickly, but is unable to finish 7.
*/

#include <iostream>
#include "hypercube.tpp"

// A macro named "MAX_DIM" will be compiled in.

// Does a naive depth-first search for the largest induced pat
void enumerate(hypercube<MAX_DIM>& h, unsigned lastAddition, unsigned highestDim = 0)
{
	static unsigned maxNumInduced = 0;
	
	if (h.numInduced > maxNumInduced)
	{
		maxNumInduced = h.numInduced;
		std::cout << h;
	}
	
	// By the nature of the order of the vertices, index in the adjacency
	// list is also the dimension number.
	unsigned stop = std::min((unsigned)MAX_DIM,highestDim + 1);
	for (unsigned i = 0; i < stop; i++)
	{
		unsigned adj = hypercube<MAX_DIM>::adjLists[lastAddition][i];
		
		// We need to check that the neighbor isn't induced specifically
		// for the case of the starting vertex's first expansion.
		if (h.vertices[adj].effectiveDegree == 1 && !h.vertices[adj].induced)
		{
			h.induce(adj);
			
			enumerate(h,adj,highestDim + (i == highestDim));
			
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
