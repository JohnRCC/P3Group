#!/bin/bash

if [[ $# -ge 2 ]] ; then
    ./main
else
    echo ""
    echo ""
    echo ""
    echo "Welcome to Electric! Please choose an option:"
    echo "  [1] Image Mode    - Calculate an electric field from a pre-defined bitmap."
    echo "  [2] Analytic Mode - Do the numerical calculation for System A for comparison."
    echo "  [3] Plotting Mode - Plot graphs of potential or field from pre-generated data."
    echo ""
    echo ""
    read type

    if [[ $type -eq "1" ]] ; then
	echo "Image mode selected. Enter bitmap filename (with extension):"
	read image
	echo "File "$image" selected. Plot potential? [y/n]"
	read pot
	opts=0
	if [[ $pot -eq "y" ]] ; then
	    opts=$(( $opts+2 ))
	    "Plotting potential."
	fi
	echo "Plot field lines? [y/n]"
	read fie
	if [[ $fie -eq "y" ]] ; then
	    opts=$(( $opts+4 ))
	    echo "Plotting field lines."
	fi
	echo "Desired error tolerance (for iterations)?"
	read errtol
	echo "Apply smoothing? (Optional, hit Enter to skip)"
	read smooth
	echo "Ready. Commencing programme..."
	
	./main $image $errtol $opts $smooth

    elif [[ $type -eq "2" ]] ; then
	echo "Analytic mode selected. Enter MinX, MaxX, dX, R:"
	read minx maxx dx r
	echo "Plot potential? [y/n]"
	read pot
	opts=0
	if [[ $pot -eq "y" ]] ; then
	    opts=$(( $opts+2 ))
	    "Plotting potential."
	fi
	echo "Plot field lines? [y/n]"
	read fie
	if [[ $fie -eq "y" ]] ; then
	    opts=$(( $opts+4 ))
	    echo "Plotting field lines."
	fi
	echo "Error tolerance (for iterations)?"
	read errtol
	echo "Ready. Commencing programme..."
	./main $minx $maxx $dx $r $errtol $opts
    fi



fi
exit 0