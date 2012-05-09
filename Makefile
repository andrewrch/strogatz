CFLAGS := -Wall -Werror -std=gnu99 -fopenmp -ftree-parallelize-loops=4 -g -O0 -lm

all: strogatz watts print_histogram

strogatz: src/strogatz.c src/graph.c src/graph.h src/histogram.c src/histogram.h Makefile
	gcc $(CFLAGS) -o $@ $(filter %.c,$^)

watts: src/watts.c src/graph.c src/graph.h src/histogram.c src/histogram.h Makefile
	gcc $(CFLAGS) -o $@ $(filter %.c,$^)

print_histogram: src/print_histogram.c src/graph.c src/graph.h src/histogram.c src/histogram.h Makefile
	gcc $(CFLAGS) -o $@ $(filter %.c,$^)
