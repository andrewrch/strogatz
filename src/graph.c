#include "graph.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

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
  g->mat = (bool*) calloc(num_vertices * num_vertices, sizeof(bool));
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
    for (int j = 1; j <= (edges_per_vertex >> 1); j++) {
      set_edge(g, i, (i + j) % g->size, true);
      set_edge(g, i, (g->size + i - j) % g->size, true);
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
unsigned int get_degree(struct graph_t *g, int node)
{
  unsigned int d = 0;
  for (int i = 0; i < g->size; i++)
    d += !!get_edge(g, i, node);
  return d;
}

/**
 * Recursive helper method to find min distance from a to b
 */
static unsigned int get_distance_impl(struct graph_t *g, int a, int b, bool* visited)
{
  visited[a] = true;
  if (get_edge(g, a, b))
    return 0;
  else
  {
    unsigned int min = UINT_MAX;
    for (int i = 0; i < g->size; i++)
      if (get_edge(g, a, i) && !visited[i])
        min = min(min, get_distance_impl(g, i, b, visited) + 1);
    return min;
  }
}

/**
 * Finds distance from node a to node b
 *
 * Each arc is of distance 1, so really this a measure
 * of how many steps it takes to get from a to b
 */
int get_distance(struct graph_t *g, int a, int b)
{
  // Keeps track of visited nodes to stop cyclic infinite recursion
  bool visited[g->size];
  for (int i = 0; i < g->size; i++) visited[i] = false;
  return get_distance_impl(g, a, b, visited);
}

/**
 * Function to calculate average path length in graph according
 * to equations given in lecture slides.
 */
double get_average_path_length(struct graph_t *g)
{
  // Calculate total path length
  unsigned int sum = 0;
  
  for (int i = 0; i < g->size; i++)
    for (int j = i; j < g->size; j++) 
      sum += get_distance(g, i, j);

  // Now make that an average
  return (double) sum / (0.5 * (double) g->size * (double) (g->size - 1));
}

static int get_links_between_neighbours(struct graph_t *g, int node)
{
  int sum = 0;
  for (int i = 0; i < g->size; i++)
    // If i is a neighbour of node
    if (get_edge(g, node, i))
      // Check all possible links
      for (int j = 0; j < g->size; j++)
        // If edge is a neighbour of i, and also a neighbour of node
        if (get_edge(g, i, j) && get_edge(g, node, j))
          sum++;
  return sum;
}

double get_global_clustering_coefficient(struct graph_t *g)
{
  double sum = 0;
  // Average C over all nodes
  for (int i = 0; i < g->size; i++)
    sum += get_local_clustering_coefficient(g, i);
  return sum / g->size;
}

double get_local_clustering_coefficient(struct graph_t *g, int node)
{
  // Named according to formula in lecture
  int e = get_links_between_neighbours(g, node);
  unsigned int k = get_degree(g, node);
  return k > 1 ? 2 * (double) e / (k * (k-1)) : 0;
}

/**
 * Produces the degree distribution for the network.
 * Returns a malloced int pointer so dont forget to free...
 */
struct histogram_t* get_degree_distribution(struct graph_t *g)
{
  // Find max degree in network and set size
  unsigned int max_degree = 0;
  for (int i = 0; i < g->size; i++)
    max_degree = max(max_degree, get_degree(g, i));

  struct histogram_t *hist = create_empty_histogram(max_degree + 1);
  // And now build the histogram
  for (unsigned int i = 0; i < g->size; i++)
    increment_bin(hist, get_degree(g, i));
  return hist;
}

/**
 * Converts a number into an ASCII label
 * Characters represent the index in reverse, but it doesn't really matter
 */
char* get_graph_label(int n, char *str)
{
  int pos = 0;
  do {
    str[pos++] = 'A' + (n % 26);
    n /= 26;
  } while (n);
  str[pos] = 0;
  return str;
}

/**
 * Prints a graph in dot format.
 */
void print_graph(struct graph_t* g)
{
  // Print the graph header
  printf("graph the_graph {\n"
      "ordering=out\n"
      "layout=\"neato\"\n");

  // Circo draws graphs in a circle sometimes but fails other times so 
  // we'll calculate node positions manually.
  double x, y, angle, radius = g->size/M_PI;
  // Print all nodes
  for (int i = 0; i < g->size; i++)
  {
    angle = ((M_PI*2)/(double)g->size)*(double)i;
    x = cos(angle)*radius;
    y = sin(angle)*radius;
    char label[50];
    get_graph_label(i, label);
    // Pink nodes for dan
    printf("%s [label=\"%d\","
        "pos=\"%f,%f!\","
        "shape=circle,"
        "width=1.,"
        "fixedsize=true," 
        "style=filled,"
        "fillcolor=\"#FF1CAE\"\n]",
        label, i, x, y);
  }
  // Print all arcs
  for (int i = 0; i < g->size; i++)
    for (int j = i; j < g->size; j++)
      if (get_edge(g, i, j))
      {
        // Wont have a label longer than this...
        char i_lab[50], j_lab[50];
        get_graph_label(i, i_lab);
        get_graph_label(j, j_lab);
        printf("%s -- %s\n", i_lab, j_lab);
      }

  // Finish that mother off
  printf("}\n");
}

void delete_graph(struct graph_t* g)
{
  free(g->mat);
  free(g);
}
