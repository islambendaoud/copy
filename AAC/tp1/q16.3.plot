set term png;
set xrange [0:500000];
set yrange[0:7000000];
set output 'q163.png';
plot 'q16.txt' using 1:4 title 'nLogn' with lines;
