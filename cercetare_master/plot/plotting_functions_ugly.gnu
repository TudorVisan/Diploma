#!/usr/bin/gnuplot
#
# Plotting the data of file plotting_data1.dat
#
# AUTHOR: Hagen Wierstorf

reset

set terminal svg size 800,600 enhanced font 'Verdana,10'
set output 'same_level'
set border linewidth 1.5

set key right

set xrange [0:41]
set yrange [0.5:2]

set ylabel "Acceleration (G)"
set xlabel "Time (s)"

plot 'plotting_data2.dat' i 0 with lines lw 2 lt 1 lc 3 t 'Middle-Top', \
     'plotting_data2.dat' i 1 with lines lw 2 lt 3 lc 1 t 'Bottom', \
     'plotting_data2.dat' i 2 with lines lw 2 lt 5 lc 4 t 'Middle-Bottom', \
     'plotting_data2.dat' i 3 with lines lw 2 lt 7 lc 2 t 'Top'
