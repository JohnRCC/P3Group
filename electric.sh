#!/bin/bash

# give the programme a nice clean screen
clear

if [[ $# -ge 2 ]] ; then
    ./main
else
    #get date param for renaming old files
    da=`date +"%F-%H-%M"`
    
    echo ""
    echo ""
    echo ""
    echo "Welcome to Electric! Please choose an option:"
    echo "  [1] Image Mode    - Calculate an electric field from a pre-defined bitmap."
    echo "  [2] Analytic Mode - Do the numerical calculation for System A for comparison."
    echo "  [3] Plotting Mode - Plot graphs of potential or field from pre-generated data."
    echo ""
    echo ""
    read type ; clear

    if [[ $type -eq "1" ]] ; then  # case for image mode

	echo "Image mode selected. Enter bitmap filename (with extension):"
	read image ; clear
	
	while [ ! -f $image ] ; do # if image does not exist
	    clear
	    echo $image" not found. Please enter an existing image file:"
	    read image
	done ; clear

	echo "File "$image" selected. Plot potential? [y/n]"
	read pot ; clear
	
	while [ $pot != "y" ] && [ $pot != "n" ] ; do # if the user is stupid and can't type y or n
	    clear
	    echo "Please enter y or n:"
	    read pot
	done ; clear

	opts=0

	if [[ $pot = "y" ]] ; then # add 2 to opts and rename old data file if it exists
	    opts=$(( $opts+2 ))
	    echo "Calculating potential."
	    if [[ -f "pot.dat" ]] ; then
		echo "Old potential data detected. Renaming to pot_"$da".dat"
		mv pot.dat pot_$da.dat
	    fi
	fi

	echo "Plot field lines? [y/n]"
	read fie ; clear

	while [ $fie != "y" ] && [ $fie != "n" ] ; do # same as for pot
	    clear
	    echo "Please enter y or n:"
	    read fie
	done ; clear

	#clear #for debug, i want to see what happens

	if [[ $fie -eq "y" ]] ; then
	    opts=$(( $opts+4 ))
	    echo "Calculating field lines."
	    if [[ -f "field.dat" ]] ; then
		echo "Old field data detected. Renaming to field_"$da".dat"
		mv field.dat field_$da.dat
	    fi
	fi

	echo "Desired error tolerance (for iterations)?"
	read errtol ; clear

	echo "Apply smoothing? (Optional, hit Enter to skip)"
	read smooth ; clear

	echo "Ready. Commencing programme..."
	./main $image $errtol $opts $smooth

	echo "Calculation complete. Would you like to plot now? [y/n]"
	read plot ; clear

	while [ $plot != "y" ] && [ $plot != "n" ] ; do
	    clear
	    echo "Please enter y or n."
	    read plot
	done ; clear
	
	if [[ $plot = "y" ]] ; then
	    type=3
	elif [[ $plot = "n" ]] ; then
	    echo "Returning to title screen..."
	    sleep 2
	    ./electric.sh
	    exit 0
	fi
	

    elif [[ $type -eq "2" ]] ; then
	echo "Analytic mode selected. Enter MinX, MaxX, dX, R:"
	read minx maxx dx r
	echo "Plot potential? [y/n]"
	read pot
	opts=0
	if [[ $pot -eq "y" ]] ; then
	    opts=$(( $opts+2 ))
	    "Calculating potential."
	fi
	echo "Plot field lines? [y/n]"
	read fie
	if [[ $fie -eq "y" ]] ; then
	    opts=$(( $opts+4 ))
	    echo "Calculating field lines."
	fi
	echo "Error tolerance (for iterations)?"
	read errtol
	echo "Ready. Commencing programme..."

	./main $minx $maxx $dx $r $errtol $opts
	

    fi



fi
exit 0