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

	//confidence = strtof(argv[1],NULL);
	confidence = 90;
	minx = strtof(argv[2],NULL);
	maxx = strtof(argv[3],NULL);
	distance = maxx-minx;
	radius = strtof(argv[4],NULL);
	
	float recommended = (2*radius)/cbrt(0.1);

	float cubeconf = cbrt((100-confidence)/100);
	
	float ratio = (2*radius)/distance;

	if (ratio > cubeconf){

		/*cout << "cannot give " << confidence << 
		"\% confidence with these dimensions;\nmake radius " <<
		"at most " << (distance/2)*cubeconf << endl;*/
		
		cout<<"Warning: dimensions specified unlikely to give accurate results (less than "<<confidence<<"\% confidence)."<<endl;
		cout<<"Type 'r' to re-define values, or 'y' to proceed. (A good plate separation for R="<<radius<<" is "<<int(recommended+1)<<".)"<<endl;
		
		return 1;
	}
	else{
		//cout << "zero!\n";	
		cout<<"Dimensions specified will give accurate results with "<<100*(1-pow(ratio,3))<<"\% confidence."<<endl;
		cout<<"Type 'r' to redefine these values, or 'y' to proceed."<<endl;
		
		return 0;
	}
}
