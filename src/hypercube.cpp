#include "hypercube.hpp"

hypercube::hypercube(unsigned d) : vertices(1 << d), numInduced(0)
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

void hypercube::induce(unsigned i)
{
	vertices[i].induced = true;
	++numInduced;
	for (unsigned adj : vertices[i].adjList)
	{
		++vertices[adj].effectiveDegree;
	}
}

// Inverse of induce
void hypercube::reduce(unsigned i)
{
	vertices[i].induced = false;
	--numInduced;
	for (unsigned adj : vertices[i].adjList)
	{
		--vertices[adj].effectiveDegree;
	}
}

void hypercube::printAdjList() const
{
	for (unsigned i = 0; i < vertices.size(); i++)
	{
		std::cout << i << ':';
		for (unsigned adj : vertices[i].adjList)
		{
			std::cout << ' ' << adj;
		}
		std::cout << std::endl;
	}
}

std::ostream& operator<<(std::ostream& stream, const hypercube& h)
{
	for (const auto& v : h.vertices)
	{
		stream << (v.induced ? "X " : "_ ");
	}
	return stream << "| " << h.numInduced << " vertices" << std::endl;
}
