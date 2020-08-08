reset

set title "REPSOL Dividend Distribution"

set term png font "Times, 18"
set output "img/repsol_div.png"

set ylabel "Euros / Share"
set yrange [0.7:1]

#set xlabel "Year"    #set x and y label
set xrange [2014:2020]    #set x and y range
#set xtics 0,1,4    #set xtics

set style fill solid
set boxwidth 0.5

unset key

# plot bar chart and the value labels on the bars
plot "data/repsol_div.dat" using 1:2 with boxes lc rgbcolor "#FFA500",\
     "data/repsol_div.dat" using 1:($2+0.01):2 with labels
#     "data/repsol_div.dat" using 1:2:2 with labels
