#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include "../headers/funcs.h"

using namespace std;

float SphericalPotential(float x, float y, float r,float Plate_separation);
float CylindricalPotential(float x, float y, float r,float Plate_separation);

int analytic(float smin,float ds,float smax,float r,int maxres,int silence)
{
 
int row,column,i;
// Choice of solution derived from cylindrical [true] or sphericsal [false] coordinates
bool bCylindricalCoords = true;

/*
 *	GENERATE MATRIX
 *
 *		Sets the values as defined by the parameters passed such that
 *		 System A's setting is satisfied: 
 *		 	- Plates of opposite polarities at either side
 *                         vrom the vertical (think y-axis)
 *		 	- Ground, circular plate positioned at middle
 *
 */


// Initalise matrix
	
 double time;
 if (silence == 0) {
   time = timerstart();
   cout << "Calculating analytical solution "; }

if (silence == 0) {
  if (bCylindricalCoords) {cout << "with Cylindrical Coordinates... " << flush; }
  else {cout << "with Spherical Coordinates..." << flush;}
}
 // Set the scale of the matrix (for compatibility with meshing)
 ds = ds / (float)maxres;
 
 // Matrix size in terms of min and max values
 float d = abs(smax-smin);
 long matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1); 
 
 if(d < pow(r,2) && silence == 0) {
   cout << "\n WARNING! - Analytic function may give an erroneous answer."
	<< "d < r^2 \n... " << flush; }
 
 
 float mid = (matsize/2.0),x,y;// - (fmod(matsize,2.0));
 //double valsA[matsize][matsize]; 

 ofstream analyticfile;
analyticfile.open("analytical.dat");

// Cast values
for(row=0;row<matsize;row++) {
  for(column=0;column<matsize;column++) {
    // Sets "true" coordinates for x and y
    y=cf(row,smin,ds,1);
    x=cf(column,smin,ds,1);

    if (column == 0) {	
    	analyticfile << row << " " << column << " " << 1 << "\n";
    }
    else if (column == matsize-1) {
	analyticfile << row << " " << column << " " << -1 << "\n";
    }
    else if( (pow((cf(row,smin,ds,1)-cf(mid,smin,ds,1)),2.0) + pow((cf(column,smin,ds,1)-cf(mid,smin,ds,1)),2.0)) < pow(r,2.0) ) {
	analyticfile << row << " " << column << " " << -0 << "\n";
    }
    else {
     	// Only values that differ from main
        if (bCylindricalCoords) {
          analyticfile << row << " " << column << " " << CylindricalPotential(x,y,r,d) << "\n";
        }
        else {
          analyticfile << row << " " << column << " " << SphericalPotential(x,y,r,d) << "\n";
        }
     }
  }
  analyticfile << "\n";
}

 analyticfile.close();
 
 if (silence == 0) {
   cout << "done. (" << timerend(time,silence) << "s)" << endl; }
 
 return 0;
 
 }
 

/*
 *	FUNCTIONS
 */
 
// Function holding solution of the potential derived with cylindrical coordinates
float CylindricalPotential(float x, float y, float r,float Plate_separation) {
  float v=0, Plate_Vdiff=2; //Plate_separation=r*pow(10,2);

// E = DV/d (Volage difference of plates(2V) / separation of plates (xmax-xmin) )
  float field = Plate_Vdiff/Plate_separation;
  //calculating the analytical solution
    v = (-1)*field*x*( 1 - ((r*r)/( x*x + y*y )) );     // Cylindrical coordinate
  return v;
}

// Function holding solution of the potential derived with spherical coordinates
float SphericalPotential(float x, float y, float r,float Plate_separation) {
  float v=0, Plate_Vdiff=2; //Plate_separation=r*pow(10,2);

// E = DV/d (Volage difference of plates(2V) / separation of plates (xmax-xmin) )
  float field = Plate_Vdiff/Plate_separation;
  //calculating the analytical solution
    v=(-1)*field*x*(1-((r*r*r)/(pow((x*x)+(y*y),1.5))));    // Spherical coordinates

  return v;
}

//CoordiFy converts the matrix location of a point into its physical coordinate
float cf(float matind,float min,float ds,int maxres) {
  // matind = index of value in array, min = min true coord value,
  // ds = coord division, maxres = scaling factor (used in main, not used here)
  return min + ((ds/(float)maxres)*matind);
}
