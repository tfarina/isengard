reset

set title "BPAN4 (Daily)"

set key top

set grid

set yrange [4:24]
set ytics 2 # smaller ytics
set mytics 2

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b %y"

set xtics rotate # rotate labels on the x axis

set style fill solid noborder

set linetype 1 lc rgb 'red'
set linetype 2 lc rgb 'green'

set terminal png enhanced
set output 'img/bpan4.png'

plot 'data/bpan4.dat' using 1:2:4:3:5:($5 < $2 ? 1 : 2) linecolor variable notitle with candlesticks, \
     'data/bpan4.dat' using 1:8 title '5-day Moving Average' with lines lc rgbcolor "red", \
     'data/bpan4.dat' using 1:9 title '20-day Moving Average' with lines lc rgbcolor "blue"
