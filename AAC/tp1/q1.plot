set term png;
set xrange [0:500000];
set yrange[0:200000000];
set output 'q1.png';
plot 'q1.txt' using 1:2 title 'aleaT max' with lines, \
'q1.txt' using 1:3 title 'aleaL max' with lines , \
'q1.txt' using 1:4 title 'aleaT min' with lines , \
'q1.txt' using 1:5 title 'aleaL min' with lines ;
