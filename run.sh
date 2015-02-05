#!/bin/bash

#	$? - exit status variable (? is wildcard)
#	() - cimmand group, starts a subshell

#	mkdir -p - creates required parent directories if non-existent
#	rm -f 	- does not give an error if rm fails		

set -o errexit	 # exit if any statement returns a non-true value
set -o nounset	 # exits if attempt to use unititialized value
# set -o pipefail	# sets pipes to fail if fail at any point of the pipe

	# set -e does not check whether $? is true,
	# 	 , in such a case, use the following:
	#	if ! command; then echo "command failed"; exit 1; fi

#	read -p "message"	pauses program until enter is struck

#	This shell script is for the program to solve problems in electrostatics. 
#	It will ask the user a number of questions and hence run the program defined
#	 below with the required parameters. (More detail below)
#	It will ask whether a plot is to be produce (and which).

# Program files
PROG_NAME="main"
DATA_OUTPUT="the_datafile.dat"
PLOT_OUTPUT="test.eps"
PLOT_SCRIPT="test_plot.sh"
PLOT_VIEWER="gv"

# Plot-arguments in main file
PLOT_FIELD_LINES=0
PLOT_POTENTIAL=1
PLOT_GRAD=2


function read_ans() {
	read -n1 -p "$*" answer
	echo -e "\n"
}

function read_val() {
	read -p "$*" val
	echo -e "\n"
}

function read_file() {
	read -p "$*" fil
	echo -e "\n"
}



echo -e "\nCommencing program...\n"
clear
echo -e "\n\n\n\t\tTHIS IS FOR THE PROGRAM $PROG_NAME "

#	echo -e "\n\n\t\t Do you wish to run this script didactically?"

echo -e "\n\n\t	I will now ask for a number of parameters to produce the required plots"



#
#	FOR SYSTEM A: Ask for analytic and range, radius and so on for numerical
#
#


echo -e "\n\n\t\t"
read_ans 'Do you wish to run the program to find a solution to system A? [Y/N] ' 
while [ $answer != "Y" ] && [ $answer != "y" ] && [ $answer != "N" ] && [ $answer != "n" ]
do
	read_ans 'Wrong input, please try again [Y/N] '
done
case $answer in
	y|Y) SYSTEM_A=1 ;;
	n|N) SYSTEM_A=0 ;;
esac


#
#	NUMERIC: system A
#


if [ $SYSTEM_A -eq 1 ]
then
	echo -e "\n\n\t\t"
	read_ans 'Do you wish to see the numerical solution to system A? [Y/N] ' 
	while [ $answer != "Y" ] && [ $answer != "y" ] && [ $answer != "N" ] && [ $answer != "n" ]
	do
		read_ans 'Wrong input, please try again [Y/N] '
	done
	case $answer in
		y|Y) NUMERIC=1 ;;
		n|N) NUMERIC=0 ;;
	esac

	if [ $NUMERIC -eq 1 ]
	then
		echo -e "\n\t Input the values for the size of the grid[Min][Max], the stepsize[ds] and the radius of the circle[r]"
		echo -e	"\t  also, the number of iteration[No. iter]"
		read GRID_MIN GRID_MAX STEP_SIZE RADIUS ITER_COUNT
	fi

	#
	#	HERE WILL BE CHECKS FOR ALL THE PARAMETERS
	#
fi


#
#	ANALYTIC, system A
#

if [ $SYSTEM_A -eq 1 ]
then
	echo -e "\n\n\t\t"
	read_ans 'Do you wish to see the analytic solution to system A? [Y/N] ' 
	while [ $answer != "Y" ] && [ $answer != "y" ] && [ $answer != "N" ] && [ $answer != "n" ]
	do
		read_ans 'Wrong input, please try again [Y/N] '
	done
	case $answer in
		y|Y) ANALYTIC=1 ;;
		n|N) ANALYTIC=0 ;;
		*)   echo "Wrong answer now breaking"; exit 1 ;;
	esac
fi


#
#	UNCERTAINTY TOLERANCE, of whole program
#
#		This error tolerance will control when the program will end by checking
#		 whether, upon each iteration, if the change in the matrix was sufficient.

DECIMAL_TEST='^[0-9]+([.][0-9]+)?$'
echo -e "\n\n\t\t"
read_val 'Enter the required error tolerance in the numerical solution [err>0] ' 
while ! [[ $val =~ $DECIMAL_TEST ]]
do
	read_val 'Input must be a positive decimal '
done

UNCERTAINTY_TOLERANCE=$val


#
#	TIMER
#
#		To view timers (written in the program, to be on, must be written in the main file)


echo -e "\n\n\t\t"
read_ans 'Do you wish an output of the program progress (timer)? [Y/N] ' 
while [ $answer != "Y" ] && [ $answer != "y" ] && [ $answer != "N" ] && [ $answer != "n" ]
do
	read_ans 'Wrong input, please try again [Y/N] '
done
case $answer in
	y|Y) TIMER=1 ;;
	n|N) TIMER=0 ;;
	*)   echo "Wrong answer now breaking"; exit 1 ;;
esac

#
#	PLOTS
#
#		In the main file, data for different plots are produced depending on an index passed as 
#		 an argument
#
#	HAS TO BE FIXED AS TO FIT THE MAIN PROGRAM


echo -e "\n\n\t\t"
read_ans 'Do you wish to view any plot? [Y/N] ' 
while [ $answer != "Y" ] && [ $answer != "y" ] && [ $answer != "N" ] && [ $answer != "n" ]
do
	read_ans 'Wrong input, please try again [Y/N] '
done
case $answer in
	y|Y) PLOT_YN=1 ;;
	n|N) PLOT_YN=0 ;;
	*)   echo "Wrong answer now breaking"; exit 1 ;;
esac

if [ $PLOT_YN -eq 1 ]
then
	echo -e "\n\n\t\t" 
	read_ans 'Which plot do you wish to view, field lines[F], potential[P], gradient[G] or none[N]? '
	while [ $answer != "F" ] && [ $answer != "P" ] && [ $answer != "G" ] && [ $answer != "N" ]
	do
		read_ans 'Wrong input, please try again [F,P,G,N] '
	done
	case $answer in
		f|F) PLOT_INDEX=0 ;;
		p|P) PLOT_INDEX=1 ;;
		g|G) PLOT_INDEX=2 ;;
		n|N) continue
	esac
fi

echo "PLOT INDEX IS: $PLOT_INDEX"

#
#	INPUT FILENAME
#
#		If analytic solution is not being considered, then ask for an input file to solve for the 
#		 required system.

GENERIC_BMP_FILE='^[A-Za-z0-9_]+\.bmp$'
if [ $SYSTEM_A -eq 0 ] && [ $ANALYTIC -eq 0 ]
then
	echo -e "\n\n\t\t"
	read_file 'Please enter the bitmap to use as input? [filename.bmp] ' 
		# readable bmp-file exists and is non-zero
	while ! [[ $fil =~ $GENERIC_BMP_FILE ]] || ! [[ -s "$fil" ]] || [[ -r "$fil" ]] 
	do
		read_file 'Needs to be a readable, non-empty file with .bmp extension. Retry '
	done
	INPUT_FILE=$fil
fi

#
#	SUMMARY
#
#		Summarises the parameters inputted, or asks to restart.

echo -e "\n\n\t\t Below follows a summary of the settings:"
sleep 1

# echo -e "\n Didactic:"

echo -e "\n System A:"
	case $SYSTEM_A in
		0) echo -e "\tOFF" ;;
		1) echo -e "\tON" ;;
	esac

if [ $SYSTEM_A -eq 1 ]
then
	echo -e "\n Analytic:"
		case $ANALYTIC in
			0) echo -e "\tOFF" ;;
			1) echo -e "\tON" ;;
		esac

	echo -e "\n Numerical (A):"
		case $NUMERIC in
			0) echo -e "\tOFF" ;;
			1) echo -e "\tON" ;;
		esac
	if [ $NUMERIC -eq 1 ]
	then
		echo -e "\n   The parameters passed to the function are:"
		echo -e "\t - Min. val      - $GRID_MIN"
		echo -e "\t - Max. val      - $GRID_MAX"
		echo -e "\t - Stepsize      - $STEP_SIZE"
		echo -e "\t - Circle radius - $RADIUS"
		echo -e "\t - No. Iteration - $ITER_COUNT"
	fi
fi

echo -e "\n Uncertainty:"
	echo -e "\tε = $UNCERTAINTY_TOLERANCE"

echo -e "\n Plot:"
	case $PLOT_YN in
		0) echo -e "\tOFF" ;;
		1) echo -e "\tON" ;;
	esac

if [ $PLOT_YN -eq 1 ]
then
	echo -e "\n Plot type:"
		case $PLOT_INDEX in
			0) echo -e "\tElectic field" ;;
			1) echo -e "\tPotential Field" ;;
			2) echo -e "\tGradients"
		esac
fi

echo -e "\n Timer:"
	case $TIMER in
		0) echo -e "\tOFF" ;;
		1) echo -e "\tON"  ;;
	esac

if [ $SYSTEM_A -eq 0 ] && [ $ANALYTIC -eq 0 ]
then
	echo -e "\n Inputfile:"
		echo -e "\t$INPUT_FILE"
fi

echo ""
read -p "Press [ENTER] to continue"

#
#	COMPILE ALL REQUIRED PROGRAMS
#

echo -e "\n\n\n\t\tNow compiling the required files:"

make 

echo -e "\n\t Compilation finilized. Now running the program with the parameters given"


#
#	PLOTTING IF ASKED FOR
#

if [ $PLOT_YN -eq 1 ]
then
		# NOTE: this works only if a plot is required for the moment!
	./$PROG_NAME $GRID_MIN $GRID_MAX $STEP_SIZE $RADIUS $ITER_COUNT $PLOT_INDEX  

	echo -e "\n\t Program run, data written to $DATA_OUTPUT"
	
	sleep 1

	echo -e "\n\t Creating now plot "
		./$PLOT_SCRIPT

	sleep 1

	echo -e "\n\t Opening plot:"
		$PLOT_VIEWER $PLOT_OUTPUT & 

fi
	

exit 0


#
#	ANALYSIS
#
#		Ask whether onw wants to add an analysis part.
#		Store data outputfile along with parameters

#
#	STORE DATA
#
#		This section will ask whether user wants to store the data created (.dat file) and where. 
#		Ask user for dilename or just set the date and parameters?

#
#	DEFAULT/TEST - RUN
#
#		Create a section for a test run, to verify certain aspects of the program



