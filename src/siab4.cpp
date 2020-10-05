#include <array>
#include <exception>
#include "hypercube.hpp"

// Empty is a non-induced vertex that is valid to induce
// Invalid is non-induced, not allowed to induce
// Induced is an induced vertex.
enum pointType { empty, invalid, induced };

// MAX_DIM will be compiled in

#if MAX_DIM == 1
	#error siab4 only supports dimensions 2+
#endif

#define ARR_SIZE (1 << MAX_DIM)

static inline hypercube h(MAX_DIM);

pointType intersect(pointType p1, pointType p2)
{
	// If either point is empty, the point just becomes
	// whatever the other one is.
	if (p1 == empty) return p2;
	if (p2 == empty) return p1;
	
	// At this point, every point is either induced or
	// invalid. If both points are invalid, then the
	// new point is invalid.
	if (p1 == invalid && p2 == invalid) return invalid;
	
	// Otherwise, at least one of the points is induced,
	// and the other is either induced or invalid. This
	// invalidates the configuration.
	throw std::exception();
}

bool adjacent(unsigned v1, unsigned v2)
{
	// Return true if the XOR of both vertices
	// is a power of 2 (since they should differ
	// by 1 bit).
	unsigned x = v1 ^ v2;
	return x != 0 && ((x & (x - 1)) == 0);
}

std::ostream& operator<<(std::ostream& stream, pointType p)
{
	return stream << (p == empty ? '_' : (p == invalid ? '^' : 'X'));
}

struct snake
{
	unsigned numVertices;
	std::array<pointType, ARR_SIZE> points;
	std::array<unsigned,2> endpoints;
	
	// Constructor for 1,2,3-snakes. These are all special cases in that
	// they all have exactly one canonical form of that size, and they
	// easily constructable. This is also to avoid constructing 2 and 3-snakes
	// since those are the only cases that use 1-snakes, which have only one
	// endpoint. In the other constructor, we will assume there are two
	// distinct endpoints.
	snake(unsigned nv) : numVertices(nv)
	{
		points.fill(empty);
		
		if (nv == 1)      endpoints = { ARR_SIZE - 1, ARR_SIZE - 1 };
		else if (nv == 2) endpoints = { ARR_SIZE - 1, ARR_SIZE - 2 };
		else              endpoints = { ARR_SIZE - 2, ARR_SIZE - 3 };
		
		for (unsigned i = 0; i < nv; i++)
		{
			points[ARR_SIZE - i - 1] = induced;
		}
		
		// Mark the vertices around the middle vertex as invalid, except for the endpoints.
		if (nv == 3)
		{
			for (unsigned adj : h.vertices[ARR_SIZE - 1].adjList)
			{
				if (points[adj] != induced)
				{
					points[adj] = invalid;
				}
			}
		}
	}
	
	std::pair<unsigned,unsigned> getMergedEndpoints(const snake& s1, const snake& s2)
	{
		bool matchFound = false;
		std::pair<unsigned, unsigned> result;
		for (unsigned ep1 : {0u,1u})
		{
			for (unsigned ep2 : {0u,1u})
			{
				if (adjacent(s1.endpoints[ep1],s2.endpoints[ep2]))
				{
					if (matchFound)
					{
						throw std::exception();
					}
					
					result = { ep1, ep2 };
					
					matchFound = true;
				}
			}
		}
		if (!matchFound) throw std::exception();
		
		return result;
	}
	
	snake(const snake& s1, const snake& s2) :
		numVertices(s1.numVertices + s2.numVertices)
	{
		auto [ep1,ep2] = getMergedEndpoints(s1,s2);
		
		endpoints[0] = s1.endpoints[!ep1];
		endpoints[1] = s2.endpoints[!ep2];
		
		for (unsigned i = 0; i < ARR_SIZE; i++)
		{
			points[i] = intersect(s1.points[i], s2.points[i]);
		}
		
		// Mark any neighbors of the old endpoints as invalid, since
		// these endpoints are now midpoints.
		for (unsigned adj : h.vertices[s1.endpoints[ep1]].adjList)
		{
			if (points[adj] == empty) points[adj] = invalid;
		}
		
		for (unsigned adj : h.vertices[s2.endpoints[ep2]].adjList)
		{
			if (points[adj] == empty) points[adj] = invalid;
		}
	}
	
	friend std::ostream& operator<<(std::ostream& stream, const snake& snake)
	{
		for (pointType p : snake.points)
		{
			stream << p << ' ';
		}
		return stream << "| " << snake.numVertices << " vertices";
	}
};

int main()
{
	std::array<std::vector<snake>,ARR_SIZE + 1> snakeClasses;
	
	for (unsigned i = 1; i <= 3 && i < snakeClasses.size(); i++)
	{
		snakeClasses[i] = { snake(i) };
	}
	
	// This currently does not do anything, since it's only going
	// to be squashing snakes on top of themselves. Need permutations.
	for (unsigned i = 4; i < snakeClasses.size(); i++)
	{
		unsigned nv1 = i/2, nv2 = (i + 1)/2;
		
		//std::cout << "i = " << i << ", nv1 = " << nv1 << ", nv2 = " << nv2 << std::endl;
		
		for (const auto& s1 : snakeClasses[nv1])
		{
			for (const auto& s2 : snakeClasses[nv2])
			{
				try
				{
					snakeClasses[i].emplace_back(s1,s2);
				}
				catch(std::exception&) {}
			}
		}
	}
	
	for (unsigned i = 0; i < snakeClasses.size(); i++)
	{
		if (!snakeClasses[i].empty())
		{
			std::cout << "Size " << i << ":\n";
			for (const auto& s : snakeClasses[i])
			{
				std::cout << s << std::endl;
			}
		}
	}
}
