reset

set terminal png enhanced

set title "Twitter (TWTR) (Daily) \n 03-Sep-2021      Close: $64.66"

set key top left

set grid

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b\n%y"

# nomirror means do not put tics on the opposite side of the plot.
set xtics nomirror
set ytics nomirror

# Name our output file.
set output 'img/twtr_volume.png'

set autoscale y
set format y "%1.0f"
set ytics 500

plot 'data/twtr_ema.dat' using 1:($7/10000) notitle with boxes lc rgbcolor "blue"
