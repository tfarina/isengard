reset
set terminal pngcairo size 800,600
set output 'petr4_movavgs.png'
set title "PETR4 Stock Chart"
set grid
set yrange [5:40]
set ytics 2 # smaller ytics
set mytics 5
plot 'petr4.dat' using 0:5 notitle with lines lc rgbcolor "green", \
     'petr4.dat' using 0:8 notitle with lines lc rgbcolor "blue", \
     'petr4.dat' using 0:9 notitle with lines lc rgbcolor "red"
