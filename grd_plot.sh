#!/bin/bash

# input argument from electric
filename=$1

echo "Plotting gradient values..."

gnuplot << EOF
set term postscript
set output "./plots/grd/$filename.eps"
set contour base
set view map
#set view 0,0,1
unset surface
set title ""
set cntrparam levels 50
splot "./data/grd/$filename.grd" using 2:1:3 with image notitle
EOF

