#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include "sublayer.h"
#include "funcs.h"
#include "timer.h"
#include "meshing.h"
//#include "./fieldline/fieldline.h"


//Function prototypes
float cf(float matind,float min,float ds);
float grad(float right,float left,float up,float down,float ds);


int main(int argc, char* argv[]) {

  using namespace std;

  if(argc<6) {
    cout<<"Usage: "<< argv[0]
	<< " [Min x/y val][Max x/y val][x/y divisions (ds)]"
	<< "[Circle Radius][No. Iterations][Output type]\n";
    return 1;
  }

 double start = timerstart();

 float smin=strtod(argv[1],NULL);
 float smax=strtod(argv[2],NULL);
 float ds=strtod(argv[3],NULL);
 float r=strtod(argv[4],NULL);
 int iter=strtod(argv[5],NULL);
 int index=strtod(argv[6],NULL);
 
 // Get matrix size
 int matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1);  
 
 // Get mindpoint of matrix for circle
 float mid = (matsize/2.0) - (fmod(matsize,2.0));

 // Create matrix to store values for potential and gradient
 // Contains three layers: two for iteration, one for gradient
 double*** vals = new double**[matsize];
 for (int r = 0; r < matsize; r++)
   {
     vals[r] = new double*[matsize];
     for (int s = 0; s < matsize; s++) {
       vals[r][s] = new double[3];
     }
   }
  
 double gen = timerstart();
 // Generate initial matrix
 int row,column,i;
 for(row=0;row<matsize;row++) {
   for(column=0;column<matsize;column++) {
     //cout << "Generating row " << row << ", column " << column << "\r";
     if(column==0) {
       vals[row][column][0]=vals[row][column][1]=1;
     }
     else if(column==matsize-1) {
       vals[row][column][0]=vals[row][column][1]=-1;
     }
     else if( (pow((cf(row,smin,ds)-cf(mid,smin,ds)),2.0) + pow((cf(column,smin,ds)-cf(mid,smin,ds)),2.0)) < pow(r,2.0) ) {
       vals[row][column][0]=vals[row][column][1]=0;
      }
      else {
	vals[row][column][0]=vals[row][column][1]=0;
      }
	
  }
}

 cout << endl;
 cout << "Time to generate matrix: " << timerend(gen) << "s" << endl;

//open file to write data to
ofstream datafile;

//define position values for ease of use
float up,down,left,right;

 double alg = timerstart();
//run algorithm to get average of values
for(i=0;i<iter;i++) {
  for(row=1;row<matsize-1;row++) {
    for(column=1;column<matsize-1;column++) {
      //get position values for step
      up    = vals[row-1][column][i%2];
      down  = vals[row+1][column][i%2];
      left  = vals[row][column-1][i%2];
      right = vals[row][column+1][i%2];
      //do check on gradient to see how it's changing
      //if(row>0 && row<matsize-1){
	//vals[row][column][2] = grad(right,left,up,down,ds);
      //}
      if(row==0) {
	vals[row][column][-(i%2)+1] = (left+right+down)/3.00;
	vals[row][column][2] = grad(right,left,0,down,ds);
      }
      else if(row==0 || row==matsize-1) {
	vals[row][column][-(i%2)+1] = (left+right+up)/3.00;
	vals[row][column][2] = grad(right,left,up,0,ds);
      }
      else if( (pow((cf(row,smin,ds)-cf(mid,smin,ds)),2.0) + pow((cf(column,smin,ds)-cf(mid,smin,ds)),2.0)) < pow(r,2.0) ) {
	vals[row][column][-(i%2)+1]=0;
	vals[row][column][2] = grad(right,left,up,down,ds);
      }
      else {
	vals[row][column][-(i%2)+1] = 0.25*(left+right+up+down);
	vals[row][column][2] = grad(right,left,up,down,ds);
      }		
    }
  }
  //cout << "Iteration " << i << "\r";
}

 cout << endl;
 cout << "Time to run algorithm: " << timerend(alg) << "s" << endl;

// Ensure the first layer always contains the final matrix
// Chose first layer rather than zeroth because the matrix will end up in
// the first layer anyway for even numbers of iterations, which is what we'll
// normally use
 if ((-(i%2)+1) == 1 ) {
   for(row=0; row<matsize; row++) {
     for(column=0; column<matsize; column++) {
       vals[row][column][1] = vals[row][column][0];
     }
   }
 }

 double mes = timerstart();
 // Run the meshing function and return the array of sublayers
 Sublayer** mesh = meshing(vals, matsize);
 cout << "Time to run meshing function: " << timerend(mes) << "s" << endl;

 // Show the mesh works
 int sublayers = 0;
 for (int r = 0; r < matsize; r++)
   {
     for (int c = 0; c < matsize; c++)
       {
	 if ( mesh[r][c].index == 0 )
	   {
	     sublayers++;
	   }
       }
   }

double prin = timerstart();
double** output = printmeshalt(vals, mesh, matsize, 9);
 int dim = matsize * 9;
cout << "Time to print matrix: " << timerend(prin) << "s" << endl;


// Converting every value from 3d matrix to 2d one
// This is stupid and inefficient but if you can manage to pass one layer
// of the matrix to the fldline function then be my guest
double **fldmat = new double*[dim];

for(row=0;row<dim;row++)
{
  fldmat[row] = new double[dim];
}

for(row=0;row<dim;row++)
{
  for(column=0;column<dim;column++)
  {
    fldmat[row][column]=output[row][column];
  }
}


// get fieldline data for completed matrix
/*fldline(matsize,matsize,fldmat,ds,ds);
  ofstream field;
  field.open("field.dat");
  //Output coords and vals
  for(r=1;r<matsize-1;r++)
  {
    for(int c=1;c<matsize-1;c++)
    {
      Fieldline Test = *fline(r,c,fldmat,ds,ds);
      field<<Test.xcord<<" "<<Test.ycord<<" "<<Test.fieldvals[0]<<" "<<Test.fieldvals[1]<<"\n";
    }
    field<<"\n";
  }
  field.close();*/

double dat = timerstart();
  
datafile.open("the_datafile.dat");

for(row=0;row<dim;row++) {
  for(column=0;column<dim;column++) {
    //cout<<"File Iter: "<<row<<"\r";
    if(index==0){
      //gradient test. see function 'grad' for more info.
      datafile<<cf(row,smin,ds)<<" "<<cf(column,smin,ds)<<" "<<vals[row][column][2]<<"\n";
    }
    else if(index==1){
      //actual values of potential (for plotting etc.)
      datafile<<row<<" "<<column<<" "<<output[row][column]<<"\n";
    }
    else if(index==2){
      //display values in terminal
      cout<<output[row][column]<<" ";
    }
  }
  datafile<<"\n";
}

if (index == 2) {
  cout << endl; }

datafile.close(); 

cout << "Time to write matrix to file: " << timerend(dat) << "s" << endl;

cout << "Total sublayers: " << sublayers << endl;
timerend(start, 1);

return 0;

}

//CoordiFy converts the matrix location of a point into its physical coordinate
float cf(float matind,float min,float ds){
  //matind = index of value in array, min = min true coord value, ds = coord division
  return min + (ds*matind);
}

//Check on gradient. Returns a value based on 4 surrounding points (needs some tweaking in order to calibrate tolerance)
float grad(float right,float left,float up,float down,float ds){
  float result = pow(pow((right-left)/ds,2.00)+pow((down-up)/ds,2.00),0.50);
  
  /*for now, return result to see values
  if(result>100 || result<0.001){
    return 0;
  }
  else{
    return result;
  }*/

  if (result>0.25){
      return 1;
    }
    else{
      return 1;
    }
  
}
