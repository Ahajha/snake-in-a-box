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

compile_3: bin/siab3_$(size)

bin/siab1: src/siab1.cpp
	$(CC) $(CFLAGS) -o $@ src/siab1.cpp

bin/siab2: src/siab2.cpp
	$(CC) $(CFLAGS) -o $@ src/siab2.cpp

bin/siab3_$(size): obj/equivRelation.o src/siab3.cpp
	$(CC) $(CFLAGS) -o $@ -D MAX_DIM=$(size) obj/equivRelation.o src/siab3.cpp

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f bin/* obj/*
