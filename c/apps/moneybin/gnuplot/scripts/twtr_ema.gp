reset

set terminal png enhanced

set title "Twitter (TWTR) (Daily) \n 19-Aug-2021      Close: $62.05"

set key top left

set grid

set yrange [35:78]
set ytics 5 # smaller ytics
set mytics 1

# nomirror means do not put tics on the opposite side of the plot.
set xtics nomirror
set ytics nomirror

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b\n%y"

# Name our output file.
set output 'img/twtr_ema.png'

plot 'data/twtr_ema.dat' using 1:5 notitle with lines lc rgbcolor "black", \
     'data/twtr_ema.dat' using 1:8 title 'EMA(12)' with lines lc rgbcolor "green", \
     'data/twtr_ema.dat' using 1:9 title 'EMA(26)' with lines lc rgbcolor "red"
