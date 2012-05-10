#include "graph.h"
#include <stdio.h>
#include <stdlib.h>


static void print_usage(FILE *f)
{
  fprintf(f, "Usage: ./strogatz num_vertices edges_per_vertex prob_of_swap\n\n");
  fprintf(f, "  num_vertices - an int\n");
  fprintf(f, "  edges_per_vertex - an int\n");
  fprintf(f, "  iterations - how many iterations to average for each probability\n");
  fprintf(f, "  decrease_factor - how much to decrease p by each time\n");
}

int main(int argc, char** argv)
{
  int num_vertices, edges_per_vertex, iterations;
  double decrease_factor;
  if (argc < 5)
    goto err_usage;
  if (sscanf(argv[1], "%i", &num_vertices) != 1)
    goto err_usage;
  if (sscanf(argv[2], "%i", &edges_per_vertex) != 1)
    goto err_usage;
  if (sscanf(argv[3], "%i", &iterations) != 1)
    goto err_usage;
  if (sscanf(argv[4], "%lf", &decrease_factor) != 1)
    goto err_usage;

  if (edges_per_vertex % 2) {
    fprintf(stderr, "edges_per_vertex should be even\n");
    return EXIT_FAILURE;
  }

  if (decrease_factor < 1)
  {
    fprintf(stderr, "decrease_factor should be greater than 1");
    return EXIT_FAILURE;
  }

  struct graph_t *g;
  for (double p = 1.0; p > 0.001; p/=decrease_factor)
  {
    fprintf(stdout, "Using probability %f\n", p);
    g = build_regular_graph(num_vertices, edges_per_vertex);
    double l = 0, l0, c = 0, c0;
    c0 = get_global_clustering_coefficient(g);
    l0 = get_average_path_length(g);
    for (int i = 0; i < iterations; i++)
    {
      fprintf(stdout, "  Iteration %d\n", i+1);
      randomise_graph(g, p);
      l += get_average_path_length(g);
      c += get_global_clustering_coefficient(g);
      delete_graph(g);
      g = build_regular_graph(num_vertices, edges_per_vertex);
    }
    fprintf(stderr, "%f %f %f\n", p, (l / iterations) / l0, (c / iterations) / c0);
    delete_graph(g);
  }
  return 0;

err_usage:
  print_usage(stderr);
  return EXIT_FAILURE;
}
