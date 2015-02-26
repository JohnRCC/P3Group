#include <cmath>
#include <iostream>
#include <stdlib.h>

// #######################################################
// #	program allows user to define an acceptable 	   #
// #	percentage error for the Analytical solution. 	   #
// #         						   #
// #	arg1: Confidence level (0,100) 		   #
// #  	arg2: Min x			                  #
// # 	arg3: Max x					  #
// #    arg4: Radius					  #
// #	Returns 1 (and advice) if confidence level 	  #
// #		cannot be reached.	  		  #
// #	Returns 0 if confidence level can be reached. 	  #
// #######################################################

using namespace std;


int main(int argc, char* argv[])
	// OR int main(int argc, char* argv) 
{

      //Suppressing this for now as is should only be called within shell script
	if(argc != 5){
	//	cout << "arguments: confidence percent (0,100); distance between plates; radius of circle." << endl;
		return 1;
	}

	float confidence, minx, maxx, radius, distance; 

	confidence = strtof(argv[1],NULL); 
	minx = strtof(argv[2],NULL);
	maxx = strtof(argv[3],NULL);
	distance = maxx-minx;
	radius = strtof(argv[4],NULL);

	float cubeconf = cbrt((100-confidence)/100);

	float ratio = (2*radius)/distance;

	if (ratio > cubeconf){

		cout << "cannot give " << confidence << 
		"\% confidence with these dimensions;\nmake radius " <<
		"at most " << (distance/2)*cubeconf << endl;
		return 1;
	}
	else{
		cout << "zero!\n";	
		return 0;
	}
}
