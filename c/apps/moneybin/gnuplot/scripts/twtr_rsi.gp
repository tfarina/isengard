reset

set title "Twitter (TWTR) (Daily) - 2021-07-23 - Close: $71.69"

set key top

set grid

set yrange [0:100]
set ytics 10 # smaller ytics
set mytics 1
set ylabel "Relative Strength Index"

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b %y"

set xtics rotate # rotate labels on the x axis

set terminal png enhanced
set output 'img/twtr_rsi.png'

plot 'data/twtr_rsi.dat' using 1:8 notitle with lines lc rgbcolor "blue" 
