#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include "../headers/funcs.h"

using namespace std;

int analytic(float smin,float ds,float smax,float r)
{
 
int row,column,i;
      
/*
 *	GENERATE MATRIX
 *
 *		Sets the values as defined by the parameters passed such that
 *		 System A's setting is satisfied: 
 *		 	- Plates of opposite polarities at either side horiz.
 *		 	- Ground, circular plate positioned at middle
 */
 
 	// Matrix size separation in terms of r-d relation
 // 	Plate separation: 1.5 is the minimum. larger values of d should be tested 
// float d = pow(r,2);
// int matsize = d/ds - (fmod(d,ds));
// smin = -d/2.0;
// smax = d/2.0;

// Initalise matrix
	
	// Matrix size in terms of min and max values
	/**/
	  float d = abs(smax-smin);
	  int matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1); 
	 
	 if(d < pow(r,2)) {
		cout << " WARNING! - Analytic function may give an erroneous answer. d < r^2\n";
	 }
	 

float mid = (matsize/2.0) - (fmod(matsize,2.0)), x,y;
double valsA[matsize][matsize]; 


// Cast values
for(row=0;row<matsize;row++) {
  for(column=0;column<matsize;column++) {
	// Sets "true" coordinates for x and y
    x=cf(row,smin,ds);
    y=cf(column,smin,ds);

      if (row== 0) {
        valsA[row][column] = 1;
      }
      else if (row == matsize-1) {
	valsA[row][column] = -1;
      }
      else if ( (pow((cf(row,smin,ds)-cf(mid,smin,ds)),2.0) + pow((cf(column,smin,ds)-cf(mid,smin,ds)),2.0)) < pow(r,2.0) ) {
	valsA[row][column] = 0;
      }
      else {
      	// Only values that differ from main
	valsA[row][column] = potential(x,y,r,d);
      }
	
  }
}

/*
 *	Writing data to file
 */

ofstream analyticfile;
analyticfile.open("analytical.dat");

for (row = 0; row < matsize; row++)
  {
  for (column = 0; column < matsize; column++)
    {
      analyticfile << row << " " << column << " " 
                   << valsA[column][row] << "\n";
    }
    analyticfile<<"\n";
  }

analyticfile.close();

return 0;

}


/*
 *	FUNCTIONS
 */

float potential(float x, float y, float r,float Plate_separation) {
  float v=0, Plate_Vdiff=2; //Plate_separation=r*pow(10,2);


// E = DV/d (Volage difference of plates(2V) / separation of plates (xmax-xmin) )
  float field = Plate_Vdiff/Plate_separation;
  //calculating the analytical solution
  v=(-1)*field*x*(1-((r*r*r)/(pow((x*x)+(y*y),1.5))));

  return v;
}


//CoordiFy converts the matrix location of a point into its physical coordinate
float cf(float matind,float min,float ds) {
  // matind = index of value in array, min = min true coord value,
  // ds = coord division
  return min + (ds*matind);
}
