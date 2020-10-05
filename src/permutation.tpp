#include "permutation.hpp"

template<unsigned N>
void permutationSet<N>::init()
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
				for (unsigned j = 0; j < (1u << i); j++)
				{
					perms[index][ind++] = subPerm[subind1++];
				}
				for (unsigned j = 0; j < (1u << i); j++)
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
				for (unsigned j = 0; j < (1u << i); j++)
				{
					perms[index][ind++] = subPerm.size() + subPerm[subind1++];
				}
				for (unsigned j = 0; j < (1u << i); j++)
				{
					perms[index][ind++] = subPerm[subind2++];
				}
			}
			
			++index;
		}
	}
}

void permutationSet<0>::init()
{
	perms = {{0}};
}
