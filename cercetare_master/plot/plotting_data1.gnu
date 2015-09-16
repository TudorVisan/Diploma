#!/usr/bin/gnuplot
#
# Plotting the data of file plotting_data1.dat
#
# AUTHOR: Hagen Wierstorf

reset

set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'same_sensor.png'
set border linewidth 1.5

set key right


set xrange [0:50]
set yrange [7000:40000]

set ylabel "Acceleration (G)"
set xlabel "Time (s)"

plot 'plotting_data1.dat' i 0 with lines lw 2 lt 1 lc 3 t 'Still Table', \
     'plotting_data1.dat' i 1 with lines lw 2 lt 3 lc 1 t 'Small Vibration', \
     'plotting_data1.dat' i 2 with lines lw 2 lt 5 lc 4 t 'Medium Vibration', \
     'plotting_data1.dat' i 3 with lines lw 2 lt 7 lc 2 t 'High Vibration'
