reset
set terminal pngcairo size 800,600
set output 'petr4_movavgs.png'
set title "PETR4 Stock Chart"
set grid
set yrange [5:40]
set ytics 2 # smaller ytics
set mytics 5
set xdata time # tells gnuplot the x axis is time data
set timefmt "%Y-%m-%d" # specify our time string format
set format x "%b %y"
set xtics rotate # rotate labels on the x axis
plot 'petr4.dat' using 1:5 notitle with lines lc rgbcolor "green", \
     'petr4.dat' using 1:8 notitle with lines lc rgbcolor "blue", \
     'petr4.dat' using 1:9 notitle with lines lc rgbcolor "red"
