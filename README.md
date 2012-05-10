strogatz
========

Implementation of Watts and Strogatz&#39;s algorithm for network rewiring

Three binaries are built by typing "make".

strogatz:
=========

strogatz takes three arguments, the graph size (N), the number of edges per node in the original graph (K), and the probability of swapping (p).

The binary then prints out the graph in dot format to stderr, so running

./strogatz 100 4 0.1 2> graph.dot

Will produce a graph with 100 nodes, and 4 connections for each node in the initial graph and then rewire them with probability.

A small bash one-liner is put inside create\_svg, so running

./create\_svg 100 4 0.1 graph.svg 

will create a similar graph, and put it in the vector file graph.svg.

The strogatz binary also prints a few statistics about the graph and the shape of the degree distribution histogram to stdout.

print\_histogram
================

Takes 3 arguments, N, K, and p, and prints graph statistics to stderr, and values to build a degree histogram to stdout.

A gnuplot script to build a histogram is provided in the graph directory:

graphs/histograms/histogram.plot

print\_L\_C\_curve
==================

Takes 4 arguments, N, K, iterations (I), and decrease factor (F).

This creates graphs of size N, with K starting nodes.  For all probabilities from 1.0 to 0, it creates I graphs, calculates the normalised clustering coefficient (C) and normalised average path length (L), averages the result (By dividing by I), and prints L, C, and p to stdout.  Each iteration, the probability is divided by F.

This can then be plotted on a logscale using the gnuplot script in:

graphs/path\_against\_clustering/path.plot
