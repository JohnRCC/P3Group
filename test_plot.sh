#!/bin/bash

gnuplot << EOF
set term postscript solid
set output "pot.eps"
set contour base
set view map
set key off
unset surface
set title ""
set cntrparam levels 200
splot "pot.dat" using 2:1:3 with image notitle,\
"pot.dat" using 2:1:3 with lines notitle
EOF

