reset

set title "OIBR3 Stock Chart"

set key top

set grid

set yrange [0:12]
set ytics 1 # smaller ytics
set mytics 1

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b %y"

set xtics rotate # rotate labels on the x axis

set terminal png enhanced
set output 'img/oibr3.png'

plot 'data/oibr3.dat' using 1:5 notitle with lines lc rgbcolor "black", \
     'data/oibr3.dat' using 1:8 title '5-day Moving Average' with lines lc rgbcolor "green", \
     'data/oibr3.dat' using 1:9 title '20-day Moving Average' with lines lc rgbcolor "red"
