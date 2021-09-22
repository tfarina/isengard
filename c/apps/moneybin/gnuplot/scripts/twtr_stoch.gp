reset

set terminal png enhanced

set title "Twitter (TWTR) (Daily) \n 15-Sep-2021      Close: $61.40"

set key top left

set grid

set yrange [0:100]
set ytics 10,20
set ylabel "Stochastic Oscillator"

# nomirror means do not put tics on the opposite side of the plot.
set xtics nomirror
set ytics nomirror

set arrow from graph 0, first 70 to graph 1, first 70 nohead
set arrow from graph 0, first 30 to graph 1, first 30 nohead

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b\n%y"

set output 'img/twtr_stoch.png'

plot 'data/twtr_stoch.dat' using 1:8 title 'STOCH(14) 21.04' with lines lc rgbcolor "blue"
