#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>

using namespace std;

//function prototype
float potential(float x,float y,float r);
float cf(float matind,float min,float ds);


int main(int argc, char* argv[])
{
float smin=strtod(argv[1],NULL);
float smax=strtod(argv[2],NULL);
float ds=strtod(argv[3],NULL);
float r=strtod(argv[4],NULL);
int iter=strtod(argv[5],NULL);
int index=strtod(argv[6],NULL);
  
//get matrix size
int matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1); 

//initalise matrix
double valsA[matsize][matsize]; 
	      
//generate matrix
for(int row=0;row<matsize;row++) {
  for(int column=0;column<matsize;column++) {
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

datafile.open("Analytic.dat")

for(row=0;row<matsize;row++) {
	for(column=o;column<matsize;column++) {
    //cout<<"File Iter: "<<row<<"\r";
    if(index==0){
      //gradient test. see function 'grad' for more info.
      datafile<<cf(row,smin,ds)<<" "<<cf(column,smin,ds)<<" "<<vals[row][column][2]<<"\n";
    }
    else if(index==1){
      //actual values of potential (for plotting etc.)
      datafile<<row<<" "<<column<<" "<<vals[row][column][((i%2)==0)?0:1]<<"\n";
    }
    else if(index==2){
      //display values in terminal
      cout<<vals[row][column][((i%2)==0)?0:1]<<" ";
    }
  }
  //cout<<"\n";
  datafile<<"\n";
}

  return 0;

}

float potential(float x, float y, float r)
{
  float v=0, Plate_Vdiff=2, Plate_separation=r*pow(10,2);
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

//CoordiFy converts the matrix location of a point into its physical coordinate
float cf(float matind,float min,float ds){
  //matind = index of value in array, min = min true coord value, ds = coord division
  return min + (ds*matind);
}
