CFLAGS = --std=c++20 -g -Wall -Wextra -Wshadow -Wuninitialized -O3 -fshort-enums
CC     = g++-10

CFILES = src/siab3\
         src/equivRelation.cpp

$(shell mkdir -p bin obj)

help:
	@echo "\"make run_X size=N\" to compile and run siabX on a given size, for X in (1,2,3)"
	
	@echo "\"make compile_3 size=N\" to compile siab3 for a given size"
	
	@echo "\"make clean\" to remove all generated binary files"

run_1: bin/siab1
	./bin/siab1 $(size)

run_2: bin/siab2
	./bin/siab2 $(size)

run_3: bin/siab3_$(size)
	./bin/siab3_$(size)

run_4: bin/siab4_$(size)
	./bin/siab4_$(size)

compile_3: bin/siab3_$(size)

compile_4: bin/siab4_$(size)

bin/siab1: obj/siab1.o obj/hypercube.o
	$(CC) $(CFLAGS) $^ -o $@

bin/siab2: obj/siab2.o obj/hypercube.o
	$(CC) $(CFLAGS) $^ -o $@

bin/siab3_$(size): obj/siab3.o obj/equivRelation.o
	$(CC) $(CFLAGS) $^ -o $@

bin/siab4_$(size): obj/siab4.o obj/hypercube.o
	$(CC) $(CFLAGS) $^ -o $@

obj/siab3.o: src/siab3.cpp src/equivRelation.hpp src/permutation.hpp
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/siab4.o: src/siab4.cpp src/hypercube.hpp
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/%.o: src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) $< -o $@ -c

src/%.hpp: ;

clean: 
	rm -f bin/* obj/*
