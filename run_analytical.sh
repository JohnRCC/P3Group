#!/bin/bash

set -e

echo "MAKING..."
make analytical

echo "RUNNING..."
./analytical/analytical -10 10 0.5 $1 1 1

echo "PLOTTING..."
./plot_analytical.sh

echo "SHOWING PLOT..."
gv analytical_plot.eps
