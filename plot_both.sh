#!/bin/bash

set -e

echo "PLOTTING ANALYTICAL..."
./plot_analytical.sh
sleep 1
echo "PLOTTING NUMERICAL..."
./plot_main.sh
sleep 1
echo "SHOWING PLOTS..."
sleep 1
gv analytical_plot.eps &
gv main_plot.eps &
