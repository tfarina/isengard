reset

set terminal png enhanced

set title "Twitter (TWTR) (Daily) \n 09-Sep-2021      Close: $64.66"

set key top left

set grid

set yrange [0:100]
set ytics 10,20
set ylabel "Relative Strength Index"

# nomirror means do not put tics on the opposite side of the plot.
set xtics nomirror
set ytics nomirror

set arrow from graph 0, first 70 to graph 1, first 70 nohead
set arrow from graph 0, first 30 to graph 1, first 30 nohead

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b\n%y"

set output 'img/twtr_rsi.png'

plot 'data/twtr_rsi.dat' using 1:8 title 'RSI(14) 48.16' with lines lc rgbcolor "blue"
