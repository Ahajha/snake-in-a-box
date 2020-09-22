#ifndef EQUIV_HPP
#define EQUIV_HPP

#include <vector>
#include <iostream>
#include <compare>
#include <unordered_map>

class equivRelation
{
	public:
	
	equivRelation();
	
	equivRelation(unsigned);
	
	// Merge two elements
	void merge(unsigned, unsigned);
	
	// Checks if two elements are equivalent
	bool equivalent(unsigned, unsigned) const;
	
	// Appends an element to the back of this ER, not equivalent
	// to any other element, and in its own new group.
	void addElement();
	
	bool operator==(const equivRelation&) const;
	std::strong_ordering operator<=>(const equivRelation&) const;
	
	// Gets all of the different possible ERs of a given size.
	static std::vector<equivRelation> enumerate(unsigned);
	
	// Returns the reverse of this relation. The reversal satisfies
	// the property that this.equiv(x,y) iff this.reverse().equiv(n-x,n-y).
	equivRelation reverse() const;
	
	// Returns the result of appending another ER to this one. Both sets
	// of numbers will be mutually non-equivalent.
	equivRelation append(const equivRelation&) const;
	
	// Similar to append, but modifies this instead.
	void operator+=(const equivRelation&);
	
	// Returns the result of removing a given number of items from the
	// end of this ER.
	equivRelation shave(unsigned) const;

	// Returns the canonical group labeling of this.
	std::vector<unsigned>& canonicalGroupLabeling() const;

	// Prints the canonical group labeling of this ER to an output stream.
	friend std::ostream& operator<<(std::ostream&, const equivRelation&);
	
	// Returns the number of elements in this ER.
	unsigned size() const;
	
	// Returns the number of non-equivalent components of this ER.
	unsigned numComponents() const;
	
	// Returns true if this is 'finer' than another ER, false if not.
	// Assumes this and the other ER are the same size.
	// Based on definition given at
	// https://en.wikipedia.org/wiki/Equivalence_relation
	bool finerThan(const equivRelation&) const;
	
	// Returns true if this is 'coarser' than another ER, false if not.
	bool coarserThan(const equivRelation&) const;
	
	private:
	
	unsigned leader(unsigned) const;
	
	struct element
	{
		unsigned numConstituents;
		
		// Mutable, since the leader function should appear
		// const, but technically isn't.
		mutable unsigned boss;
		
		element() {}
	
		element(unsigned index) :
			numConstituents(1), boss(index) {}
	};
	
	std::vector<element> elements;
	
	mutable std::vector<unsigned> cgl;
	mutable bool changed;
	
	void updateCGL() const;
};

// Provides a hash function.
struct er_hash
{
	// std::size_t is to adhere to the std::hash standard
	std::size_t operator()(const equivRelation&) const;
};

// Provides a central area to store all ER's. Provides constant
// time exchanges between ERs and indexes.
class er_storage
{
	public:
	
	er_storage();
	
	// Gets the ER at a given index.
	const equivRelation& operator[](unsigned) const;
	
	// If the ER does not exist, adds it and gives
	// its new index.
	
	// This uses () instead of [] to avoid warnings
	// related to overloading of the same operator
	// (somehow an unsigned and equivRelation are
	// too similar, and is ambiguous)
	unsigned operator()(const equivRelation&);
	
	private:
	
	// Basic structure is first split by ER size,
	// then hash to get the index.
	// The size of this vector should be at least one more than the size of the
	// largest ER stored in it. (5 elements -> index 5, which requires 6 elements)
	std::vector<std::unordered_map<equivRelation,unsigned,er_hash>> table;
	
	std::vector<equivRelation> ers;
};

#endif
