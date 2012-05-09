set term postscript color "Helvetica" 18 linewidth 2.0;
set output "postscript/path_against_clustering.ps"
plot "500_0.0001_graph.plt" using 1:2 with linespoints linewidth 3.0 pointsize 2.0 lc rgb "#FF3030" title "L(P) / L(0)", "" using 1:3 with linespoints linewidth 3.0 pointsize 2.0 lc rgb "#4E78A0" title "C(P) / C(0)"
