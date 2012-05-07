#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdbool.h>

#define min(x,y) ({ \
  typeof (x) __x = (x); \
  typeof (y) __y = (y); \
  (__x < __y) ? __x : __y; \
})
#define max(x,y) ({ \
  typeof (x) __x = (x); \
  typeof (y) __y = (y); \
  (__x > __y) ? __x : __y; \
})

struct graph_t {
  bool *mat;
  int size;
};

struct graph_t* build_unconnected_graph(int num_vertices);
struct graph_t* build_regular_graph(int num_vertices, int edges_per_vertex);
double get_average_path_length(struct graph_t*);
unsigned int* get_degree_distribution(struct graph_t*, unsigned int*);
double get_local_clustering_coefficient(struct graph_t*, int);
double get_global_clustering_coefficient(struct graph_t*);
void randomise_graph(struct graph_t*, float);
int get_degree(struct graph_t*, int);
int get_distance(struct graph_t*, int, int);
void print_graph(struct graph_t*);
void delete_graph(struct graph_t*);

#endif

