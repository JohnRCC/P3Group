#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>

using namespace std;

//function prototypes
float potential(float x,float y,float r,float Plate_separation);
float cf(float matind,float min,float ds); 


int main(int argc, char* argv[])
{
	
/*
 * 	Verifying and casting correct amount of commandline arguments
 */	
if(argc<6) {
  cout<<"Usage: "<<argv[0]<<" [Min x/y val][Max x/y val][x/y divisions (ds)][Circle Radius][No. Iterations][Output type]\n";
return 1;
}

float smin=strtod(argv[1],NULL);
float smax=strtod(argv[2],NULL);
float ds=strtod(argv[3],NULL);
float r=strtod(argv[4],NULL);
int iter=strtod(argv[5],NULL);
int index=strtod(argv[6],NULL);
  
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
float d = pow(r,2);
int matsize = d/ds - (fmod(d,ds));
 smin = -d/2.0;
 smax = d/2.0;

// Initalise matrix
	
	// Matrix size in terms of min and max values
	/*
	 * float d = abs(smax-smin) - 2*ds;
	 * int matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1); 
	 *
	 */
	 

float mid = (matsize/2.0) - (fmod(matsize,2.0));
double valsA[matsize][matsize]; 


// Cast values
for(row=0;row<matsize;row++) {
  for(column=0;column<matsize;column++) {
	// Sets "true" cordinets for x and y
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

/*
 *	Writing data to file
 */

ofstream datafile;
datafile.open("analytical.dat");

for(row=0;row<matsize;row++) {
	for(column=0;column<matsize;column++) {
    //cout<<"File Iter: "<<row<<"\r";
    if(index==0){
      //gradient test. see function 'grad' for more info.
      datafile<<cf(row,smin,ds)<<" "<<cf(column,smin,ds)<<" "<<valsA[row][column]<<"\n";
    }
    else if(index==1){
      //actual values of potential (for plotting etc.)
      datafile<<row<<" "<<column<<" "<<valsA[row][column]<<"\n";
    }
    else if(index==2){
      //display values in terminal
      cout<<valsA[row][column]<<" ";
    }
  }
  //cout<<"\n";
  datafile<<"\n";
}

datafile.close();

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
float cf(float matind,float min,float ds){
  //matind = index of value in array, min = min true coord value, ds = coord division
  return min + (ds*matind);
}
