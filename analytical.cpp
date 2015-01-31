#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>

using namespace std;

//function prototype
float potential(float x,float y,float r);

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



  return 0;

}

float potential(float x, float y, float r)
{
  float v=0;
  float e0=8.85*pow(10,-12); // permitivity of free space

  /*I am not sure how to deal with the sigma, the charge density. The electric
 field of an infinite plate is independent of how far away we are, so it should  be a constant/factor in the equation. Can we just make it equal to 1?
 ASK LARS */

  float sigma=1;
  float field=sigma/e0; // electric field between two plates

  //calculating the analytical solution
  v=(-1)*field*x*(1-((r*r*r)/(pow((x*x)+(y*y),1.5))));

  return v;
}
