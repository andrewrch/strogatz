strogatz: src/strogatz.c src/graph.c src/graph.h src/histogram.c src/histogram.h
	gcc -std=gnu99 -lm -g -O2 -o strogatz src/strogatz.c src/graph.c src/histogram.c
