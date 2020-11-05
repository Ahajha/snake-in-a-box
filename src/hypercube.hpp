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
		uint8_t effectiveDegree;
		
		vertex() : induced(false), effectiveDegree(0) {}
	};
	
	std::array<vertex,numVertices> vertices;
	unsigned numInduced;
	
	hypercube();
	
	// Reduce is the inverse of induce.
	void induce(unsigned);
	void reduce(unsigned);
	
	bool operator==(const hypercube& other) const;
	
	// Prints which vertices are induced as Xs, and those that are not
	// as _s, all on one line.
	template<unsigned M>
	friend std::ostream& operator<<(std::ostream& stream, const hypercube<M>& h);
};

#include "hypercube.tpp"

#endif
