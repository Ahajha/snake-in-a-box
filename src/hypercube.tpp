#include "hypercube.hpp"

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
bool hypercube<N>::operator==(const hypercube& other) const
{
	for (unsigned i = 0; i < numVertices; ++i)
	{
		if (vertices[i].induced != other.vertices[i].induced)
		{
			return false;
		}
	}
	return true;
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
