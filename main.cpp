#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <vector>
#include "sublayer.h"
#include "meshing.h"
#include "funcs.h"
#include "timer.h"
#include "./easybmp/EasyBMP.h"

using namespace std;

//function prototypes
float grad(float right,float left,float up,float down,float ds);

int main(int argc, char* argv[]) {
//commence timer
cout<<"Commence timer..."<<endl;
double start = timerstart();

//IMAGE MODE:
//  ./main image.bmp [err_tol][index]
//
//ANALYTIC MODE:
//  ./main [minx][maxx][dx][rad][err_tol][index]

//Check on arguments (needs updating)
cout<<"Checking arguments...";
if(argc<4 || argc>7) {
  cout<<"Usage: "<<endl;
  cout<<"    Analytical: "<<argv[0]<<" [Min x/y val][Max x/y val][x/y divisions (ds)][Circle Radius][Error Tolerance][Output type]\n";
  cout<<"    Numerical:  "<<argv[0]<<" [BMP Filename][Error Tolerance][Output Type][Optional Smoothing]"<<endl;
return 1;
}
cout<<"Done."<<endl;
//Initialise variables (used in both cases)
cout<<"Initialising variables... ";
 int row,column,i,rowsize,columnsize,errtol,matsize,index,smooth;
float smin,smax,ds,r,mid;
BMP Image;
cout<<"Done"<<endl;

if(argc==4 || argc==5) //case for numerical solution
{
  cout<<"Bitmap detected. Entering image mode..."<<endl;
  cout<<"Opening bitmap image... ";
  //Read in image from bitmap
  Image.ReadFromFile(argv[1]);
  cout<<"Done."<<endl;

  //Get dimensions
  cout<<"Getting image dimensions... ";
  rowsize = Image.TellHeight();
  columnsize = Image.TellWidth();
  
  //setting ds to 1 since not defined by image
  ds = 1;
  cout<<"Done."<<endl;

  //Get index val
  cout<<"Getting index value... ";
  index=strtod(argv[3],NULL);
  errtol=strtod(argv[2],NULL);
  cout<<"Done."<<endl;

  // Check for smoothing
  if (argc==5) {
    smooth = strtod(argv[4],NULL); }
  else {
    smooth = 0; }
}

else if(argc==7)
{
  cout<<"Input variables detected. Entering analytic mode..."<<endl;
  smin=strtod(argv[1],NULL);
  smax=strtod(argv[2],NULL);
  ds=strtod(argv[3],NULL);
  r=strtod(argv[4],NULL);
  errtol=strtod(argv[5],NULL);
  index=strtod(argv[6],NULL);

  matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1);
  rowsize=columnsize=matsize;

  mid = (matsize/2.0) - (fmod(matsize,2.0));
}

//Define matrix
  cout<<"Defining matrix... ";
  double*** vals = new double**[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      vals[r] = new double*[columnsize];
      for (int s = 0; s < columnsize; s++) {
	vals[r][s] = new double[3];
      }
    }
  

  /*vector<vector<vector<double> > > vals;

  vals.resize(rowsize);
  for(row=0;row<rowsize;row++)
  {
    vals[row].resize(columnsize);
    for(column=0;column<columnsize;column++)
    {
      vals[row][column].resize(3);
    }
  }*/

  cout<<"Done."<<endl;

if(argc==4 || argc==5)
{
  //Generate matrix from image file
  cout<<"Filling image matrix... ";
  for (row=0; row<rowsize; row++) 
  {
    for (column=0; column<columnsize; column++) 
    {
      if (Image(column,row)->Red == 255 && Image(column,row)->Green == 0 && Image(column,row)->Blue == 0) // Red = +1V
      {
	vals[row][column][0] = 1;
        vals[row][column][1] = 1;
        vals[row][column][2] = 1;
      }
      else if (Image(column,row)->Red == 0 && Image(column,row)->Green == 0 && Image(column,row)->Blue == 255) // Blue = -1V
      {
	vals[row][column][0] = -1;
        vals[row][column][1] = -1;
        vals[row][column][2] = -1;
      }
      else if (Image(column,row)->Red == 0 && Image(column,row)->Green == 0 && Image(column,row)->Blue == 0) // Black = 0V
      {
        vals[row][column][0] = 0;
        vals[row][column][1] = 0;
        vals[row][column][2] = 0;
      }
      else // not source/sink, free to change
      {
        vals[row][column][0] = 0;
        vals[row][column][1] = 0;
	vals[row][column][2] = 2;
      }
    }
  }
  cout<<"Done."<<endl;
}

else if(argc==7) // case for analytical solution
{  
    cout<<"Filling analytic matrix... ";
  //generate initial matrix from definite values
  for(row=0;row<matsize;row++)
  {
    for(column=0;column<matsize;column++)
    {
      if(column==0) //left plate
      {
	vals[row][column][0]=vals[row][column][1]=vals[row][column][2]=1;
      }
      else if(column==matsize-1) //right plate
      {
	vals[row][column][0]=vals[row][column][1]=vals[row][column][2]=-1;
      }
      else if( (pow((cf(row,smin,ds)-cf(mid,smin,ds)),2.0) + pow((cf(column,smin,ds)-cf(mid,smin,ds)),2.0)) < pow(r,2.0) ) // within circle
      {
	vals[row][column][0]=vals[row][column][1]=vals[row][column][2]=0;
      }
      else // not source/sink, free to change
      {
	vals[row][column][0]=vals[row][column][1]=0;
	vals[row][column][2]=2;
      }	
    }
  }
  cout<<"Done"<<endl;

  //Get analytical solution
  analytic(smin,ds,smax,r);
}

//open file to write data to
ofstream datafile;

//DEBUG: output initial matrix to file
/*cout<<"Attempting to print matrix to file for testing... ";
datafile.open("mat_test.dat");
for(row=0;row<rowsize;row++)
{
  for(column=0;column<columnsize;column++)
  {
    datafile<<row<<" "<<column<<" "<<vals[row][column][1]<<endl;
  }
  datafile<<endl;
}
cout<<"Done."<<endl;*/
cout<<"Running algorithm... ";
double left, right, up, down;
for(i=0;i<errtol;i++) 
{  
  for(row=0;row<rowsize;row++)
  {
    for(column=0;column<columnsize;column++)
    {
      left = vals[row][column-1][(i%2)];
      right = vals[row][column+1][(i%2)];
      up = vals[row+1][column][(i%2)];
      down = vals[row-1][column][(i%2)];

      if(vals[row][column][2] != 2)
      {
        vals[row][column][-(i%2)+1] = vals[row][column][2];
      }
      else if(row==0 && column==0)
      {
        vals[row][column][-(i%2)+1] = (right+up)/2.00;
      }
      else if(row==0 && column==columnsize-1)
      {
        vals[row][column][-(i%2)+1] = (left+up)/2.00;
      }
      else if(row==rowsize-1 && column==0)
      {
        vals[row][column][-(i%2)+1] = (down+right)/2.00;
      }
      else if(row==rowsize-1 && column==columnsize-1)
      {
        vals[row][column][-(i%2)+1] = (left+down)/2.00;
      }
      else if(column==0)
      {
        vals[row][column][-(i%2)+1] = (right+down+up)/3.00;
      }
      else if(column==columnsize-1)
      {
	vals[row][column][-(i%2)+1] = (left+down+up)/3.00;
      }
      else if(row==0)
      {
	vals[row][column][-(i%2)+1] = (right+up+left)/3.00;
      }
      else if(row==rowsize-1)
      {
	vals[row][column][-(i%2)+1] = (right+left+down)/3.00;
      }
      else
      {
	vals[row][column][-(i%2)+1] = (up+down+right+left)/4.00;
      }          
    }
  }
}
cout<<"Done."<<endl;

// Ensure the first layer always contains the final matrix
// Chose first layer rather than zeroth because the matrix will end up in
// the first layer anyway for even numbers of iterations, which is what we'll
// normally use
 if ((-(i%2)+1) == 1 ) {
   for(row=0; row<rowsize; row++) {
     for(column=0; column<columnsize; column++) {
       vals[row][column][1] = vals[row][column][0];
     }
   }
 }

 // Apply meshing to the numerical solution
 Sublayer** mesh;
 double** output;
 if ( argc == 4 )
   {
     mesh = meshing(vals, rowsize, columnsize, smooth);
     output = printmeshalt(vals, mesh, rowsize, columnsize, 9);
   }

datafile.open("mat_test.dat");

if (argc==7) {
  for(row=0;row<rowsize;row++) {
    for(column=0;column<columnsize;column++) {
      //cout<<"File Iter: "<<row<<"\r";
      if(index==1||index==3||index==5||index==7){
	//gradient test. see function 'grad' for more info.
	datafile<<cf(row,smin,ds)<<" "<<cf(column,smin,ds)<<" "<<vals[row][column][2]<<"\n";
      }
      else if(index==2||index==3||index==6||index==7){
	//actual values of potential (for plotting etc.)
	datafile<<row<<" "<<column<<" "<<vals[row][column][((i%2)==0)?0:1]<<"\n";
      }
      else if(index==4||index==5||index==6||index==7){
	//get fieldlines
	double **fldmat = new double*[rowsize];
	
	for(row=0;row<rowsize;row++)
	  {
	    fldmat[row] = new double[columnsize];
	  }
	
	for(row=0;row<rowsize;row++)
	  {
	    for(column=0;column<columnsize;column++)
	      {
		fldmat[row][column]=vals[row][column][-(i%2)+1];
	      }
	  }
	
	// get fieldline data for completed matrix
	fldline(rowsize,columnsize,fldmat,ds,ds);
      }
    }
  }
 }

 else {
   for(row=0;row<rowsize;row++) {
     for(column=0;column<columnsize;column++) {
       //cout<<"File Iter: "<<row<<"\r";
       if(index==1||index==3||index==5||index==7){
	 //gradient test. see function 'grad' for more info.
	 datafile<<cf(row,smin,ds)<<" "<<cf(column,smin,ds)<<" "<<vals[row][column][2]<<"\n";
       }
     }
   }

   int rdim = rowsize * 9;
   int cdim = columnsize * 9;
   
   for(row=0;row<rdim;row++) {
     for(column=0;column<cdim;column++) {
       //cout<<"File Iter: "<<row<<"\r";
       if(index==2||index==3||index==6||index==7){
	 //actual values of potential (for plotting etc.)
	 datafile << row << " " << column << " " << output[row][column] << "\n";
       }
       else if(index==4||index==5||index==6||index==7){
	 //get fieldlines
	 double **fldmat = new double*[rdim];
	 
	 for(row=0;row<rowsize;row++)
	   {
	     fldmat[row] = new double[cdim];
	   }
	 
	 for(row=0;row<rdim;row++)
	   {
	     for(column=0;column<cdim;column++)
	       {
		 fldmat[row][column] = output[row][column];
	       }
	   }
	 
	 // get fieldline data for completed matrix
	 fldline(rdim,cdim,fldmat,ds,ds);
       }
     }
   }
 }

//cout<<"\n";
datafile<<"\n";

 
datafile.close(); 


timerend(start,1);

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
