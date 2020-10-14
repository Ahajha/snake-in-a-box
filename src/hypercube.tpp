#ifndef HYPERCUBE_HPP
#define HYPERCUBE_HPP

#include <array>
#include <iostream>

// Hypercube graph capable of keeping track of
// induced vertices and degrees of vertices
// with respect to how many induced neighbors
// vertices have.
template <unsigned N>
struct hypercube
{
	constexpr static unsigned numVertices = 1 << N;
	
	private:
	constexpr static std::array<std::array<unsigned, N>, numVertices> makeAdjLists();
	
	public:
	
	// Format of array is [i] gives the adjacency list of vertex i, and
	// [i][j] gives the vertex obtained by starting from vertex i and
	// moving across dimension j.
	constexpr static auto adjLists = makeAdjLists();
	
	struct vertex
	{
		bool induced;
		unsigned effectiveDegree;
		
		vertex() : induced(false), effectiveDegree(0) {}
	};
	
	std::array<vertex,numVertices> vertices;
	unsigned numInduced;
	
	hypercube();
	
	// Reduce is the inverse of induce.
	void induce(unsigned);
	void reduce(unsigned);
	
	// Prints which vertices are induced as Xs, and those that are not
	// as _s, all on one line.
	template<unsigned M>
	friend std::ostream& operator<<(std::ostream& stream, const hypercube<M>& h);
};

template<unsigned N>
hypercube<N>::hypercube() : numInduced(0) {}

template<unsigned N>
constexpr std::array<std::array<unsigned, N>, hypercube<N>::numVertices>
	hypercube<N>::makeAdjLists()
{
	std::array<std::array<unsigned, N>, numVertices> adjLists;
	
	for (unsigned i = 0; i < numVertices; i++)
	{
		for (unsigned j = 0; j < N; j++)
		{
			adjLists[i][j] = i ^ (1 << j);
		}
	}
	
	return adjLists;
}

template<unsigned N>
void hypercube<N>::induce(unsigned i)
{
	vertices[i].induced = true;
	++numInduced;
	for (unsigned adj : adjLists[i])
	{
		++vertices[adj].effectiveDegree;
	}
}

template<unsigned N>
void hypercube<N>::reduce(unsigned i)
{
	vertices[i].induced = false;
	--numInduced;
	for (unsigned adj : adjLists[i])
	{
		--vertices[adj].effectiveDegree;
	}
}

template<unsigned N>
std::ostream& operator<<(std::ostream& stream, const hypercube<N>& h)
{
	for (const auto& v : h.vertices)
	{
		stream << (v.induced ? "X " : "_ ");
	}
	return stream << "| " << h.numInduced << " vertices" << std::endl;
}

#endif
