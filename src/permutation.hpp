#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP

// Permutations of N-dimensional hypercube.
// The permutations cannot be assumed to be
// in any specific order, and may change
// in future versions.

// Before accessing a permutation set,
// you should first call permutationSet<X>::init()
// for only the largest value of X that is needed,
// any smaller values are also filled in.

// Known issue: This does not compile for N >= 8
// and with -O3 due to stack size issues.

template<unsigned N>
struct permutationSet
{
	typedef std::array<unsigned,(1 << N)> permutation;
	
	static inline std::array<permutation,
		permutationSet<N-1>::perms.size()*2*N> perms = {};
	
	static void init();
};

template<>
struct permutationSet<0>
{
	typedef std::array<unsigned,1> permutation;
	
	static inline std::array<permutation,1> perms = {};

	static void init();
};

#include "permutation.tpp"

#endif
