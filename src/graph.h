#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdbool.h>

struct graph_t {
  bool **mat;
  int size;
};

struct graph_t* build_unconnected_graph(int size);
struct graph_t* build_ring_graph(int size);
void randomise_graph(struct graph_t*, float);
int get_degree(struct graph_t*, int);
void print_graph(struct graph_t*);
void print_matrix(struct graph_t*);
void delete_graph(struct graph_t*);

#endif

