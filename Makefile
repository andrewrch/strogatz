all: strogatz watts

strogatz: src/strogatz.c src/graph.c src/graph.h src/histogram.c src/histogram.h Makefile
	gcc -std=gnu99 -Wall -Werror -lm -g -O2 -o $@ $(filter %.c,$^)

watts: src/watts.c src/graph.c src/graph.h src/histogram.c src/histogram.h Makefile
	gcc -std=gnu99 -Wall -lm -g -O2 -o $@ $(filter %.c,$^)
