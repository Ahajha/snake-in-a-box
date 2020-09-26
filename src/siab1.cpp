/*
This is a hastily thrown together program to get a basic idea of how the
naive algorithm performs.
Inputs 1-5 finish essentially instantly (the result for 1 is not correct,
though we don't really care here), 6 might finish eventually but would
take a little bit, not sure how long exactly.
*/

#include <vector>
#include <iostream>

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

std::ostream& operator<<(std::ostream& stream, const hypercube& h)
{
	for (unsigned i = 0; i < h.vertices.size(); i++)
	{
		stream << i << ':';
		for (unsigned adj : h.vertices[i].adjList)
		{
			stream << ' ' << adj;
		}
		stream << std::endl;
	}
	return stream;
}

// Does a naive depth-first search for the largest induced path
void enumerate(hypercube& h, unsigned lastAddition)
{
	static unsigned maxNumInduced = 0;
	
	if (h.numInduced > maxNumInduced)
	{
		maxNumInduced = h.numInduced;
		std::cout << maxNumInduced << std::endl;
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
	
	//std::cout << h << std::endl;
	
	h.induce(0);

	enumerate(h,0);
}
