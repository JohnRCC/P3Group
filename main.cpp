#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include "./headers/sublayer.h"
#include "./headers/meshing.h"
#include "./headers/funcs.h"
#include "./headers/gradient.h"
#include "./headers/timer.h"
#include "./easybmp/EasyBMP.h"

using namespace std;

// The main flunction
int main(int argc, char* argv[]) {
  
  // Determine if progress should be output to the terminal
  // If silence = 0, progress will be displayed; for any
  // other value, it will be supressed
  int silence = 0;
  if (argc == 6)
    {
      silence = strtod(argv[5],NULL);
    }
  if (argc == 8)
    {
      silence = strtod(argv[7],NULL);
    }
  
  // Commence timer
  if (silence == 0) {
    cout << "Commence timer... " << flush; }
  double start = timerstart();
  if (silence == 0) {
    cout << "done." << endl; }
  
  //generic mode:
  //  ./main [type][args]
  
  // Check on arguments (needs updating)
  // 	EXPLANATION:
  // 		
  // 		Min,Max x/y:	"Real" maximum, minimum coordinates of system
  // 				Entries: float values	
  // 		x/y divisions:	Granularity of Initial grid
  // 				Entries: float value
  // 		Circle Radius:	Radius of GND conducting circle at (0,0)
  // 				Entries: float value
  // 		Error tol:	CURRENT: number of iterations
  // 				Entries: float value
  // 		Output Type:	Index for plot type
  // 				Entries: any sum of 1,2,4:
  // 				 1 - gradient test (now obsolete)
  // 				 2 - potential data
  // 				 4 - fieldlines
  // 				eg. 6 would give potential and fieldlines
  // 				    3 would give gradient test and data 
  // 		Terminal out:	Turn on or off printing program progress
  // 				Entries: 0 (default) -off- or 1 -on-

  if (silence == 0) {
    cout << "Checking arguments... " << flush; }
  
  if (argc < 4 || argc > 8) {
    cout << "Usage: " << endl;
    cout << "    Analytical: " << argv[0]
	 << " [Min x/y val][Max x/y val][x/y divisions (ds)][Circle Radius]"
	 << "[Error Tolerance][Output type][Terminal output]" << endl;;
    cout << "    Numerical:  " << argv[0]
	 << " [BMP Filename][Error Tolerance][Output Type]"
	 << "[Smoothing][Terminal Output]" << endl;
    return 1;
  }
  
  if (silence == 0) {
    cout << "done." << endl; }
  
  // Initialise variables (used in both cases)

  double time = timerstart();
  if (silence == 0) {
    cout << "Initialising variables... " << flush; }
  int row, column, i, rowsize, columnsize, errtol,
    matsize, index, smooth, count;
  float smin, smax, ds, r, mid, percent;
  BMP Image;
  Sublayer** mesh;
  double** output;
  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl; }
  
  if (argc < 7) // Case for numerical solution
    {
      if (silence == 0) {
	cout << "Bitmap detected. Entering image mode." << endl; 
	time = timerstart();
	cout << "Opening bitmap image... " << flush; }
      
      // Read in image from bitmap
      Image.ReadFromFile(argv[1]);
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
      
      // Get dimensions
      if (silence == 0) {
	time = timerstart();
	cout << "Getting image dimensions... " << flush; }
      rowsize = Image.TellHeight();
      columnsize = Image.TellWidth();
      
      // Set ds to 1 since not defined by image
      ds = 1;
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
      
      // Get index and error tolerance values
      if (silence == 0) {
	time = timerstart();
	cout << "Getting index value... " << flush; }
      index = strtod(argv[3],NULL);
      errtol = strtod(argv[2],NULL);
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
      
      // Check for smoothing
      if (silence == 0) {
	time = timerstart();
	cout << "Checking for smoothing... " << flush;}
      if (argc > 4) {
	smooth = strtod(argv[4],NULL); }
      else {
	smooth = 0; }
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
    }
  
  else if (argc > 6) // Case for the analytical solution
    {
      if (silence == 0) {
	cout << "Input variables detected. Solving for system A." << endl;
	time = timerstart();
	cout << "Defining variables... " << flush; }
      
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
      
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
    }
  
  // Define the matrix
  if (silence == 0) {
    time = timerstart();
    cout << "Defining matrix... " << flush; }
  
  double*** vals = new double**[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      vals[r] = new double*[columnsize];
      for (int s = 0; s < columnsize; s++) {
	vals[r][s] = new double[3];
      }
    }
  
  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl; }
  
  if (argc < 7) // For the numerical case
    {
      // Generate matrix from image file
      if (silence == 0) {
	time = timerstart();
	cout << "Filling image matrix... " << flush; }
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
	  if (r > (count*percent) && silence == 0)
	    {
	      if (count < 10) {
		cout << count << "%\b\b" << flush; }
	      else {
		cout << count << "%\b\b\b" << flush; }
	      count++;
	    }
	  
	}
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
    }
  
  else if (argc > 6) // For the analytical case
    {  
      // Generate initial matrix from definite values
      if (silence == 0) {
	time = timerstart();
	cout << "Filling analytic matrix... " << flush; }
      count = 0;
      percent = matsize / 100.0;
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
	  
	  // Display percentage completion
	  if (r > (count*percent) && silence == 0)
	    {
	      if (count < 10) {
		cout << count << "%\b\b" << flush; }
	      else {
		cout << count << "%\b\b\b" << flush; }
	      count++;
	    }
	}
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
      
      // Get analytical solution
      analytic(smin,ds,smax,r);
    }
  
  // Open file to write data to
  if (silence == 0) {
    time = timerstart();
  cout << "Opening datafile... " << flush; }
  ofstream datafile;
  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl; }
  
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
  if (silence == 0) {
    time = timerstart();
    cout << "Running algorithm... " << flush; }
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
      if (i > (count*percent) && silence == 0)
	{
	  if (count < 10 ) {
	    cout << count << "%\b\b" << flush; }
	  else {
	    cout << count << "%\b\b\b" << flush; }
	  count++;
	}
    }
  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl; }
  
  // Ensure the first layer always contains the final matrix
  // Chose first layer rather than zeroth because the matrix will end up in
  // the first layer anyway for even numbers of iterations, which is what we'll
  // normally use
  if (silence == 0) {
    time = timerstart();
    cout << "Normalising matrix layers... " << flush; }
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
  
  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl; }
  
  // Determine the gradient at each point
  if (silence == 0) {
    time = timerstart();
    cout << "Calculating gradients... " << flush; }
 
  vals = getgrad(vals, rowsize, columnsize, ds);
  
  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl; }

  // Apply meshing to the numerical solution
  if (argc < 7)
    {
      if (silence == 0) {
	time = timerstart();
	cout << "Creating sublayer mesh... " << flush; }
      mesh = meshing(vals, rowsize, columnsize, smooth);
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl;
	time = timerstart();
	cout << "Creating output matrix... " << flush; }
      output = printmeshalt(vals, mesh, rowsize, columnsize, 9);
      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
    }

  // A high-res matrix with values taken entriely from the top-level matrix
  // without any sublayers
  double** comparison = nomeshing(vals, rowsize, columnsize, 9);

  // Output results for the analytical case
  if (argc > 6)
    {
      
      // Print gradient data    	
      if (index == 1 || index == 3 || index == 5 || index == 7)
	{
	  if (silence == 0) {
	    time = timerstart();
	    cout << "Printing gradient to file... " << flush;
	  }

	  datafile.open("grad.dat");
	  //datafile << "Gradient" << endl << endl;
	  count = 0;
	  percent = rowsize / 100.0;

	  for (row = 0; row < rowsize; row++)
	    {
	      for (column = 0; column < columnsize; column++)
		{
		  // Gradient test. See function 'grad' for more info.
		  datafile << cf(row,smin,ds) << " " << cf(column,smin,ds)
			   << " " << vals[row][column][2] << endl;
		}

	      datafile << endl;
	      
	      // Display percentage completion
	      if (row > (count*percent) && silence == 0)
		{
		  if (count < 10) {
		    cout << count << "%\b\b" << flush; }
		  else {
		    cout << count << "%\b\b\b" << flush; }
		  count++;
		}
	    }

	  datafile << endl;
	  datafile.close();
	  
	  if (silence == 0) {
	    cout << "done. (" << timerend(time) << "s)" << endl; }
	}
      
      // Print potential data		
      if (index == 2 || index == 3 || index == 6 || index == 7)
	{
	  if (silence == 0) {
	    time = timerstart();
	    cout << "Printing potential to file... " << flush;
	  }
	  
	  datafile.open("pot.dat");
	  //datafile << "Potential" << endl << endl;	
	  count = 0;
	  percent = rowsize / 100.0;

	  for (row = 0; row < rowsize; row++)
	    {
	      for (column = 0; column < columnsize; column++)
		{	
		  //actual values of potential (for plotting etc.)
		  datafile << row << " " << column << " "
			   << vals[row][column][1] << endl;
		}

	      datafile << endl;
	      
	      // Display percentage completion
	      if (row > (count*percent) && silence == 0)

		{
		  if (count < 10) {
		    cout << count << "%\b\b" << flush; }
		  else {
		    cout << count << "%\b\b\b" << flush; }
		  count++;
		}
	    }

	  datafile << endl;
	  datafile.close();
	  
	  if (silence == 0) {
	    cout << "done. (" << timerend(time) << "s)" << endl; }
	}
      
      // Get field line data		
      if (index == 4 || index == 5 || index == 6 || index == 7)
	{
	  if (silence == 0) {
	    time = timerstart();
	    cout << "Printing field lines to file... " << flush; }

	  //datafile << "Field lines" << endl << endl;		
	  count = 0;
	  percent = rowsize / 100.0;

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

	      // Display percentage completion
	      if (row > (count*percent) && silence == 0)
		{
		  if (count < 10) {
		    cout << count << "%\b\b" << flush; }
		  else {
		    cout << count << "%\b\b\b" << flush; }
		  count++;
		}
	    }
	  
	  // Get fieldline data for completed matrix
	  fldline(rowsize,columnsize,fldmat,ds,ds);
	  
	if (silence == 0) {
	  cout << "done. (" << timerend(time) << "s)" << endl; }
	}
    }
  
  // Output results for the numerical case
  else
    { 

      // Print gradient to file
      if (index == 1 || index == 3 || index == 5 || index == 7)
	{
	  if (silence == 0) {
	    time = timerstart();
	    cout << "Printing gradient to file... " << flush;
	  }

	  datafile.open("grad.dat");
	  //datafile << "Gradient" << endl << endl;	
	  count = 0;
	  percent = rowsize / 100.0;

	  for (row = 0; row < rowsize; row++)
	    {
	      for (column = 0; column < columnsize; column++)
		{
		  // Gradient test. see function 'grad' for more info.
		  datafile << cf(row,smin,ds) << " " << cf(column,smin,ds)
			   << " " << vals[row][column][2] << endl; 
		}
	      
	      datafile << endl;
	      
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

	  datafile << endl;
	  datafile.close();
	  
	  if (silence == 0) {
	    cout << "done. (" << timerend(time) << "s)" << endl; }
	}
      
      // Print potential to file	
      if (index == 2 || index == 3 || index == 6 || index == 7)
	{ 
	  if (silence == 0) {
	    time = timerstart();
	    cout << "Printing potential to file... " << flush;
	  }

	  datafile.open("pot.dat");
	  //datafile << "Potential" << endl << endl;
	  int rdim = rowsize * 9;
	  int cdim = columnsize * 9;
	  count = 0;
	  percent = rdim / 100.0;
	  
	  for (row = 0; row < rdim; row++)
	    {
	      for (column = 0; column < cdim; column++)
		{
		  // Actual values of potential (for plotting etc.)
		  datafile << row << " " << column << " "
		  	   << output[row][column] << endl;
		}
	      datafile << endl;
	      
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

	  datafile << endl;
	  datafile.close();
	  
	  if (silence == 0) {
	    cout << "done. (" << timerend(time) << "s)" << endl; }
	}
      
      // Get field line data		  
      if (index == 4 || index == 5 || index == 6 || index == 7)
	{
	  // Get fieldlines
	  if (silence == 0) {
	    time = timerstart();
	    cout << "Printing field lines to file... " << flush;
	  }

	  //datafile << "Field lines" << endl << endl;
	  double **fldmat = new double*[rowsize];
	  for (int y = 0; y < rowsize; y++)
	    {
	      fldmat[y] = new double[columnsize];
	    }
	  
	  for (row = 0; row < rowsize; row++)
	    {
	      for (column = 0; column < columnsize; column++)
		{
		  fldmat[row][column] = vals[row][column][1];
		}

	      // Display percentage completion
	      if (row > (count*percent) && silence == 0)
		{
		  if (count < 10) {
		    cout << count << "%\b\b" << flush; }
	      		else {
			  cout << count << "%\b\b\b" << flush; }
		  count++;
		}
	      
	      // Get fieldline data for completed matrix
	      fldline(rowsize,columnsize,fldmat,ds,ds);
	    } 
	  
	  if (silence == 0) {
	    cout << "done. (" << timerend(time) << "s)" << endl; }
	}
    }
  
  if (silence == 0) {
    timerend(start,1); }
  
  return 0;
}
