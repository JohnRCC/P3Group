#!/bin/bash

# Electric is a programme to simulate generic electrostatic systems.
# There are three modes available.


# Give the script a nice clean screen
clear

# Flag to ensure programme loops effectively
run=yes

while [ $run = "yes" ] ; do

    # Condition to automatically run programme is user wishes (i.e. with user-inputted arguments)
    if [[ $# -ge 2 ]] ; then
	./main
	exit 0
    else

	cat ./resources/intro # Display welcome screen

	read choice ; clear

	# If user did not pick a valid option
	while [ $choice != "i" ] && [ $choice != "a" ] && [ $choice != "p" ] && [ $choice != "h" ] && [ $choice != "q" ] ; do
	    echo "Invalid option selected. Please choose one: [I]mage, [A]nalytic, [P]lotting, [H]elp, [Q]uit."
	    read choice ; clear
	done

	# Now run through each case in turn
	
	# Case for quitting
	if [[ $choice == "q" ]] ; then
	    clear
	    echo "Quitting..."
	    exit 0
	fi

	# Case for help text
	if [[ $choice == "h" ]] ; then
	    less ./resources/help
	fi

	# Case for image / analytical modes (choosing output file first)
	if [[ $choice == "i" || $choice == "a" ]] ; then
	    
	    # Gets rid of old pot.dat, field.dat, grad.dat files if present (they shouldn't be)
	    if [[ -f pot.dat ]] ; then
		rm pot.dat
	    fi

	    if [[ -f field.dat ]] ; then
		rm field.dat
	    fi

	    if [[ -f grad.dat ]] ; then
		rm grad.dat
	    fi
	    
	    # Gives the correct name based on choice
	    if [[ $choice == "i" ]] ; then
		choicename=Image
	    elif [[ $choice == "a" ]] ; then
		choicename=Analytic
	    fi
	    
	    # File selector. If file already exists, will keep old files the same and add a number to the new one.
	    echo $choicename "mode selected. Please name your output file (will be used for potential, field and grad if requested):"
	    read outfile ; clear
	    
	    if [[ -f ./data/pot/$outfile.pot || -f ./data/fld/$outfile.fld || -f ./data/grd/$outfile.grd ]] ; then
		
		fileloop=yes # Maintains while loop until unused filename is found.
		fileiter=0
		
		echo "File(s) "$outfile" already present."
		
		while [ $fileloop == "yes" ] ; do
		    
		    fileiter=$(( $fileiter + 1 ))
		    
		    # in first case, check if any numbered versions of file exist
		#   if [[ ! -f ./data/pot/$outfile\_1.pot || ! -f ./data/fld/$outfile\_1.fld || ! -f ./data/grd/$outfile\_1.grd ]] ; then
		#	outfile=$outfile\_1
		#	fileloop=no
		    if [[ ! -f ./data/pot/$outfile\_$fileiter.pot || ! -f ./data/fld/$outfile\_$fileiter.fld || ! -f ./data/grd/$outfile\_$fileiter.grd ]] ; then
			outfile=$outfile\_$fileiter
			fileloop=no
		    fi
		done
		
		echo "Renaming to "$outfile"."
	    fi
	    
	    # Initialise variable for generating different files. (needed in both cases)
	    option=0

	    # Now branch off into image or analytic mode
	    if [[ $choice == "i" ]] ; then
		
		clear

		echo "Please enter image file name (with extension):"
		read imagefile ; clear

		# Validity check on filename
		while [ ! -f $imagefile ] ; do
		    echo "Invalid image specified. Please try again:"
		    read imagefile ; clear
		done

		echo $imagefile" selected. Calculate equipotentials? [y/n]"
		read pot ; clear

		while [ $pot != "y" ] && [ $pot != "n" ] ; do
		    echo "Please type y or n:"
		    read pot ; clear
		done

		if [[ $pot == "y" ]] ; then
		    option=$(( $option + 2 ))
		    echo "Calculating equipotentials. Calculate electric field? [y/n]"
		elif [[ $pot == "n" ]] ; then
		    touch ./data/pot/$outfile.pot # to maintain symmetry in filenames
		    echo "Not calculating equipotentials. Calculate electric field? [y/n]"
		fi

		read fld ; clear

		while [ $fld != "y" ] && [ $fld != "n" ] ; do
		    echo "Please type y or n:"
		    read fld ; clear
		done

		if [[ $fld == "y" ]] ; then
		    option=$(( $option + 4 ))
		    echo "Calculating electric field. Output grad test results? [y/n] (Advanced users only: recommended choice no)"
		elif [[ $fld == "n" ]] ; then
		    touch ./data/fld/$outfile.fld
		    echo "Not calculating electric field. Output grad test results? [y/n] (Advanced users only: recommended choice no)"
		fi

		read grd ; clear
		
		while [ $grd != "y" ] && [ $grd != "n" ] ; do
		    echo "Please type y or n:"
		    read grd; clear
		done

		if [[ $grd == "y" ]] ; then
		    option=$(( $option + 1 ))
		    echo "Calculating grad test results. Error tolerance? (Used to determine number of iterations, a good guideline number is ____."
		elif [[ $grd == "n" ]] ; then
		    touch ./data/grd/$outfile.grd
		    echo "Not calculating grad test results. Error tolerance? (Used to determine number of iterations, a good guideline number is ____."
		fi

		echo "HERE WE GOOOOOOOO" 
	    fi
	    
	    if [[ $choice == "a" ]] ; then
		echo anal
		echo "woo" > ./data/pot/$outfile.pot
		echo "yay" > ./data/fld/$outfile.fld
		echo "gud" > ./data/grd/$outfile.grd
	    fi
	fi
    fi
done