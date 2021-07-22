reset

set title "Hecla Mining (HL) (Daily)"

set key top

set grid

set yrange [4:10]
set ytics 1 # smaller ytics
set mytics 1

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b %y"

set xtics rotate # rotate labels on the x axis

set terminal png enhanced
set output 'img/hl.png'

plot 'data/hl.dat' using 1:5 notitle with lines lc rgbcolor "black", \
     'data/hl.dat' using 1:8 title '5-day Moving Average' with lines lc rgbcolor "green", \
     'data/hl.dat' using 1:9 title '20-day Moving Average' with lines lc rgbcolor "red"
