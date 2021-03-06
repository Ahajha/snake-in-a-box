CFLAGS = --std=c++20 -g -Wall -Wextra -Wshadow -Wuninitialized -O3 -fshort-enums
CC     = g++-10
LINK   = g++-10
HCUBE_FILES = src/hypercube.hpp   src/hypercube.tpp
PERMU_FILES = src/permutation.hpp src/permutation.tpp

$(shell mkdir -p bin obj)

help:
	@echo "\"make run_X size=N\" to compile and run siabX on a given size, for X in (1,2,3,4)"
	@echo "siab4 requires an extra argument, guess=G, of an estimate of the answer."
	@echo
	@echo "\"make count size=N pruned=P semipruned=S\" to compile and run count-forms"
	@echo "on a given size, with the extra parameters marking the maximum number of vertices to count of:"
	@echo "  pruned:     the number of forms with full pruning"
	@echo "  semipruned: the number of unpruned forms with more vertices than the maximum pruned forms"
	@echo "  (to simulate no pruning, set pruned to 1)"
	@echo
	@echo "\"make clean\" to remove all generated binary files"

run_1: bin/siab1_$(size)
	./bin/siab1_$(size)

run_2: bin/siab2_$(size)
	./bin/siab2_$(size)

run_3: bin/siab3_$(size)
	./bin/siab3_$(size)

run_4: bin/siab4_$(size)
	./bin/siab4_$(size) $(guess)

count: bin/count_$(size)
	./bin/count_$(size) $(pruned) $(semipruned)

bin/siab1_$(size): obj/siab1_$(size).o
bin/siab2_$(size): obj/siab2_$(size).o
bin/siab3_$(size): obj/siab3_$(size).o obj/equivRelation.o
bin/siab4_$(size): obj/siab4_$(size).o
bin/count_$(size): obj/count_$(size).o

bin/%:
	$(LINK) $^ -o $@

obj/siab1_$(size).o: src/siab1.cpp $(HCUBE_FILES)
obj/siab2_$(size).o: src/siab2.cpp $(HCUBE_FILES)
obj/siab3_$(size).o: src/siab3.cpp $(PERMU_FILES) src/equivRelation.hpp
obj/siab4_$(size).o: src/siab4.cpp $(HCUBE_FILES) $(PERMU_FILES)
obj/count_$(size).o: src/count-forms.cpp $(HCUBE_FILES) $(PERMU_FILES)

obj/%_$(size).o:
	$(CC) $(CFLAGS) $< -o $@ -c -D MAX_DIM=$(size)

obj/%.o: src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) $< -o $@ -c

clean: 
	rm -f bin/* obj/*
