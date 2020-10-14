CFLAGS = --std=c++20 -g -Wall -Wextra -Wshadow -Wuninitialized -O3 -fshort-enums
CC     = g++-10
LINK   = g++-10
HCUBE_FILES = src/hypercube.tpp

$(shell mkdir -p bin obj)

help:
	@echo "\"make run_X size=N\" to compile and run siabX on a given size, for X in (1,2,3,4)"
	@echo "siab4 requires an extra argument, guess=G, of an estimate of the answer."
	@echo "\"make clean\" to remove all generated binary files"

run_1: bin/siab1_$(size)
	./bin/siab1_$(size)

run_2: bin/siab2_$(size)
	./bin/siab2_$(size)

run_3: bin/siab3_$(size)
	./bin/siab3_$(size)

run_4: bin/siab4_$(size)
	./bin/siab4_$(size) $(guess)

bin/siab1_$(size): obj/siab1_$(size).o
bin/siab2_$(size): obj/siab2_$(size).o
bin/siab3_$(size): obj/siab3_$(size).o obj/equivRelation.o
bin/siab4_$(size): obj/siab4_$(size).o obj/hypercube.o

bin/%:
	$(LINK) $^ -o $@

obj/siab1_$(size).o: src/siab1.cpp $(HCUBE_FILES)
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/siab2_$(size).o: src/siab2.cpp $(HCUBE_FILES)
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/siab3_$(size).o: src/siab3.cpp src/equivRelation.hpp src/permutation.hpp
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/siab4_$(size).o: src/siab4.cpp src/hypercube.hpp src/permutation.hpp
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/%.o: src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) $< -o $@ -c

src/%.hpp: ;

clean: 
	rm -f bin/* obj/*
