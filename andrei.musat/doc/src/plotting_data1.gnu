#!/usr/bin/gnuplot
#
# Plotting the data of file plotting_data1.dat
#
# AUTHOR: Hagen Wierstorf

reset

# wxt
#set terminal wxt size 350,262 enhanced font 'Verdana,10' persist
# png
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'plotting_data1.png'
# svg
#set terminal svg size 350,262 fname 'Verdana, Helvetica, Arial, sans-serif' \
#fsize '10'
#set output 'plotting_data1.svg'

# color definitions
set border linewidth 1.5
#set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 13 ps 1.5 # --- blue
set style line 1 lc rgb '#8b1a0e' pt 6 ps 2 lt 1 lw 2 # --- red
set style line 2 lc rgb '#5e9c36' pt 8 ps 2 lt 1 lw 2 # --- green
set style line 3 lc rgb '#336699' pt 10 ps 2 lt 1 lw 2 # --- green
#set style line 4 lc rgb '#ff6600' pt 12 ps 1 lt 1 lw 2 # --- green

set key left

#set ytics 1
#set tics scale 0.75

#set xrange [0:1000000]
#set yrange [0:2000]
set logscale x
set logscale y

plot 'plotting_data2.dat' i 0 with linespoints ls 1 t 'Graphviz',\
     'plotting_data2.dat' i 1 with linespoints ls 2 t 'Eclipse GEF',\
     'plotting_data2.dat' i 2 with linespoints ls 3 t 'Proposed Algorithm'
