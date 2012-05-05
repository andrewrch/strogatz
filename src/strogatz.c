#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    printf("Usage: ./strogatz size_of_graph prob_of_swap\n\n");
    printf("size_of_graph - an int\n");
    printf("prob_of_swap - a float between 0 and 1\n");
    return 0;
  }

  struct graph_t *g = build_ring_graph(atoi(argv[1]));
  randomise_graph(g, atof(argv[2]));
  print_graph(g);
//  print_matrix(g);
  delete_graph(g);
  return 0;
}
