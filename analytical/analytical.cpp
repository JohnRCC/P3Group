#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include "../funcs.h"

using namespace std;

//function prototypes
float potential(float x,float y,float r,float Plate_separation);

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

// Initalise matrix
int matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1); 
float mid = (matsize/2.0) - (fmod(matsize,2.0));
double valsA[matsize][matsize]; 

// Useful quantities
//
// 	Plate separation: (system span) - 2(matrix cell size)
float d=(smax-smin)-2*ds;

// Cast values
for(row=0;row<matsize;row++) {
  for(column=0;column<matsize;column++) {
	// Sets "true" coordinates for x and y
    float x=cf(row,smin,ds);
    float y=cf(column,smin,ds);

        if(column==0) {
      valsA[row][column]=valsA[row][column]=1;
      }
      else if(column==matsize-1) {
	valsA[row][column]=valsA[row][column]=-1;
      }
      else if( (pow((cf(row,smin,ds)-cf(mid,smin,ds)),2.0) + pow((cf(column,smin,ds)-cf(mid,smin,ds)),2.0)) < pow(r,2.0) ) {
	valsA[row][column]=valsA[row][column]=0;
      }
      else {
      	// Only values that differ from main
	valsA[row][column]=valsA[row][column]=potential(x,y,r,d);
      }
	
  }
}

/*	PREVIOUS CAST OF MATRIX, CHANGED IT FOR SAME AS MAIN
    float x=row*ds;
    float y=column*ds;
    //setting V inside the sphere to be zero 
    if(pow(((x*x)+(y*y)),0.5) <= r){
	valsA[row][column]=0;
    }
    else {
      valsA[row][column]=potential(x,y,r); 
    }
  }
}

*/


/*
 *	Writing data to file
 */

ofstream analyticfile;
analyticfile.open("analytical.dat");

for(row=0;row<matsize;row++)
{
  for(column=0;column<matsize;column++)
  {
    analyticfile<<row<<" "<<column<<" "<<valsA[row][column]<<"\n";
  }
  analyticfile<<"\n";
}

analyticfile.close();

return 0;

}

float potential(float x, float y, float r,float Plate_separation) {
  float v=0, Plate_Vdiff=2; //Plate_separation=r*pow(10,2);
  //float e0=8.85*pow(10,-12); // permitivity of free space

  // E = sigma/e0
  // float sigma=1;
  // float field=sigma/e0; // electric field between two plates

/* A "good" value for d, the separation of the plates will be calculated, either from the 
	derivation of the analtic solution, or by trying a number of values */

// E = DV/d (Volage difference of plates(2V) / separation of plates (xmax-xmin) )
  float field = Plate_Vdiff/Plate_separation;
  //calculating the analytical solution
  v=(-1)*field*x*(1-((r*r*r)/(pow((x*x)+(y*y),1.5))));

  return v;
}
