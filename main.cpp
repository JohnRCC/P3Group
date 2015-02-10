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

// Function prototypes
float grad(float right,float left,float up,float down,float ds);

// The main flunction
int main(int argc, char* argv[]) {
  
  // Commence timer
  cout << "Commence timer..." << flush;
  double start = timerstart();
  cout << "done." << endl;
  
  //IMAGE MODE:
  //  ./main image.bmp [err_tol][index]
  //
  //ANALYTIC MODE:
  //  ./main [minx][maxx][dx][rad][err_tol][index]
  
  // Check on arguments (needs updating)
  cout << "Checking arguments... " << flush;
  
  if (argc < 4 || argc > 7) {
    cout << "Usage: " << endl;
    cout << "    Analytical: " << argv[0]
	 << " [Min x/y val][Max x/y val][x/y divisions (ds)][Circle Radius]"
	 << "[Error Tolerance][Output type]" << endl;;
    cout << "    Numerical:  " << argv[0]
	 << " [BMP Filename][Error Tolerance][Output Type]"
	 << "[Optional Smoothing]" << endl;
    return 1;
  }
  
  cout << "done." << endl;
  
  // Initialise variables (used in both cases)
  double time = timerstart();
  cout << "Initialising variables... " << flush;
  int row, column, i, rowsize, columnsize, errtol,
    matsize, index, smooth, count, percent;
  float smin, smax, ds, r, mid;
  BMP Image;
  Sublayer** mesh;
  double** output;
  cout << "done. (" << timerend(time) << "s)" << endl;
  
  if (argc == 4 || argc == 5) // Case for numerical solution
    {
      cout << "Bitmap detected. Entering image mode." << endl;
      time = timerstart();
      cout << "Opening bitmap image... " << flush;
  
      // Read in image from bitmap
      Image.ReadFromFile(argv[1]);
      cout << "done. (" << timerend(time) << "s)" << endl;
      
      // Get dimensions
      time = timerstart();
      cout << "Getting image dimensions... " << flush;
      rowsize = Image.TellHeight();
      columnsize = Image.TellWidth();
      
      // Set ds to 1 since not defined by image
      ds = 1;
      cout << "done. (" << timerend(time) << "s)" << endl;
      
      // Get index and error tolerance values
      time = timerstart();
      cout << "Getting index value... " << flush;
      index = strtod(argv[3],NULL);
      errtol = strtod(argv[2],NULL);
      cout << "done. (" << timerend(time) << "s)" << endl;
      
      // Check for smoothing
      time = timerstart();
      cout << "Checking for smoothing... " << flush;
      if (argc==5) {
	smooth = strtod(argv[4],NULL); }
      else {
	smooth = 0; }
      cout << "done. (" << timerend(time) << "s)" << endl;
    }
  
  else if (argc == 7) // Case for the analytical solution
    {
      cout << "Input variables detected. Entering analytic mode." << endl;
      time = timerstart();
      cout << "Defining variables... " << flush;

      // Convert input arguments to variables
      smin = strtod(argv[1],NULL); // Minimum x/y value
      smax = strtod(argv[2],NULL); // Maximum x/y value
      ds = strtod(argv[3],NULL); // x/y divisions
      r = strtod(argv[4],NULL); // Circle radius
      errtol = strtod(argv[5],NULL); // Error tolerance
      index = strtod(argv[6],NULL); // Output type
      
      // Define the size of the matrix
      matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1);
      rowsize = columnsize = matsize;
      
      mid = (matsize/2.0) - (fmod(matsize,2.0));
      cout << "done. (" << timerend(time) << "s)" << endl;
    }
  
  // Define the matrix
  time = timerstart();
  cout << "Defining matrix... " << flush;
  double*** vals = new double**[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      vals[r] = new double*[columnsize];
      for (int s = 0; s < columnsize; s++) {
	vals[r][s] = new double[3];
      }
    }
  cout << "done. (" << timerend(time) << "s)" << endl;

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

  if (argc == 4 || argc == 5) // For the numerical case
    {
      // Generate matrix from image file
      time = timerstart();
      cout << "Filling image matrix... " << flush;
      count = 0;
      percent = rowsize / 100.0;
      for (row = 0; row < rowsize; row++) 
	{
	  for (column = 0; column < columnsize; column++) 
	    {
	      if ( Image(column,row)->Red == 255 &&
		   Image(column,row)->Green == 0 &&
		   Image(column,row)->Blue == 0 ) // Red = +1V
		{
		  vals[row][column][0] = 1;
		  vals[row][column][1] = 1;
		  vals[row][column][2] = 1;
		}
	      else if ( Image(column,row)->Red == 0 &&
			Image(column,row)->Green == 0 &&
			Image(column,row)->Blue == 255 ) // Blue = -1V
		{
		  vals[row][column][0] = -1;
		  vals[row][column][1] = -1;
		  vals[row][column][2] = -1;
		}
	      else if ( Image(column,row)->Red == 0 &&
			Image(column,row)->Green == 0 &&
			Image(column,row)->Blue == 0 ) // Black = 0V
		{
		  vals[row][column][0] = 0;
		  vals[row][column][1] = 0;
		  vals[row][column][2] = 0;
		}
	      else // Not source/sink, free to change
		{
		  vals[row][column][0] = 0;
		  vals[row][column][1] = 0;
		  vals[row][column][2] = 2;
		}
	    }

	  // Display percentage completion
	  if (r > (count*percent))
	    {
	      if (count < 10) {
		cout << count << "%\b\b" << flush; }
	      else {
		cout << count << "%\b\b\b" << flush; }
	      count++;
	    }

	}
      cout << "done. (" << timerend(time) << "s)" << endl;
    }
  
  else if (argc == 7) // For the analytical case
    {  
      // Generate initial matrix from definite values
      time = timerstart();
      cout << "Filling analytic matrix... " << flush;
      for(row = 0; row < matsize; row++)
	{
	  for(column = 0; column < matsize; column++)
	    {
	      if (column == 0) // Left plate
		{
		  vals[row][column][0] =
		    vals[row][column][1] =
		    vals[row][column][2] = 1;
		}
	      else if(column == matsize-1) // Right plate
		{
		  vals[row][column][0] =
		    vals[row][column][1] =
		    vals[row][column][2] = -1;
		}
	      else if ( (pow( (cf(row,smin,ds) - cf(mid,smin,ds)), 2.0 )
			 + pow( (cf(column,smin,ds) - cf(mid,smin,ds)), 2.0 ) )
			< pow(r, 2.0) ) // Within the circle
		{
		  vals[row][column][0] =
		    vals[row][column][1] =
		    vals[row][column][2] = 0;
		}
	      else // not source/sink, free to change
		{
		  vals[row][column][0] = vals[row][column][1] = 0;
		  vals[row][column][2] = 2;
		}	
	    }
	}
      cout << "done. (" << timerend(time) << "s)" << endl;
      
      // Get analytical solution
      analytic(smin,ds,smax,r);
    }
  
  // Open file to write data to
  time = timerstart();
  cout << "Opening datafile... " << flush;
  ofstream datafile;
  cout << "done. (" << timerend(time) << "s)" << endl;
 
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
  
  // Run the algorithm which calculates the potential at each point
  time = timerstart();
  cout << "Running algorithm... " << flush;
  double left, right, up, down;
  count = 0;
  percent = errtol / 100.0;
  
  // Run the algorithm for 'error tolerance' iterations
  for(i = 0; i < errtol; i++) 
    {  
      for(row = 0; row < rowsize; row++)
	{
	  for(column = 0; column < columnsize; column++)
	    {
	      // Define adjacent points (up, down, left, right)
	      if (column != 0 ) {
		left = vals[row][column-1][(i%2)]; }
	      if (column != columnsize-1) {
		right = vals[row][column+1][(i%2)]; }
	      if (row != rowsize-1) {
		up = vals[row+1][column][(i%2)]; }
	      if (row != 0) {
		down = vals[row-1][column][(i%2)]; }
	      
	      // For any possible point, apply propogation
	      if (vals[row][column][2] != 2)
		{
		  vals[row][column][-(i%2)+1] = vals[row][column][2];
		}
	      else if (row == 0 && column == 0)
		{
		  vals[row][column][-(i%2)+1] = (right+up)/2.00;
		}
	      else if (row == 0 && column == columnsize-1)
		{
		  vals[row][column][-(i%2)+1] = (left+up)/2.00;
		}
	      else if (row == rowsize-1 && column == 0)
		{
		  vals[row][column][-(i%2)+1] = (down+right)/2.00;
		}
	      else if (row == rowsize-1 && column == columnsize-1)
		{
		  vals[row][column][-(i%2)+1] = (left+down)/2.00;
		}
	      else if (column == 0)
		{
		  vals[row][column][-(i%2)+1] = (right+down+up)/3.00;
		}
	      else if (column == columnsize-1)
		{
		  vals[row][column][-(i%2)+1] = (left+down+up)/3.00;
		}
	      else if (row == 0)
		{
		  vals[row][column][-(i%2)+1] = (right+up+left)/3.00;
		}
	      else if (row == rowsize-1)
		{
		  vals[row][column][-(i%2)+1] = (right+left+down)/3.00;
		}
	      else
		{
		  vals[row][column][-(i%2)+1] = (up+down+right+left)/4.00;
		}          
	    }
	}
      
      // Display percentage completion
      if (i > (count*percent))
	{
	  if (count < 10 ) {
	    cout << count << "%\b\b" << flush; }
	  else {
	    cout << count << "%\b\b\b" << flush; }
	  count++;
	}
    }
  cout << "done. (" << timerend(time) << "s)" << endl;

  // Ensure the first layer always contains the final matrix
  // Chose first layer rather than zeroth because the matrix will end up in
  // the first layer anyway for even numbers of iterations, which is what we'll
  // normally use
  time = timerstart();
  cout << "Normalising matrix... " << flush;
  if ((-(i%2)+1) == 1 )
    {
      for (row = 0; row < rowsize; row++)
	{
	  for (column = 0; column < columnsize; column++)
	    {
	      vals[row][column][1] = vals[row][column][0];
	    }
	}
    }
  cout << "done. (" << timerend(time) << "s)" << endl;

  // Apply meshing to the numerical solution
  if (argc == 4 || argc == 5)
   {
     time = timerstart();
     cout << "Creating sublayer mesh... " << flush;
     mesh = meshing(vals, rowsize, columnsize, smooth);
     cout << "done. (" << timerend(time) << "s)" << endl;
     cout << "Creating output matrix... " << flush;
     output = printmeshalt(vals, mesh, rowsize, columnsize, 9);
     cout << "done. (" << timerend(time) << "s)" << endl;
   }

  
  datafile.open("mat_test.dat");
  
  time = timerstart();
  cout << "Delivering final output... " << flush;

  // Output results for the analytical case
  if (argc == 7)
    {
      count = 0;
      percent = rowsize / 100.0;
      for (row = 0; row < rowsize; row++)
	{
	  for (column = 0; column < columnsize; column++)
	    {
	      //cout<<"File Iter: "<<row<<"\r";
	      if (index == 1 || index == 3 || index == 5 || index == 7)
		{
		  // Gradient test. See function 'grad' for more info.
		  datafile << cf(row,smin,ds) << " " << cf(column,smin,ds)
			   << " " << vals[row][column][2] << endl;
		}
	      else if (index == 2 || index == 3 || index == 6 || index == 7)
		{
		  //actual values of potential (for plotting etc.)
		  datafile << row << " " << column << " "
			   << vals[row][column][1] << endl;
		}
	      else if (index == 4 || index == 5 || index == 6 || index == 7)
		{
		  // Get fieldlines
		  double **fldmat = new double*[rowsize];
		  for(row = 0; row < rowsize; row++)
		    {
		      fldmat[row] = new double[columnsize];
		    }
		  
		  for(row = 0; row < rowsize; row++)
		    {
		      for(column = 0;column < columnsize; column++)
			{
			  fldmat[row][column] = vals[row][column][1];
			}
		    }
		  
		  // Get fieldline data for completed matrix
		  fldline(rowsize,columnsize,fldmat,ds,ds);
		}
	    }
	  
	  // Display percentage completion
	  if (row > (count*percent))
	    {
	      if (count < 10) {
		cout << count << "%\b\b" << flush; }
	      else {
		cout << count << "%\b\b\b" << flush; }
	      count++;
	    }
	  
	}
    }
  
  // Output results for the numerical case
  else
    {
      count = 0;
      percent = rowsize / 100.0;
      for (row = 0; row < rowsize; row++)
	{
	  for (column = 0; column < columnsize; column++)
	    {
	      //cout<<"File Iter: "<<row<<"\r";
	      if (index == 1 || index == 3 || index == 5 || index == 7)
		{
		  // Gradient test. see function 'grad' for more info.
		  datafile << cf(row,smin,ds) << " " << cf(column,smin,ds)
			   << " " << vals[row][column][2] << endl;
		}
	    }

	  // Display percentage completion
	  if (row > (count*percent))
	    {
	      if (count < 10) {
		cout << count << "%\b\b" << flush; }
	      else {
		cout << count << "%\b\b\b" << flush; }
	      count++;
	    }

	}
      
      int rdim = rowsize * 9;
      int cdim = columnsize * 9;
      count = 0;
      percent = rdim / 100.0;

      for (row = 0; row < rdim; row++)
	{
	for (column = 0; column < cdim; column++)
	  {
	    //cout<<"File Iter: "<<row<<"\r";
	    if (index == 2 || index == 3 || index == 6 || index == 7)
	      {
		// Actual values of potential (for plotting etc.)
		datafile << row << " " << column << " "
			 << output[row][column] << endl;
	      }
	    else if (index == 4 || index == 5 || index == 6 || index == 7)
	      {
		// Get fieldlines
		double **fldmat = new double*[rdim];
		for (row = 0; row < rdim; row++)
		  {
		    fldmat[row] = new double[cdim];
		  }
		
		for (row = 0; row < rdim; row++)
		  {
		    for (column = 0; column < cdim; column++)
		      {
			fldmat[row][column] = output[row][column];
		      }
		  }
		
		// Get fieldline data for completed matrix
		fldline(rdim,cdim,fldmat,ds,ds);
	      }
	  }
	
	// Display percentage completion
	if (row > (count*percent))
	  {
	    if (count < 10) {
	      cout << count << "%\b\b" << flush; }
	    else {
	      cout << count << "%\b\b\b" << flush; }
	    count++;
	  }
	
	}
    }
  
  datafile << "\n";
  datafile.close();
  cout << "done. (" << timerend(time) << "s)" << endl;
    
  timerend(start,1);
  
  return 0;
}


//CoordiFy converts the matrix location of a point into its physical coordinate
float cf(float matind,float min,float ds) {
  // matind = index of value in array, min = min true coord value,
  // ds = coord division
  return min + (ds*matind);
}


// Check on gradient. Returns a value based on 4 surrounding points
// (needs some tweaking in order to calibrate tolerance)
float grad(float right,float left,float up,float down,float ds) {
  float result = pow(pow((right-left)/ds,2.00)+pow((down-up)/ds,2.00),0.50);
  
  /*for now, return result to see values
  if(result>100 || result<0.001){
    return 0;
  }
  else{
    return result;
  }*/

  if (result > 0.25) 
    {
      return 1;
    }
  else
    {
      return 1;
    }
  
}
