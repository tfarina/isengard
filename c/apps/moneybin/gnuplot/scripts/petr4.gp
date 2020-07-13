reset

set title "PETR4 Stock Chart"

set key bottom left

set grid

set yrange [10:32]
set ytics 2             # smaller ytics

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b %y"

set xtics rotate        # rotate labels on the x axis

set terminal png enhanced
set output 'img/petr4.png'

plot 'data/petr4.dat' using 1:5 notitle with lines lc rgbcolor "black", \
     'data/petr4.dat' using 1:8 title '5-day Moving Average' with lines lc rgbcolor "green", \
     'data/petr4.dat' using 1:9 title '20-day Moving Average' with lines lc rgbcolor "red"
