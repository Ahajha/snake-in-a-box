CFLAGS = --std=c++20 -g -Wall -Wextra -Wshadow -Wuninitialized -O3 -fshort-enums
CC     = g++-10

CFILES = src/siab3\
         src/equivRelation.cpp

$(shell mkdir -p bin obj)

help:
	@echo "\"make all size=N\" to compile for a given size"
	@echo "\"make run size=N\" to compile and run on a given size"
	@echo "\"make clean\" to remove all generated binary files"

all: bin/siab3_$(size)

run: bin/siab3_$(size)
	./bin/siab3_$(size)

bin/siab3_$(size): obj/equivRelation.o src/siab3.cpp
	$(CC) $(CFLAGS) -o $@ -D MAX_DIM=$(size) obj/equivRelation.o src/siab3.cpp

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f bin/* obj/*
