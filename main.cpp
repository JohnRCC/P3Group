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
  if (argc == 8 || argc == 9)
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
  // 		Error tol:	<100: number of iterations
  // 				>100: percentage accuracy eg 5 = max. 5% change
  // 		Output Type:	Index for plot type
  // 				Entries: any sum of 1,2,4:
  // 				 1 - gradient test (now obsolete)
  // 				 2 - potential data
  // 				 4 - fieldlines
  // 				eg. 6 would give potential and fieldlines
  // 				    3 would give gradient test and data 
  //            Meshing:        The power of three for the finest mesh grid,
  //                            e.g. if maxpower = 2, the largest submatrix
  //                            will be 9x9 (2^3).
  //                            Set to zero to disable meshing.
  // 		Terminal out:	Turn on or off printing program progress
  // 				Entries: 0 (default) -off- or 1 -on-

  if (silence == 0) {
    cout << "Checking arguments... " << flush; }
  
  if (argc < 4 || argc > 9) {
    cout << "Usage: " << endl;
    cout << "    Analytical: " << argv[0]
	 << " [Min x/y val][Max x/y val][x/y divisions (ds)][Circle Radius]"
	 << "[Error Tolerance][Output type][Terminal output][Meshing]" << endl;;
    cout << "    Numerical:  " << argv[0]
	 << " [BMP Filename][Error Tolerance][Output Type]"
	 << "[Meshing][Terminal Output]" << endl;
    return 1;
  }
  
  if (silence == 0) {
    cout << "done." << endl; }
  
  // Initialise variables (used in both cases)

  double time = timerstart();
  if (silence == 0) {
    cout << "Initialising variables... " << flush; }

  int row, column, i, rowsize, columnsize, matsize,
    index, smooth, count, rdim, cdim, maxpower, maxres;
  float smin, smax, ds, r, mid, percent;
  BMP Image;
  Sublayer** mesh;
  double*** output;
  double errtol, runtime, cputime;

  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl; }
  
  // Declare variables specific to the numerical case
  if (argc < 7)
    {
      if (silence == 0) {
	cout << "Bitmap detected. Entering image mode." << endl; 
	time = timerstart();
	cout << "Defining conditions... " << flush; }
      
      // Read in image from bitmap
      Image.ReadFromFile(argv[1]);
      
      // Get dimensions
      rowsize = Image.TellHeight();
      columnsize = Image.TellWidth();
      
      // Set ds to 1 since not defined by image
      ds = 1;
      
      // Get index and error tolerance values
      index = strtod(argv[3],NULL);
      errtol = strtod(argv[2],NULL);
      
      // Check for smoothing
      // Currently disabled due to shortage of input arguments
      /*
      if (argc > 4) {
	smooth = strtod(argv[4],NULL); }
      else {
	smooth = 0; }
      */
      smooth = 0;

      // Check for meshing
      if (argc > 4) {
	maxpower = strtod(argv[4],NULL); }
      else {
	maxpower = 2; }

      // Work out resultant meshing variables
      maxres = 1;
      for (i = 0; i < maxpower; i++) {
	maxres = maxres * 3; }
      rdim = rowsize * maxres;
      cdim = columnsize * maxres;

      if (silence == 0) {
	cout << "done. (" << timerend(time) << "s)" << endl; }
    }
  
  // Declare variables specific to the analytical case
  else if (argc > 6)
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
      if (argc == 9) {
	maxpower = strtod(argv[8],NULL); } // Meshing power
      else {
	maxpower = 2; }
      
      // Define the size of the matrix
      matsize = (((float)smax-smin)/ds) - fmod((smax-smin)/ds,1);
      rowsize = columnsize = matsize;
      mid = (matsize/2.0);// - (fmod(matsize,2.0));
      maxres = 1;
      for (i = 0; i < maxpower; i++) {
	maxres = maxres * 3; }
      rdim = rowsize * maxres;
      cdim = columnsize * maxres;
      
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
	vals[r][s] = new double[4];
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
	  if (row > (count*percent) && silence == 0)
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
	      else if ( (pow( (cf(row,smin,ds,1) - cf(mid,smin,ds,1)), 2.0 )
			 + pow( (cf(column,smin,ds,1) - cf(mid,smin,ds,1)), 2.0 ) )
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
      analytic(smin,ds,smax,r,maxres,silence);
    }
  
  // Run the algorithms which calculate the potential at each point
  //
  int algType = 9;

  //
  // FIVE_POINT DIFFERENCE METHOD ALGORITHM
  //

if (algType == 5) {
  if (silence == 0) {
    time = timerstart();
    cout << "Running five-point difference method algorithm... " << flush; }
  
  i = algFivePointDM(vals,columnsize,rowsize,errtol,silence);

  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s, "
	 << i-1 << " iterations)" << endl; }
}
  //
  //NINE-POINT DIFFERENCE METHOD ALGORITHM
  //
if (algType == 9){
  if (silence == 0) {
    time = timerstart();
    cout << "Running nine-point difference method algorithm... " << flush; }
  
  i = algNinePointDM(vals,columnsize,rowsize,errtol,silence);

  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s, "
	 << i-1 << " iterations)" << endl; }
}
  // Determine the gradient at each point
  if (silence == 0) {
    time = timerstart();
    cout << "Calculating gradients... " << flush; }
 
  getgrad(vals, rowsize, columnsize, ds);
  
  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl; }

  // Create a matrix of sublayers
  // Use meshtype = 0 for gradient-dependent meshing and
  // meshtype = 1 for second-derivative based meshing
  int meshtype = 0;
  if (meshtype == 0) {
    mesh = meshing(vals, rowsize, columnsize, maxpower, smooth, silence); }
  else if (meshtype == 1) {
    mesh =
      secondmeshing(vals, rowsize, columnsize, maxpower, smooth, silence); }
  
  if (silence == 0) {
    time = timerstart();
    cout << "Creating output matrix... " << flush; }
  
  // Create an output matrix which includes the sublayers
  output = printmesh(vals, mesh, rowsize, columnsize, maxres, silence);
  
  if (silence == 0) {
    cout << "done. (" << timerend(time) << "s)" << endl;
    time = timerstart();
    cout << "Refining output matrix... " << flush; }
  
  // Refine the output matrix (if meshing was used)
  if (maxres > 0)
    {
      if (algType == 5) {
	refine5point(output, rdim, cdim, maxres, silence); }
      else if (algType == 9) {
	refine9point(output, rdim, cdim, maxres, silence); }
    }
  
  if (silence == 0) {
    cout << "done (" << timerend(time) << "s)." << endl; }
  
  // Create a high-res matrix with values taken entriely from the top-level
  // matrix without any sublayers
  // double** comparison = nomeshing(vals, rowsize, columnsize, maxpower);

  // Open datafile to output results to
  ofstream datafile;

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
		  datafile << cf(row,smin,ds,1) << " " << cf(column,smin,ds,1)
			   << " " << vals[row][column][2] << "\n";
		}

	      datafile << "\n";
	      
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
	  percent = rdim / 100.0;

	  for (row = 0; row < rdim; row++)
	    {
	      for (column = 0; column < cdim; column++)
		{	
		  //actual values of potential (for plotting etc.)
		  datafile << row << " " << column << " "
			   << output[row][column][0] << "\n";
		}

	      datafile << "\n";
	      
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
		  datafile << cf(row,smin,ds,1) << " " << cf(column,smin,ds,1)
			   << " " << vals[row][column][2] << "\n"; 
		}
	      
	      datafile << "\n";
	      
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
      
      // Print potential to file	
      if (index == 2 || index == 3 || index == 6 || index == 7)
	{ 
	  if (silence == 0) {
	    time = timerstart();
	    cout << "Printing potential to file... " << flush;
	  }

	  datafile.open("pot.dat");
	  //datafile << "Potential" << endl << endl;
	  count = 0;
	  percent = rdim / 100.0;
	  
	  for (row = 0; row < rdim; row++)
	    {
	      for (column = 0; column < cdim; column++)
		{
		  // Actual values of potential (for plotting etc.)
		  datafile << cf(row,smin,ds,maxres) << " "
			   << cf(column,smin,ds,maxres) << " "
			   << output[row][column][0] << "\n";
		}
	      datafile << "\n";
	      
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
  
  // Print final time to terminal
  if (silence == 0) {
    runtime = timerend(start,1);
    cputime = timer(1); }
  else {
    runtime = timerend(start);
    cputime = timer(); }

  // Output runtime and cpu time to text file
  datafile.open("stats.dat");
  datafile << "Input arguments: ";
  for (i = 1; i < argc; i++) {
    datafile << argv[i] << " "; }
  datafile << "\n" << "Runtime: " << runtime << "s"
	   << "\n" << "CPU time: " << cputime << "s\n" << endl;
  datafile.close();
  
  
  return 0;
}

float cf(int matind, float min, float ds, int maxres)
{
  return min + ((ds/(float)maxres)*matind);
}
