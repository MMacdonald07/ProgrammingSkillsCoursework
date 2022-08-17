# Programming Skills Coursework

## Question 1: diffusion in one dimension (20 marks)

A random walk model in one dimension is defined as follows. A particle may exist at any
of L sites. The particle moves by ‘hopping’ either left or right: it chooses its direction at
random. For example, if the particle starts on site i then it can hop to either site i + 1
or site i − 1. We use periodic boundaries, so if a particle hops to the right from site L, it
arrives back at site 1; similarly, hopping to the left from site 1 leads up back to site L.

Write a program in which there are L = 10 sites: start the particle at site 1 and let it make
M = 20 hops. Print the position after each hop. (For example, a possible set of positions
that you might get (after 10 hops) is: 1 2 3 2 3 2 1 10 9 10 1.) Verify (i) that you
can get different outputs by varying the “random seed” (see appendix) and (ii) that by
recording the random seed, you are always able to reproduce any of your previous results.

In some physical problems involving diffusion, it’s important to know how long the particle
takes to explore all of its environment. Add an array to your program that keeps track of
whether the particle has visited each site. Use a loop to count how many hops your particle
makes before it has visited every site. Make 100 measurements of this number of hops and
calculate the average.

Vary the number of sites, L, and make a graph of the average number of hops required to
visit every site. Why does the graph have this shape?

## Question 2: diffusion in three dimensions (40 marks)

Repeat the analysis for a particle hopping in a three-dimensional array of sites, each of size
L = 10 (i.e. put the sites in a simple cubic grid of size L × L × L.) At each time step,
the particle can hop to any of its six nearest neighbours, chosen at random. Use periodic
boundaries in all three directions.

Repeat the analysis of question 1: calculate the average number of hops to visit all the sites
on a three-dimensional grid of size L. Make a graph of the average number of hops against
the total number of sites N = L for varying size L. Why does the graph have this shape?

## Question 3: diffusion on a network (40 marks)

Instead of simple lattices, it can be useful to consider diffusion on “networks” of sites.

The network contains A = 5 clusters each containing B = 4 sites. Sites within the same
cluster are all connected to each other, but the clusters are arranged in a ring, with only
one connection between each cluster.

Write a program that describes a random walk on this kind of network. At each step, the
particle should choose a possible path at random, out of those that are available. There are
several ways to write the program. A simple option is that for each site (i), you store a list
of possible other sites (j) to which a hop is possible. Another choice is to use a matrix to
keep track of which sites are connected to each other. Note that not all sites have the same
number of possible hops.

Check that your program works by running it for a few different values of A and B. Explore
how the number of steps required to visit all sites depends on A and B. Discuss your results
in your report.

## Report

In order to view report, will need to download the Report.pdf file in repository.
