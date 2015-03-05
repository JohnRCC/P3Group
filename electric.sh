#!/bin/bash

<<<<<<< HEAD
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
=======
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

	    checkloop=yes #loop in case user wants to re-enter variables
	    while [ $checkloop = "yes" ] ; do
	    
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
		    sleep 1
		fi
		
	    # Initialise variable for generating different files. (needed in both cases)
		option=0
		
	    # Now branch off into image or analytic mode
		if [[ $choice == "i" ]] ; then
		    
		#   clear
		    
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
			potcheck="| Potential" #used for names.txt
			option=$(( $option + 2 ))
			echo "Calculating equipotentials. Calculate electric field? [y/n]"
		    elif [[ $pot == "n" ]] ; then
		#	touch ./data/pot/$outfile.pot # to maintain symmetry in filenames
			potcheck=" "
			echo "Not calculating equipotentials. Calculate electric field? [y/n]"
		    fi
		    
		    read fld ; clear
		    
		    while [ $fld != "y" ] && [ $fld != "n" ] ; do
			echo "Please type y or n:"
			read fld ; clear
		    done
		    
		    if [[ $fld == "y" ]] ; then
			fldcheck="| Field" # used for names.txt
			option=$(( $option + 4 ))
			echo "Calculating electric field. Output grad test results? [y/n] (Advanced users only: recommended choice no)"
		    elif [[ $fld == "n" ]] ; then
		#	touch ./data/fld/$outfile.fld
			fldcheck=" "
			echo "Not calculating electric field. Output grad test results? [y/n] (Advanced users only: recommended choice no)"
		    fi
		    
		    read grd ; clear
		    
		    while [ $grd != "y" ] && [ $grd != "n" ] ; do
			echo "Please type y or n:"
			read grd; clear
		    done
		    
		    if [[ $grd == "y" ]] ; then
			grdcheck="| Gradient" # used for names.txt
			option=$(( $option + 1 ))
			echo "Calculating grad test results. Error tolerance? (Used to determine number of iterations, a good guideline number is ____."
		    elif [[ $grd == "n" ]] ; then
		#	touch ./data/grd/$outfile.grd
			grdchedk=" "
			echo "Not calculating grad test results. Error tolerance? (Used to determine number of iterations, a good guideline number is ____."
		    fi
		    
		    read errtol ; clear
		    
		    echo "Apply smoothing? [y/n]"
		    read smooth ; clear
		    
		    while [ $smooth != "y" ] && [ $smooth != "n" ] ; do
			echo "Please type y or n:"
			read smooth ; clear
		    done
		    
		    if [[ $smooth == "y" ]] ; then
			smoothnum=1
		    else
			smoothnum=0
			smooth=n
		    fi
		    
		    echo "Here are your settings. Type 'run' to begin or 'reset' to re-enter values."
		    echo "             Output: "$outfile
		    echo "         Image file: "$imagefile
		    echo "Calculate potential: "$pot
		    echo "    Calculate field: "$fld
		    echo " Calculate gradient: "$grd
		    echo "    Error tolerance: "$errtol
		    echo "          Smoothing: "$smooth
		    
		    #set go to "run" for now as leaving it blank is causing issues
		    go=run
		    
		    read go ; clear
		    
		    if [[ $go = "" ]] ; then
			go=run
		    fi

		    while [ $go != "run" ] && [ $go != "reset" ] ; do
			echo "Please type 'run' or 'reset':"
			read go ; clear
		    done
		    
		    if [[ $go = "run" ]] ; then
		      
			# first thing that needs doing is to update names.txt with this new file
			echo `date +"%D %T"` "--" $outfile "--" $potcheck $fldcheck $grdcheck >> ./data/.names.txt
		      
			checkloop=no
			./main $imagefile $errtol $option $smooth

			#move generated files into respective folders
			if [[ -f pot.dat ]] ; then
			    mv pot.dat ./data/pot/$outfile.pot
			else
			    cp ./data/pot/.empty.pot ./data/pot/$outfile.pot
			fi

			if [[ -f field.dat ]] ; then
			    mv field.dat ./data/fld/$outfile.fld
			else
			    cp ./data/fld/.empty.fld ./data/fld/$outfile.fld
			fi

			if [[ -f grad.dat ]] ; then
			    mv grad.dat ./data/grd/$outfile.grd
			else
			    cp ./data/grd/.empty.grd ./data/grd/$outfile.grd
			fi

			#used to go straight to plotting mode
			plotmode=0

			echo "Calculations complete. Go to plotting mode now? [y/n]"
			read plotq ; clear

			while [ $plotq != "y" ] && [ $plotq != "n" ] ; do
			    echo "Please enter y or n:"
			    read plotq ; clear 
			done

			if [[ $plotq = "y" ]] ; then
			    choice=p
			    plotmode=1
		            plotfile=$outfile
			fi
		    elif [[ $go = "reset" ]] ; then
			echo "Resetting..."
		    fi
		    
		    
		fi
		
		if [[ $choice == "a" ]] ; then
		    
		    valsloop=y
		    
		    while [ $valsloop = "y" ] ; do
		    
			echo "Please enter the minimum and maximum x values:"
			read minx maxx ; clear
		   
			echo "Please enter cylinder radius:"
			read radius ; clear
		   
			./errorA 90 $minx $maxx $radius
			read valscont ; clear
			
			while [ $valscont != "y" ] && [ $valscont != "r" ] ; do
			    echo "Please type 'y' to proceed or 'r' to redefine values:"
			    read valscont ; clear
			done
			
			if [[ $valscont = "y" ]] ; then
			    valsloop=n
			fi
			
		    done
		    
		    echo "Error tolerance / iteration number?"
		    read errtol ; clear	
		    
		fi
	    done
>>>>>>> master
	fi
	echo "Error tolerance (for iterations)?"
	read errtol
	echo "Ready. Commencing programme..."
	./main $minx $maxx $dx $r $errtol $opts
    fi

<<<<<<< HEAD


fi
exit 0
=======
	#plotting mode
	if [[ $choice = "p" ]] ; then
          plotcont=y
          while [ $plotcont = "y" ] ; do
		
 	    if [[ $plotmode = 0 ]] ; then
	  	echo "Entering plotting mode. Please enter filename you want to plot (without extension), or type 'list' for a list of files available for plotting:"
		
		listloop=y
		
		while [ $listloop = "y" ] ; do
		    read plotfile ; clear
		    if [[ $plotfile = "list" ]] ; then
		      echo "Showing plottable files. The 'Potential | Field | Gradient flags show what is available for plotting."
		      echo ""
	    	      cat ./data/.names.txt
		      echo ""
		      echo "Please enter the name of the file you want to plot (without extension):"
		    elif [[ -f ./data/pot/$plotfile.pot || -f ./data/fld/$plotfile.fld || -f ./data/grd/$plotfile.grd ]] ; then
			listloop=n
		    else
			echo "Invalid file specified (no files with that name exist). Please try again, or type list for a list of files available for plotting:"
		    fi
		done
            fi
		
	    # POTENTIAL PLOT
		if [[ `head -n 1 ./data/pot/$plotfile.pot` != "#dummy" ]] ; then
		    echo "File "$plotfile" selected. Plot potential? [y/n]"
		    read potplot ; clear
		
		    while [ $potplot != "y" ] && [ $potplot != "n" ] ; do
			echo "Please enter y or n:"
			read potplot ; clear
		    done
		
		    if [[ $potplot = "y" ]] ; then
			./pot_plot.sh $plotfile
			echo "Done. Plot can be found at ./plots/pot/"$plotfile".eps"
		    else
			echo "Skipping potential."
		    fi
		fi
		    
	    # FIELD PLOT
		if [[ `head -n 1 ./data/fld/$plotfile.fld` != "#dummy" ]] ; then
		    echo "Plot electric field? [y/n]"
		    read fldplot ; clear
		
		    while [ $fldplot != "y" ] && [ $fldplot != "n" ] ; do
			echo "Please enter y or n:"
			read fldplot ; clear
		    done
		
		    if [[ $fldplot = "y" ]] ; then
			./fld_plot.sh $plotfile
			echo "Done. Plot can be found at ./plots/fld/"$plotfile".eps"
		    else
			echo "Skipping electric field."
		    fi
		fi
		
	    # GRAD PLOT
		if [[ `head -n 1 ./data/grd/$plotfile.grd` != "#dummy" ]] ; then
		    echo "Plot gradient? [y/n] (Advanced users only. Recommended: no)"
		    read grdplot ; clear
		
		    while [ $grdplot != "y" ] && [ $grdplot != "n" ] ; do
			echo "Please enter y or n:"
			read grdplot ; clear
		    done
		
		    if [[ $grdplot = "y" ]] ; then
			./grd_plot.sh $plotfile
			echo "Done. Plot can be found at ./plots/grd/"$plotfile".eps"
		    else
			echo "Skipping gradient."
		    fi
		fi
		
		#come out of autoplot mode
		plotmode=0

		echo "Plotting complete. Plot more files? [y/n] (N will return you to the main menu.)"
		read plotcont ; clear
		
		while [ $plotcont != "y" ] && [ $plotcont != "n" ] ; do
		    echo "Please enter y or n:"
		    read plotcont ; clear
		done
	    done		
	fi
    fi
done
>>>>>>> master
