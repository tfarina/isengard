set terminal png size 900, 300

set output "img/gold_71.png"

set title "Historical Gold Prices - 1971 to 2020"
set title font "Helvetica,14"

unset key

set timefmt "%Y"  # tells gnuplot the time format of the date in the dat file
set xdata time    # tells gnuplot the x axis is time data
set xrange ["1971":"2020"]
set xtics format "%Y"

set yrange [10:1800]
set ytics 50, 250, 1800
set ylabel "U.S. dollars (US$) per troy ounce"

plot 'data/gold.dat' using 1:2 with lines lc rgbcolor "blue"
