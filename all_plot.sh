#!/bin/bash

if [ -f pot.dat ]
then
echo "Graphing potential..."
gnuplot << EOF
set term postscript
set output "pot.eps"
set contour base
set view map
unset surface
set title ""
set cntrparam levels 50
splot "pot.dat" using 2:1:3 with image notitle#, \
   #   "pot.dat" using 2:1:3 with line notitle
#plot "field.dat" using 2:1:3:4 with vectors head size 01.20,20 notitle	# line for contour plot
EOF
fi

if [ -f analytical.dat ]
then
echo "Graphing analytical potential..."
gnuplot << EOF
set term postscript
set output "analytical.eps"
set contour base
set view map
unset surface
set title ""
set cntrparam levels 50
splot "analytical.dat" using 2:1:3 with image notitle#, \
   #   "analytical.dat" using 2:1:3 with line notitle
EOF
fi

if [ -f grad.dat ]
then
echo "Graphing gradient..."
gnuplot << EOF
set term postscript
set output "grad.eps"
set contour base
set view map
unset surface
set title ""
set cntrparam levels 50
splot "grad.dat" using 2:1:3 with image notitle
EOF
fi

if [ -f field.dat ]
then
echo "Graphing field lines..."
gnuplot << EOF
set term postscript
set output "field.eps"
set contour base
set view map
set view 0,0,1
unset surface
set title ""
set cntrparam levels 50
plot "field.dat" using 2:1:3:4 with vectors head size 01.20,20 notitle	# line for contour plot
EOF
fi

echo "Done."
