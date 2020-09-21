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
#include "equivRelation.hpp"

// A macro named MAX_DIM will be compiled in.

typedef char componentNumType;

// Permutations of N-dimensional hypercube

template<unsigned N>
struct permutationSet
{
	typedef std::array<unsigned,(1 << N)> permutation;
	
	static inline std::array<permutation,
		permutationSet<N-1>::perms.size()*2*N> perms = {};
	
	static void init()
	{
		permutationSet<N-1>::init();
		
		unsigned index = 0;
		for (const auto& subPerm : permutationSet<N-1>::perms)
		{
			// This dimension is going to represent where the interleaving
			// occurs. Examples: (on 1234)
			// For i = 0, this would change every 2^0 = 1 times,
			// so this would be 15263748.
			// For i = 1, this would change every 2^1 = 2 times,
			// so this would be 12563478.
			// For i = 2, this would change every 2^2 = 4 times,
			// so this would be 12345678.
			// Then there is another version that is reversed,
			// for these three would be 51627384, 56218743, and 56781234.
			for (unsigned i = 0; i < N; i++)
			{
				unsigned ind = 0;
				unsigned subind1 = 0, subind2 = 0;
				while (ind < perms[0].size())
				{
					for (unsigned j = 0; j < (1 << i); j++)
					{
						perms[index][ind++] = subPerm[subind1++];
					}
					for (unsigned j = 0; j < (1 << i); j++)
					{
						perms[index][ind++] = subPerm.size() + subPerm[subind2++];
					}
				}
				
				++index;
				
				// Do the same thing, but swap the groups. This
				// could probably be cleaner without code duplication.
				
				ind = 0;
				subind1 = 0, subind2 = 0;
				while (ind < perms[0].size())
				{
					for (unsigned j = 0; j < (1 << i); j++)
					{
						perms[index][ind++] = subPerm.size() + subPerm[subind1++];
					}
					for (unsigned j = 0; j < (1 << i); j++)
					{
						perms[index][ind++] = subPerm[subind2++];
					}
				}
				
				++index;
			}
		}
		/*
		std::cout << N << ":" << std::endl;
		for (const auto perm : perms)
		{
			for (unsigned i : perm)
			{
				std::cout << i << ' ';
			}
			std::cout << std::endl;
		}
		*/
	}
};

template<>
struct permutationSet<0>
{
	typedef std::array<unsigned,1> permutation;
	static inline std::array<permutation,1> perms = {};

	static void init()
	{
		perms = {{0}};
	}
};

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
	std::size_t operator()(const subcube<N> sc) const
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

// Need a forward declaration for another forward declaration...
template<unsigned N>
struct subcubeClass;

template<unsigned N>
struct subcubeClassHash
{
	std::size_t operator()(const subcubeClass<N> sc) const;
};

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
	
	static inline std::vector<subcubeClass> classes = {};
	
	static void init()
	{
		subcubeClass<N-1>::init();
		
		for (const auto& subClass1 : subcubeClass<N-1>::classes)
		{
			for (const auto& subClass2 : subcubeClass<N-1>::classes)
			{
				for (const auto& sub2 : subClass2.instances)
				{
					try
					{
						const auto newClass = subcubeClass(subClass1.canonicalForm,sub2);
						
						for (const auto& existingClass : classes)
						{
							if (existingClass.canonicalForm == newClass.canonicalForm)
								// this is stupid, but just here for now
								throw std::exception();
						}
						
						classes.push_back(newClass);
					}
					catch (std::exception& e) {}
				}
			}
		}
		
		std::cout << N << ", " << (1 << N) << " vertices, "
			<< classes.size() << " classes" << std::endl << std::flush;
		/*
		for (unsigned i = 0; i < classes.size(); i++)
		{
			std::cout << i << ": " << classes[i].instances.size()
				<< " instances" << std::endl;
			for (const auto& instance : classes[i].instances)
			{
				std::cout << "    " << instance << std::endl;
			}
		}
		*/
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
	
	static inline std::vector<subcubeClass> classes = {};
	
	static void init()
	{
		classes = { subcubeClass(1), subcubeClass(0) };
	}
};

template<unsigned N>
std::size_t subcubeClassHash<N>::operator()(const subcubeClass<N> sc) const
{
	// Classes can just be identified by their canonical form.
	return subcubeHash<N>()(sc.canonicalForm);
}

int main()
{
	permutationSet<MAX_DIM>::init();
	
	subcubeClass<MAX_DIM>::init();
	
	subcube<MAX_DIM> largest;
	for (const auto& scc : subcubeClass<MAX_DIM>::classes)
	{
		if (scc.canonicalForm.numComponents == 1 &&
			scc.canonicalForm.numVertices > largest.numVertices)
		{
			largest = scc.canonicalForm;
		}
	}
	
	std::cout << largest << std::endl;
}
