#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  int num_vertices, edges_per_vertex;

  if (sscanf(argv[1], "%i", &num_vertices) != 1)
    return EXIT_FAILURE;
  if (sscanf(argv[2], "%i", &edges_per_vertex) != 1)
    return EXIT_FAILURE;

  struct graph_t *g;
  const int NUMBER_LOOPS = 1;
  for (double p = 1.0; p > 0.001; p/=2)
  {
    g = build_regular_graph(num_vertices, edges_per_vertex);
    double l = 0, l0, c = 0, c0;
    c0 = get_global_clustering_coefficient(g);
    l0 = get_average_path_length(g);
    //printf("%f, %f\n", l0, c0);
    for (int i = 0; i < NUMBER_LOOPS; i++)
    {
      randomise_graph(g, p);

//      double l2 = get_average_path_length(g); 
//      if (l2 > l0)
//      {
//        //printf("%f %f\n", l2, l0);
//        return 0;
//      }
//      else
//        l+= l2;
//      print_graph(g);
      l += get_average_path_length(g);
      c += get_global_clustering_coefficient(g);

      //struct histogram_t *h = get_degree_distribution(g);
      //print_histogram(stderr, h);
      //print_graph(g);

      delete_graph(g);
      g = build_regular_graph(num_vertices, edges_per_vertex);
      //delete_histogram(h);
    }
    fprintf(stderr, "%f %f %f\n", p, (l / NUMBER_LOOPS) / l0, (c / NUMBER_LOOPS) / c0);
    delete_graph(g);
  }
  return 0;


}
