set term png;
set xrange [0:500000];
set yrange[0:999999999];
set output 'q13.png';
plot 'q13.txt' using 1:2 title 'minimumTriT' with lines, \
'q13.txt' using 1:3 title 'minimumSimpleT' with lines ;
