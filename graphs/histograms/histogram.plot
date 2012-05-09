# This should set up an environment to produce a histogram
# Looping in gnuplot seems to be hard or non existant in 
# version 4.4, so do the plotting manually with 
#
# output = "1000_%d_histogram.ps"
# plot "1000_%d_histogram.plt" using (bin((,binwidth)):2 smooth freq with boxes lc rgb "#FF3030" title "Probability %d";

reset;
set xrange [0:];
set ylabel "Frequency";
set xlabel "Degree of node";
binwidth=1;
bin(x,width)=width*floor(x/width);
set style fill solid border -1;
set term postscript color;

