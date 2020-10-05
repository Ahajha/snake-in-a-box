/*
We'll call the pieces that get stuck together 'subcubes'.

A dimension N subcube is just a hypercube of dimension N
that is 'eligible' to be combined with another dim. N
subcube to make an 'induced subtrail'.

The requirements are that all induced vertices have a
degree of at most 2, and there are no cycles.
*/

/*
Important thing to prove right now, if this is
true then a lot of improvements can be made:

A form is canonical iff no elementary operations
produce a smaller form.

'=>' is obvious, this is the definition of canonical.

'<=' is the tricky part.
*/

/*
I still have this ongoing issue with 'relocation truncated to fit',
though if the above is true the permutationSet class may not be needed.
*/

/*
Another optimization that needs to be done is the greedy approach:
Only generate forms that are as small as needed. As a made-up example,
suppose we want to find the solution for N=5, and we know that the
maximum induced trail-in-a-box for N=4 is 20. Then we should start by
pairing 2 20s together, and if that succeeds, we are done. If not,
try a 19+20, and so on. This also works recursively, the 19s need not
be generated until they are called on, and this carries on down the stack.
*/

#include <vector>
#include <iostream>
#include <array>
#include <exception>
#include <unordered_set>
#include <ctime>
#include "equivRelation.hpp"
#include "permutation.hpp"

// A macro named MAX_DIM will be compiled in.
// Size 0 causes issues with template specializations,
// so we ignore this case.
#if MAX_DIM == 0
	#error size should be positive
#endif

typedef char componentNumType;

// Represents, respectively: A non-induced vertex,
// an induced vertex with 2 neighbors, an induced
// vertex with 1 neighbor, an induced vertex with 0
// neighbors.
// Empty is 'false', which will make everything else true.
// This simplifies some things later on.
enum pointType { empty = 0, point = 1, endpoint = 2, midpoint = 3 };

// Returns the new type of a vertex after connecting with another
// type of vertex.
pointType getNewType(pointType current, pointType connection)
{
	switch (current)
	{
		case endpoint:
			return connection == empty ? endpoint : midpoint;
		case point:
			return connection == empty ? point : endpoint;
		default:
			return current;
	}
}

template<unsigned N>
struct subcube
{
	// Point type: Represents, respectively: A non-induced vertex,
	// an induced vertex with 2 neighbors, an induced
	// vertex with 1 neighbor, an induced vertex with 0
	// neighbors.
	// ComponentNumType: Undefined if the vertex is not
	// induced. Otherwise is the number
	// of the component it represents.
	std::array<std::pair<pointType,componentNumType>,(1 << N)> verts;
	unsigned numComponents;
	unsigned numVertices;
	
	// Throws an exception if the pairing is invalid
	subcube(const subcube<N-1>& sub1, const subcube<N-1>& sub2)
	{
		equivRelation er(sub1.numComponents + sub2.numComponents);
		
		for (unsigned i = 0; i < sub1.verts.size(); i++)
		{
			// Look for any instances of a midpoint connecting with anything
			// other than an empty space
			if ((sub1.verts[i].first == midpoint && sub2.verts[i].first) ||
			    (sub2.verts[i].first == midpoint && sub1.verts[i].first))
			{
				throw std::exception();
			}
			
			// Merge equivalence classes, if needed. Ensure there are no cycles.
			if (sub1.verts[i].first && sub2.verts[i].first)
			{
				if (er.equivalent(sub1.verts[i].second,
					sub1.numComponents + sub2.verts[i].second))
				{
					throw std::exception();
				}
				
				er.merge(sub1.verts[i].second,
					sub1.numComponents + sub2.verts[i].second);
			}
		}
		
		// Construct the form of the new subcube.
		const auto& cgl = er.canonicalGroupLabeling();
		
		for (unsigned i = 0; i < sub1.verts.size(); i++)
		{
			unsigned j = i + sub1.verts.size();
		
			verts[i].first = getNewType(sub1.verts[i].first, sub2.verts[i].first);
			verts[j].first = getNewType(sub2.verts[i].first, sub1.verts[i].first);
			
			// Empty vertices will have 0 as the component number.
			verts[i].second = verts[i].first ?
				cgl[sub1.verts[i].second] : 0;
			verts[j].second = verts[j].first ?
				cgl[sub1.numComponents + sub2.verts[i].second] : 0;
		}
		
		numComponents = er.numComponents();
		numVertices = sub1.numVertices + sub2.numVertices;
	}
	
	subcube() : numVertices(0) {}
	
	subcube(const subcube& other, const permutationSet<N>::permutation& perm)
	{
		for (unsigned i = 0; i < verts.size(); i++)
		{
			verts[i] = other.verts[perm[i]];
		}
		
		numComponents = other.numComponents;
		numVertices = other.numVertices;
	}
	
	constexpr std::strong_ordering operator<=>(const subcube& other) const
	{
		for (unsigned i = 0; i < verts.size(); i++)
		{
			bool exists1 = verts[i].first;
			bool exists2 = other.verts[i].first;
			
			if (exists1 != exists2)
				return exists1 <=> exists2;
		}
		return std::strong_ordering::equal;
	}
	
	constexpr bool operator==(const subcube& other) const
	{
		for (unsigned i = 0; i < verts.size(); i++)
		{
			bool exists1 = verts[i].first;
			bool exists2 = other.verts[i].first;
			
			if (exists1 != exists2)
				return false;
		}
		return true;
	}
};

template<>
struct subcube<0>
{
	std::array<std::pair<pointType,componentNumType>,1> verts;
	unsigned numComponents;
	unsigned numVertices;
	
	subcube() : numVertices(0) {}
	
	subcube(unsigned v)
	{
		verts[0] = std::make_pair(v == 1 ? point : empty, 0);
		numComponents = v;
		numVertices = v;
	}
	
	constexpr std::strong_ordering operator<=>(const subcube& other) const
	{
		return verts[0] <=> other.verts[0];
	}
	
	constexpr bool operator==(const subcube& other) const
	{
		return verts[0] == other.verts[0];
	}
};

// Provides a hash function for subcube.
template<unsigned N>
struct subcubeHash
{
	std::size_t operator()(const subcube<N>& sc) const
	{
		std::size_t hash = 0;
		for (auto [ptype,compNum] : sc.verts)
		{
			hash = 2*hash + (ptype != empty);
		}
		return hash;
	}
};

template<unsigned N>
std::ostream& operator<<(std::ostream& stream, const subcube<N>& sc)
{
	for (auto [ptype,compNum] : sc.verts)
	{
		if (ptype == empty)
		{
			stream << "_ ";
		}
		else
		{
			stream << (int)compNum << ' ';
		}
	}
	stream << "| " << sc.numComponents << " components, "
		<< sc.numVertices << " vertices";
	
	return stream;
}

// Holds a group of subcubes that are symmetrically identical.

template<unsigned N>
struct subcubeClass
{
	subcube<N> canonicalForm;
	std::unordered_set<subcube<N>,subcubeHash<N>> instances;
	
	subcubeClass(const subcube<N-1>& sub1, const subcube<N-1>& sub2) :
		canonicalForm(sub1,sub2)
	{
		for (const auto& perm : permutationSet<N>::perms)
		{
			auto [iter,inserted] = instances.emplace(canonicalForm,perm);
			
			if (inserted && canonicalForm > *iter)
				throw std::exception();
		}
	}
};

template<>
struct subcubeClass<0>
{
	subcube<0> canonicalForm;
	std::vector<subcube<0>> instances;
	
	subcubeClass(unsigned v) : canonicalForm(v)
	{
		instances.emplace_back(v);
	}
};

template<unsigned N>
struct subcubeClassStorage
{
	// Each physical form is generated at most once, consequently canonical forms are
	// generated at most once, so we don't need to check for membership, and thus
	// we can use a vector rather than a set.
	static inline std::vector<std::vector<subcubeClass<N>>> sets = {};
	
	static void findLargestSet();
	
	static bool fill(int size);
};

template<unsigned N>
void subcubeClassStorage<N>::findLargestSet()
{
	subcubeClassStorage<N-1>::findLargestSet();
	
	unsigned largest = 2 * (subcubeClassStorage<N-1>::sets.size() - 1);
	
	sets.resize(largest + 1);
	
	while (fill(largest)) --largest;
	
	sets.resize(largest + 1);
}

template<>
void subcubeClassStorage<0>::findLargestSet()
{
	sets = { { subcubeClass<0>(0) }, { subcubeClass<0>(1) } };
}

template<>
void subcubeClassStorage<MAX_DIM>::findLargestSet()
{
	subcubeClassStorage<MAX_DIM-1>::findLargestSet();
	
	unsigned largest = 2 * (subcubeClassStorage<MAX_DIM-1>::sets.size() - 1);
	
	sets.resize(largest + 1);
	
	while (true)
	{
		fill(largest);
		
		for (const auto& scc : sets[largest])
		{
			if (scc.canonicalForm.numComponents == 1)
			{
				std::cout << scc.canonicalForm << std::endl;
				return;
			}
		}
		
		// We don't need to keep these, they are useless now.
		// TODO: We don't really need to put them anywhere to begin with.
		sets[largest].clear();
		--largest;
	}
}

template<unsigned N>
bool subcubeClassStorage<N>::fill(int size)
{
	// This takes care of the case of the dimension
	// being fully enumerated already, or if the set has already been filled.
	if (size < 0 || !sets[size].empty()) return false;
	
	std::cout << "Filling size = " << size << " for dimension " << N << std::endl;
	
	// For brevity
	const auto& smallerSets = subcubeClassStorage<N-1>::sets;
	
	int maxValue = std::min((int)(smallerSets.size() - 1), size);
	
	// Have 2 counters: One starts at the largest value that would be
	// valid, which is either the max number of vertices for the
	// previous dimension, or i itself, whichever is smaller. The other
	// is whatever value it needs to be for them to add to i.
	int s1 = maxValue, s2 = size - s1;
	
	while (s2 <= maxValue)
	{
		// I don't think a check for s1 is needed,
		// but it doesn't hurt to have for now.
		subcubeClassStorage<N-1>::fill(s1);
		subcubeClassStorage<N-1>::fill(s2);
		
		for (const auto& set1 : smallerSets[s1])
		{
			for (const auto& set2 : smallerSets[s2])
			{
				// If the second canonical form is larger, we can prune this,
				// since swapping them would certainly give a smaller result.
				if (set1.canonicalForm <= set2.canonicalForm)
				{
					for (const auto& instance2 : set2.instances)
					{
						try
						{
							sets[size].emplace_back(set1.canonicalForm, instance2);
						}
						catch(std::exception& e) {}
					}
				}
			}
		}
		
		--s1;
		++s2;
	}
	
	return sets[size].empty();
}

// Return value also doesn't matter (I think), so just say it isn't empty.
template<>
bool subcubeClassStorage<0>::fill(int) { return false; }

int main()
{
	auto startTime = clock();
	
	permutationSet<MAX_DIM>::init();
	
	subcubeClassStorage<MAX_DIM>::findLargestSet();
	
	std::cout << "Finished in " << (float)(clock()-startTime)/(CLOCKS_PER_SEC)
		<< " seconds" << std::endl;
}
