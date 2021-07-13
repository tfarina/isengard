set terminal png size 900, 300

set output "img/gold_71.png"

set title "Historical Gold Prices - 1971 to 2020"
set title font "Helvetica,14"

unset key

# Tells gnuplot the input format in which the date is at the data file.
set timefmt "%Y"

# Horizontal (time) axis layout
set xdata time    # tells gnuplot the x axis is time data
set xrange ["1971":"2020"]
set format x "%Y"
set mxtics 5

set ylabel "U.S. dollars (US$) per troy ounce"
set yrange [0:2000]
set ytics 250
set mytics 5


plot 'data/gold.dat' using 1:2 with lines lc rgbcolor "blue"
