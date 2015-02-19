#!/bin/bash

# give the programme a nice clean screen
clear

run=yes

while [ $run = "yes" ] ; do

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

	    echo "Image mode selected. Please enter desired output filename (minus extension):"
	    read output ; clear
	    
	    if [[ -f ./data/pot/$output.pot || -f ./data/field/$output.fld ]] ; then #if filename has already been used
		fileloop=yes
		fileiter=0
		while [ $fileloop = "yes" ] ; do
		    
		    fileiter=$(( $fileiter + 1 ))
		    
		    if [[ ! -f ./data/pot/$output\_0.pot || ! -f ./data/field/$output\_0.fld ]] ; then
			fileloop=no
			mv ./data/pot/$output.pot ./data/pot/$output\_0.pot
			mv ./data/field/$output.fld ./data/field/$output\_0.field
			fileiter=0
		    fi

		    if [[ -f ./data/pot/$output\_$fileiter.pot || -f ./data/field/$output\_$fileiter.fld ]] ; then #first time it finds a free numbered file
			fileloop=no
			mv ./data/pot/$output\_$fileiter.pot ./data/pot/$output\_$(( $fileiter + 1 )).pot
			mv ./data/field/$output\_$fileiter.fld ./data/field/$output\_$(( $fileiter + 1 )).fld
		    fi

		done
		echo "File "$output" already present. Renaming old files to "$output\_$fileiter"."
	    fi
		    
	    
	    echo "Enter bitmap filename (with extension):"
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
		    echo "Old potential data detected. Renaming to pot_"$da".dat and placing in data/pot/ folder."
		    mv pot.dat ./data/pot/pot_$da.dat
		fi
	    fi
	    
	    echo "Plot field lines? [y/n]"
	    read fie ; clear
	    
	    while [ $fie != "y" ] && [ $fie != "n" ] ; do # same as for pot
		clear
		echo "Please enter y or n:"
		read fie
	    done ; clear
	    
	    #clear for debug, i want to see what happens
	    
	    if [[ $fie -eq "y" ]] ; then
		opts=$(( $opts+4 ))
		echo "Calculating field lines."
		if [[ -f "field.dat" ]] ; then
		    echo "Old field data detected. Renaming to field_"$da".dat and placing in data/field/ folder."
		    mv field.dat ./data/field/field_$da.dat
		fi
	    fi
	    
	    echo "Desired error tolerance (for iterations)?"
	    read errtol ; clear
	    
	    echo "Apply smoothing? (1 = yes. Optional, hit Enter to skip)"
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
		sleep 1
	    fi
	fi
	
	if [[ $type -eq "2" ]] ; then
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

	if [[ $type -eq "3" ]] ; then
	    clear
	    echo "Plotting mode selected. Please enter filename (with .dat extension):"
	    read plotfile ; clear

	    while [ ! -f $plotfile ] ; do
		echo "Invalid file specified. Please try again:"
		read plotfile ; clear
	    done

	    echo "File "$plotfile" selected."
	    echo "PLOTTER IN HERE"
	fi
	
	
	
    fi
    
done

exit 0