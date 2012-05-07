#include "graph.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define M_PI 3.1415926535

/**
 * Initialises an empty graph with no connections
 */
struct graph_t* build_unconnected_graph(int size)
{
  struct graph_t *g = (struct graph_t*) malloc(sizeof(struct graph_t));
  g->mat = (bool**) malloc(size * sizeof(bool*));
  for (int i = 0; i < size; i++)
  {
    g->mat[i] = (bool*) malloc(size * sizeof(bool));
    // Set all connections false
    for(int j = 0; j < i; j++)
    {
      g->mat[i][j] = false;
    }
  }
  g->size = size;
  return g;
}

/**
 * Builds a graph of any size with a ring topology
 */
struct graph_t* build_ring_graph(int size)
{
  struct graph_t* g = build_unconnected_graph(size);
  for (int i = 0; i < size; i++)
  {
    // Nodes which this one will connect to
    int a = (i+1)%size;
    int b = (i+2)%size;

    // Bit of fiddling to get connections all on one
    // side of the adjacency matrix
    if (a > i)
      g->mat[i][(i+1)%size] = true;
    else
      g->mat[(i+1)%size][i] = true;
    if (b > i)
      g->mat[i][(i+2)%size] = true;
    else
      g->mat[(i+2)%size][i] = true;
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
      if (g->mat[i][j] && (float)rand()/(float)RAND_MAX < p && get_degree(g, j) != 1)
      {
        // Disconnect nodes
        g->mat[i][j] = false;
        // Pick a new node
        int new;
        do {
          new = (float)rand()/(float)RAND_MAX * g->size;
        }
        // Want to make sure we create a new connection, and it isn't
        // a self loop
        while (new == j || new == i);
        if (new > i)
          g->mat[i][new] = true;
        else
          g->mat[new][i] = true;

      }
    }
}

// Calculates the degree of a node
int get_degree(struct graph_t *g, int node)
{
  int d = 0;
  for (int i = node; i < g->size; i++)
  {
    if (g->mat[i][node])
      d++;
    if (g->mat[node][i])
      d++;
  }
  return d;
}

/**
 * Converts a number into an ASCII label
 */
char* get_graph_label(int n)
{
  // Wont have a label longer than this...
  char* str = (char*) malloc(10* sizeof(char));
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
    char* label = get_graph_label(i);
    // Dont know why I chose this colour, looks wicked though
    printf("%s [label=\"\",pos=\"%f,%f!\",shape=circle,width=.3,style=filled,fillcolor=\"#FF1CAE\",fixedsize=true]\n", label, x, y);
    free(label);
  }
  for (int i = 0; i < g->size; i++)
    for (int j = i; j < g->size; j++)
      if (g->mat[i][j])
      {
        char* i_lab = get_graph_label(i);
        char* j_lab = get_graph_label(j);
        printf("%s -- %s\n", i_lab, j_lab);
        free(i_lab); free(j_lab);
      }

  // Finish that mother off
  printf("}\n");
}

void print_matrix(struct graph_t* g)
{
  for (int i =0; i < g->size; i++)
  {
    for (int j = 0; j < g->size; j++)
      printf("%c ", g->mat[i][j] ? 't' : 'f');
    printf("\n");
  }
}

void delete_graph(struct graph_t* g)
{
  for (int i = 0; i < g->size; i++)
    free(g->mat[i]);
  free(g->mat);
}
