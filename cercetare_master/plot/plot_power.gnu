#!/usr/bin/gnuplot
#
# Plotting the data of file plotting_data1.dat
#
# AUTHOR: Hagen Wierstorf

reset

set terminal svg size 800,600 enhanced font 'Verdana,10'
set output 'power'
set border linewidth 1.5

set key right


set xrange [0:1101]
set yrange [0:30000]

set ylabel "Current (uA)"
set xlabel "Time (ms)"

plot 'power.dat' i 0 with lines lw 2 lt 1 lc 3 t 'Power Consumption'
