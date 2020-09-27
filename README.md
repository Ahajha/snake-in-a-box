# snake-in-a-box

Repository for algorithm experimentation for the snake-in-a-box problem.  
Problem overview can be found here: https://en.wikipedia.org/wiki/Snake-in-the-box  
This repository contains several different algorithms, described below.  

## Algorithms 1 & 2

Algorithm 1 is a naive algorithm, which starts in one corner of the hypercube and exhaustively tries every way to extend the snake.  
Algorithm 2 improves on this slightly, by only allowing the snake to extend in a given dimension after all previous dimensions have been explored.

Neither of these are my own design, but are used as a comparison benchmark. Algorithm 2 can finish dimension 6 almost instantly, have not seen how long 7 would take.
## Algorithm 3

Algorithm 3 is the first algorithm of my own design. It is based on the principle of taking all possible halves of a hypercube and merging them together
(with validation to ensure they can be put together in the first place). There also needs to be a loosening of the requirements to ensure all snakes
are enumerated, and that comes in the form of enumerating "trail in a box"es, where the requirement of connectedness is dropped. To solve the SIAB problem, we
just have to check for the largest trail-in-a-box with 1 component.
There is also a greedy property to this algorithm, since it only generates the subcubes needed to create those of a specific number of vertices. For example:  

To solve N=3, we need to first solve N=2 recursively, which comes up with 3 vertices max (note these are generally trails, not snakes).
Then it tries to take 2 3s and put them together, but fails to make a match, so it tries to go down to 5, but that requires also generating
2 vertices for the N=2, which is done recursively (note that N=2 is taking care of all the recursive calls to N=1, and N=0 is the base case).
It ends up finding the solution for N=3 at 5 vertices, and doesn't need to fully enumerate dimension 2.  

This has some strengths, but quite a few weakenesses. The main strength is the idea of not having to solve the same problem twice, hence merging halves together.
However, the main weaknesses come in the form of having to generate and store tons of configurations.  

Before the greedy approach was implemented N=5 would take about 25 seconds to run (on an R7 3700X, single threaded), and consumes about 6-8 GB of RAM. After,
it only takes about half a second, but still takes too long for N=6 (I haven't waited for it to finish, takes at least a few minutes with signs of it taking a lot longer).

## Algorithm 4

Algorithm 4 is a (not yet implemented) algorithm that takes a few good ideas from algorithms 2 and 3. It is based on splitting the snakes themselves in half,
rather than their containers. This approach has a few advantages.  

First, we only need to use a single size of container (N). In the 'sub-snakes', we need to keep track of invalid vertices (those that
would make the degree of a vertex more than 2) and endpoints (we join snakes at their endpoints).  

Second, we can skip over a lot of sizes by building up the snake logarithmically. For example, to find snakes of size 13, we can merge snakes of size 6 and 7. 6 needs 2 3s,
7 needs a 3 and a 4, 3 needs 2 and 1, 4 needs 2, 2 needs 1. (1 is the base). This is prime territory for a dynamic programming algorithm.

The amount of configurations generated is a subset of those in algorithm 3, for two reasons. One, because this only considers trails with 1 component, and second, because
we only need to look at one dimension.

# Running the code

This project uses a makefile. Run ```make``` to get a basic help prompt. The general form of the input is ```make run_X size=N```,
where X is the algorithm version and N is the number of dimensions.
