#!/bin/bash

gnuplot << EOF
set term postscript
set output "main_plot.eps"
set contour base
set view map
#set view 0,0,1
unset surface
set title ""
set cntrparam levels 200
splot "pot.dat" using 1:2:3 with image notitle #, \
     # "mat_test.dat" using 1:2:3 with lines notitle
EOF

