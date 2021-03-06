#!/bin/bash

# filename from electric.sh
filename=$1

echo "Plotting electric field..."

gnuplot << EOF
set term postscript
set output "./plots/fld/$filename.eps"
set contour base
set view map
#set view 0,0,1
unset surface
set title ""
set cntrparam levels 50
plot "./data/fld/$filename.fld" using 2:1:3:4 with vectors head size 01.20,20 notitle
EOF

