reset

set terminal png enhanced

set title "Twitter (TWTR) (Daily) \n 03-Sep-2021      Close: $64.66"

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

set output 'img/twtr_sma.png'

plot 'data/twtr_sma.dat' using 1:5 notitle with lines lc rgbcolor "black", \
     'data/twtr_sma.dat' using 1:8 title 'SMA(5)' with lines lc rgbcolor "green", \
     'data/twtr_sma.dat' using 1:9 title 'SMA(20)' with lines lc rgbcolor "red"
