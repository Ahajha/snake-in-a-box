CFLAGS = --std=c++20 -g -Wall -Wextra -Wshadow -Wuninitialized -O3 -fshort-enums
CC     = g++-10
LINK   = g++-10

$(shell mkdir -p bin obj)

help:
	@echo "\"make run_X size=N\" to compile and run siabX on a given size, for X in (1,2,3)"
	@echo "\"make clean\" to remove all generated binary files"

run_1: bin/siab1
	./bin/siab1 $(size)

run_2: bin/siab2
	./bin/siab2 $(size)

run_3: bin/siab3_$(size)
	./bin/siab3_$(size)

run_4: bin/siab4_$(size)
	./bin/siab4_$(size)

bin/siab1: obj/siab1.o obj/hypercube.o
bin/siab2: obj/siab2.o obj/hypercube.o
bin/siab3_$(size): obj/siab3_$(size).o obj/equivRelation.o
bin/siab4_$(size): obj/siab4_$(size).o obj/hypercube.o

bin/%:
	$(LINK) $^ -o $@

obj/siab3_$(size).o: src/siab3.cpp src/equivRelation.hpp src/permutation.hpp
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/siab4_$(size).o: src/siab4.cpp src/hypercube.hpp src/permutation.hpp
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/%.o: src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) $< -o $@ -c

src/%.hpp: ;

clean: 
	rm -f bin/* obj/*
