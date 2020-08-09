reset

set title "GALP Stock Chart"

set key bottom left

set grid

set yrange [7:16]
set ytics 2             # smaller ytics

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b %y"

set xtics nomirror

set terminal png enhanced
set output 'img/galp.png'

plot 'data/galp.dat' using 1:5 notitle with lines lc rgbcolor "black", \
     'data/galp.dat' using 1:8 title '5-day Moving Average' with lines lc rgbcolor "green", \
     'data/galp.dat' using 1:9 title '20-day Moving Average' with lines lc rgbcolor "red"
