#!/bin/bash

<<<<<<< HEAD
gnuplot << EOF
set term postscript
set output "pot.eps"
=======
# input argument from electric
filename=$1

echo "Plotting potential..."

gnuplot << EOF
set term postscript
set output "./plots/pot/$filename.eps"
>>>>>>> master
set contour base
set view map
#set view 0,0,1
unset surface
set title ""
set cntrparam levels 50
<<<<<<< HEAD
splot "pot.dat" using 2:1:3 with image notitle
#plot "field.dat" using 2:1:3:4 with vectors head size 01.20,20 notitle	# line for contour plot
=======
splot "./data/pot/$filename.pot" using 2:1:3 with image notitle
>>>>>>> master
EOF

