#include "graph.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

/**
 * Sets whether an edge is present between two vertices i & j
 * Assumes the graph is undirected
 */
static void set_edge(struct graph_t *g, int i, int j, bool connected)
{
  g->mat[(min(i,j)*g->size)+max(i,j)] = connected;
}

/**
 * Returns true if an edge is present between two vertices i & j, else false
 * Assumes the graph is undirected
 */
static bool get_edge(struct graph_t *g, int i, int j)
{
  return g->mat[(min(i,j)*g->size)+max(i,j)];
}

/**
 * Initialises an empty graph with no connections
 */
struct graph_t* build_unconnected_graph(int num_vertices)
{
  struct graph_t *g = (struct graph_t*) malloc(sizeof(struct graph_t));
  g->mat = (bool*) calloc(num_vertices*((num_vertices>>1)+(num_vertices%2)+1), sizeof(bool));
  g->size = num_vertices;
  return g;
}

/**
 * Builds a graph of any size with a ring topology
 */
struct graph_t* build_regular_graph(int num_vertices, int edges_per_vertex)
{
  struct graph_t* g = build_unconnected_graph(num_vertices);
  for (int i = 0; i < num_vertices; i++) {
    for (int j = i+1; j <= (i + edges_per_vertex); j++) {
      set_edge(g, i, j % g->size, true);
    }
  }
  return g;
}

/**
 * This does the Strogatz whatsit.
 * Takes a graph g, iterates over all arcs, and rewires connections
 * to random nodes with probability p
 */
void randomise_graph(struct graph_t *g, float p)
{
  // SEEEEEEEEEEDD
  srand((unsigned)time(0));

  // Iterate over each node
  for (int i = 0; i < g->size; i++)
    // Now test all connections
    for (int j = i; j < g->size; j++)
    {
      // If random number < probability we swap nodes about, 
      // Also, dont disconnect a node from the graph.
      if (get_edge(g, i, j) && (float)rand()/(float)RAND_MAX < p && get_degree(g, j) != 1)
      {
        // Disconnect nodes
        set_edge(g, i, j, false);
        // Pick a new node
        int new;
        do {
          new = (float)rand()/(float)RAND_MAX * g->size;
        }
        // Want to make sure we create a new connection, and it isn't
        // a self loop
        while (new == j || new == i);
        set_edge(g, i, new, true);
      }
    }
}

// Calculates the degree of a node
int get_degree(struct graph_t *g, int node)
{
  int d = 0;
  for (int i = 0; i < g->size; i++)
    d += !!get_edge(g, i, node);
  return d;
}

/**
 * Converts a number into an ASCII label
 */
char* get_graph_label(int n, char *str)
{
  int pos = 0;
  while (n >= 26)
  {
    int c = (n-26)/26;
    str[pos] = c + 65;
    n%=26;
    pos++;
  }
  str[pos] = n+65;
  str[pos+1] = '\0';
  return str;
}

/*
 * Prints a graph in dot format.
 * Please use `circo` to draw the graph in a nice big circle to 
 * make it look pretty.
 */
void print_graph(struct graph_t* g)
{
  // Print the graph header
  printf("graph the_graph {\n");
  printf("ordering=out\n");
  printf("layout=\"neato\"\n");

  // Circo draws graphs in a circle sometimes but fails other times so 
  // we'll calculate node positions manually.
  double x = 0, y = 0;
  double radius = g->size/M_PI;
  // Print all nodes and their connections
  for (int i = 0; i < g->size; i++)
  {
    x = cos(((M_PI*2)/(double)g->size)*(double)i)*radius;
    y = sin(((M_PI*2)/(double)g->size)*(double)i)*radius;
    char label[10];
    get_graph_label(i, label);
    // Dont know why I chose this colour, looks wicked though
    printf("%s [label=\"\",pos=\"%f,%f!\",shape=circle,width=.3,style=filled,fillcolor=\"#FF1CAE\",fixedsize=true]\n", label, x, y);
  }
  for (int i = 0; i < g->size; i++)
    for (int j = i; j < g->size; j++)
      if (get_edge(g, i, j))
      {
        // Wont have a label longer than this...
        char i_lab[10], j_lab[10];
        get_graph_label(i, i_lab);
        get_graph_label(j, j_lab);
        printf("%s -- %s\n", i_lab, j_lab);
      }

  // Finish that mother off
  printf("}\n");
}

void print_matrix(struct graph_t* g)
{
  for (int i =0; i < g->size; i++)
  {
    for (int j = 0; j < g->size; j++)
      printf("%c ", get_edge(g, i, j) ? 't' : 'f');
    printf("\n");
  }
}

void delete_graph(struct graph_t* g)
{
  free(g->mat);
  free(g);
}
