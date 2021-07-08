reset

set title "PRIO3 (Daily)"

# Where is the legend?
set key top

set grid

set yrange [4:25]
set ytics 1 # smaller ytics
set mytics 1

set timefmt "%Y-%m-%d"  # tells gnuplot the time format of the date in the dat file
set xdata time          # tells gnuplot the x axis is time data

# Set the format of the dates on the x axis.
set format x "%b %y"

set xtics rotate # rotate labels on the x axis

set style fill solid noborder

set linetype 1 lc rgb 'red'
set linetype 2 lc rgb 'green'

set terminal png enhanced
set output 'img/prio3.png'

plot 'data/prio3.dat' using 1:2:4:3:5:($5 < $2 ? 1 : 2) linecolor variable notitle with candlesticks, \
     'data/prio3.dat' using 1:8 title '5-day Moving Average' with lines lc rgbcolor "red", \
     'data/prio3.dat' using 1:9 title '20-day Moving Average' with lines lc rgbcolor "blue"
