reset

set title "BPAN4 (Daily)"

set key top

# nomirror means do not put tics on the opposite side of the plot
set xtics nomirror
set ytics nomirror

set yrange [15:24]

set timefmt "%Y-%m-%d"  # specify our time string format
set xdata time          # tells gnuplot the x axis is time data
set format x "%b %d"

set linetype 1 lc rgb 'red'
set linetype 2 lc rgb 'green'

set terminal png enhanced
set output 'img/bpan4_month05.png'

plot 'data/bpan4_month05.dat' using 1:2:4:3:5:($5 < $2 ? 1 : 2) linecolor variable notitle with candlesticks
