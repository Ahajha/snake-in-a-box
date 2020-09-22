// g++-10 --std=c++20 -O3 (with or without -g) test.cpp -- Fail
// g++-10 --std=c++20 (with or without -g) test.cpp -- Pass

#include <array>

template<unsigned N>
struct A
{
	static inline std::array<unsigned,A<N-1>::C.size()*2*N*(1 << N)> C = {};
	
	static void init()
	{
		C[0] = A<N-1>::C[0];
	}
};

template<>
struct A<0>
{
	static inline std::array<unsigned,1> C= {0};
	static void init() {}
};

int main()
{
	A<8>::init();
}
