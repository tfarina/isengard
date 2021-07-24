reset

set title "Twitter (TWTR) (Daily) - 2021-07-23 - Close: $71.69"

set key top

set grid

set yrange [35:78]
set ytics 5 # smaller ytics
set mytics 1

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b %y"

set xtics rotate # rotate labels on the x axis

set terminal png enhanced
set output 'img/twtr_sma.png'

plot 'data/twtr_sma.dat' using 1:5 notitle with lines lc rgbcolor "black", \
     'data/twtr_sma.dat' using 1:8 title '5-day Simple Moving Average' with lines lc rgbcolor "green", \
     'data/twtr_sma.dat' using 1:9 title '20-day Simple Moving Average' with lines lc rgbcolor "red"
