/*
This is another simple program to demonstrate a slightly less naive algorithm,
where we use dimension strings to prune some symmetries present in the first
naive approach.

At each step, we keep track of a 'string' of dimensions. We are only allowed to
move in a given dimension after moving in all previous ones at least once. Thus,
we must also keep track of the maximum dimension already used.

This is a bit better, d=6 finishes much more quickly, but is unable to finish 7.
*/

#include <vector>
#include <iostream>

// Number of dimensions.
unsigned dims;

struct vertex
{
	std::vector<unsigned> adjList;
	
	bool induced;
	unsigned effectiveDegree;
	
	vertex() : adjList(), induced(false), effectiveDegree(0) {}
};

// Does a few things.
// 1: Specifically a hypercube graph
// 2: Keeps track of induced vertices
struct hypercube
{
	std::vector<vertex> vertices;
	unsigned numInduced;
	
	// Constructs a hypercube of a given dimension.
	hypercube(unsigned d) : vertices(1 << d), numInduced(0)
	{
		for (unsigned i = 0; i < vertices.size(); i++)
		{
			// Each vertex has exactly d neighbors, each of which
			// can be found by taking i and changing one bit.
			for (unsigned j = 0; j < d; j++)
			{
				vertices[i].adjList.push_back(i ^ (1 << j));
			}
		}
	}
	
	void induce(unsigned i)
	{
		vertices[i].induced = true;
		++numInduced;
		for (unsigned adj : vertices[i].adjList)
		{
			++vertices[adj].effectiveDegree;
		}
	}
	
	// Inverse of induce
	void reduce(unsigned i)
	{
		vertices[i].induced = false;
		--numInduced;
		for (unsigned adj : vertices[i].adjList)
		{
			--vertices[adj].effectiveDegree;
		}
	}
};

// Does a naive depth-first search for the largest induced pat
void enumerate(hypercube& h, unsigned lastAddition, unsigned highestDim = 0)
{
	static unsigned maxNumInduced = 0;
	static std::vector<unsigned> dimString;
	
	if (h.numInduced > maxNumInduced)
	{
		maxNumInduced = h.numInduced;
		std::cout << maxNumInduced << ':';
		for (unsigned n : dimString)
		{
			std::cout << ' ' << n;
		}
		std::cout << std::endl;
	}
	
	// By the nature of the order of the vertices, index in the adjacency
	// list is also the dimension number.
	unsigned stop = std::min(dims,highestDim + 1);
	for (unsigned i = 0; i < stop; i++)
	{
		unsigned adj = h.vertices[lastAddition].adjList[i];
		
		// We need to check that the neighbor isn't induced specifically
		// for the case of the starting vertex's first expansion.
		if (h.vertices[adj].effectiveDegree == 1 && !h.vertices[adj].induced)
		{
			h.induce(adj);
			dimString.push_back(i);
			
			enumerate(h,adj,(i == highestDim) ? highestDim + 1 : highestDim);
			
			h.reduce(adj);
			dimString.pop_back();
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
	
	dims = atoi(args[1]);
	
	hypercube h(dims);
	
	h.induce(0);

	enumerate(h,0);
}
