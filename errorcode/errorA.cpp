#include <cmath>
#include <iostream>
#include <stdlib.h>

// #######################################################
// #	program allows user to define an acceptable 	 #
// #	percentage error for the Analytical solution. 	 #
// #         						 #
// #	arg1: Confidence level (0,100) 		 	 #
// #  	arg2: Distance between plates			 #
// # 	arg3: Radius					 #
// #	Returns 1 (and advice) if confidence level 	 #
// #		cannot be reached.	  		 #
// #	Returns 0 if confidence level can be reached. 	 #
// #######################################################

using namespace std;


int main(int argc, char* argv[])
	// OR int main(int argc, char* argv) 
{

	if(argc != 4){
		cout << "arguments: confidence percent (0,100); distance between plates; radius of circle." << endl;
		return 1;
	}

	float confidence, distance, radius; 

	confidence = strtof(argv[1],NULL); 
	distance = strtof(argv[2],NULL); 
	radius = strtof(argv[3],NULL);

	float cubeconf = cbrt((100-confidence)/100);

	float ratio = (2*radius)/distance;

	if (ratio > cubeconf){

		cout << "cannot give " << confidence << 
		"\% confidence with these dimensions;\nmake radius " <<
		"less than " << (distance/2)*cubeconf << endl;
		return 1;
	}
	else{
		cout << "zero!\n";	
		return 0;
	}
}
