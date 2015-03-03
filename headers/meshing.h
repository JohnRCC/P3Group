#ifndef MESHING
#define MESHING
#include "sublayer.h"
#include "timer.h"
#include <iostream>
#include <iomanip>
using namespace std;

// A set of functions related to meshing.

// Function prototype
double mod(double);


// The meshing function. Creates higher-resolution sublayers at points
// of interest (high gradient).
// Takes as input the top-level array, its dimensions,
// and optionally whether to smooth edges (for use with circles).
// Returns an array of pointers to sublayers, with sublayer[x][y]
// corresponding to toplayer[x][y]. Example of use:
// Sublayer** example = meshing(toplayer, 5);
Sublayer** meshing(double*** toplayer, int rowsize, int columnsize,
		   int maxpower = 2, int smoothing = 0, int silence = 0)
{
  if (maxpower < 1) {
    if (silence == 0) {
      cout << "No meshing required." << endl; }
    // Create empty array to return
    Sublayer** pointers = new Sublayer*[rowsize];
    for (int r = 0; r < rowsize; r++)
      {
	pointers[r] = new Sublayer[columnsize];
      }
    return pointers;
  }


  double time = timerstart();
  if (silence == 0) {
    cout << "Checking gradients... " << flush; }

  // Establish a maximum gradient
  double maxgrad = 0;

   // Loop through the matrix, checking the gradient of each point
  for (int r = 0; r < rowsize; r++)
    {
      for (int c = 0; c < columnsize; c++)
	{
	  double g = toplayer[r][c][3];

	  // Check if it is the highest gradient encountered
	  if (mod(g) > maxgrad)
	    {
	      maxgrad = g;
	    }
	}
    }

  if (silence == 0) {
    cout << "done (" << timerend(time) << "s)." << endl;
    time = timerstart();
    cout << "Establishing sublayer conditions... " << flush; }

  // Set the boundary conditions for sublayer size
  double limit[maxpower+1];
  double division = 1.0 / (maxpower+1);
  for (int i = 1; i <= maxpower; i++)
    {
      limit[i] = 0 + (i * division * maxgrad);
    }

  // Create an array to store the pointers
  Sublayer** pointers = new Sublayer*[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      pointers[r] = new Sublayer[columnsize];
    }
  
   if (silence == 0) {
     cout << "done (" << timerend(time) << "s)." << endl; }

  // Create the sublayers where necessary
  if (silence == 0) {
    time = timerstart();
    cout << "Creating sublayers... " << flush; }
  int count = 0;
  double percent = (rowsize-2) / 100.0;
  
  // Loop through the top-level matrix. The outermost points are excluded
  // as sublayer() is not designed to handle them
  for (int r = 1; r < rowsize-1; r++)
    {
      for (int c = 1; c < columnsize-1; c++)
	{
	  // For each point, check the gradient against the limits
	  for (int i = maxpower; i > 0; i--)
	    {
	      if (mod(toplayer[r][c][3]) > limit[i])
		{
		  // If the condition is met, create a sublayer of the
		  // appropiate size
		  int power = 1;
		  for (int a = 0; a < i; a++) {
		    power = power * 3; }
		  pointers[r][c] =
		    sublayer(c, r, toplayer, power, (power*10), smoothing);
		  break;
		}
	    }
	  // If the gradient is less than the lowest limit,
	  // no sublayer is created
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

  /* Previous version of the function.
     Will be deleted once we're sure the new version is strictly superior.

  for (int r = 1; r < rowsize-1; r++)
    {
      for (int c = 1; c < columnsize-1; c++)
	{
	  // Maximum sublayer size
	  if (mod(toplayer[r][c][2]) > lim9)
	    {
	      pointers[r][c] = sublayer(c, r, toplayer, 9, 90, smoothing);
	    }
	  // Moderate sublayer size
	  else if (mod(toplayer[r][c][2]) > lim3)
	    {
	      pointers[r][c] = sublayer(c, r, toplayer, 3, 30, smoothing);
	    }
	  // Else there is no sublayer
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
  */
  
  if (silence == 0) {
    cout << "done (" << timerend(time) << "s)." << endl; }
  
  // Return the array containing the sublayers
  return pointers;
}


// An alternate version of the meshing system, using the second derivative
// to determine mesh placement, instead of the first.
Sublayer** secondmeshing(double*** toplayer, int rowsize, int columnsize,
			 int maxpower = 2, int smoothing = 0, int silence = 0)
{
  if (maxpower < 1) {
    if (silence == 0) {
      cout << "No meshing required." << endl; }
    // Create empty array to return
    Sublayer** pointers = new Sublayer*[rowsize];
    for (int r = 0; r < rowsize; r++)
      {
	pointers[r] = new Sublayer[columnsize];
      }
    return pointers;
  }

  // Create an array to store the derivatives of the gradient
  double*** deriv = new double**[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      deriv[r] = new double*[columnsize];
      for (int s = 0; s < columnsize; s++)
	{
	  deriv[r][s] = new double[3];
	}
    }

  // Fill the array
  for (int r = 0; r < rowsize; r++)
    {
      for (int c = 0; c < columnsize; c++)
	{
	  deriv[r][c][1] = toplayer[r][c][2];
	}
    }

  getgrad(deriv, rowsize, columnsize, 1);

  double time = timerstart();
  if (silence == 0) {
    cout << "Checking gradients... " << flush; }

  // Establish a maximum gradient
  double maxgrad = 0;

   // Loop through the matrix, checking the gradient of each point
  for (int r = 0; r < rowsize; r++)
    {
      for (int c = 0; c < columnsize; c++)
	{
	  double g = deriv[r][c][2];

	  // Check if it is the highest gradient encountered
	  if (mod(g) > maxgrad)
	    {
	      maxgrad = g;
	    }
	}
    }

  if (silence == 0) {
    cout << "done (" << timerend(time) << "s)." << endl;
    time = timerstart();
    cout << "Establishing sublayer conditions... " << flush; }

  // Set the boundary conditions for sublayer size
  double limit[maxpower+1];
  double division = 1.0 / (maxpower+1);
  for (int i = 1; i <= maxpower; i++)
    {
      limit[i] = 0 + (i * division * maxgrad);
    }

  // Create an array to store the pointers
  Sublayer** pointers = new Sublayer*[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      pointers[r] = new Sublayer[columnsize];
    }
  
   if (silence == 0) {
     cout << "done (" << timerend(time) << "s)." << endl; }

  // Create the sublayers where necessary
  if (silence == 0) {
    time = timerstart();
    cout << "Creating sublayers... " << flush; }
  int count = 0;
  double percent = (rowsize-2) / 100.0;
  
  // Loop through the top-level matrix. The outermost points are excluded
  // as sublayer() is not designed to handle them
  for (int r = 1; r < rowsize-1; r++)
    {
      for (int c = 1; c < columnsize-1; c++)
	{
	  // For each point, check the gradient against the limits
	  for (int i = maxpower; i > 0; i--)
	    {
	      if (mod(deriv[r][c][2]) > limit[i])
		{
		  // If the condition is met, create a sublayer of the
		  // appropiate size
		  int power = 1;
		  for (int a = 0; a < i; a++) {
		    power = power * 3; }
		  pointers[r][c] =
		    sublayer(c, r, toplayer, power, (power*10), smoothing);
		  break;
		}
	    }
	  // If the gradient is less than the lowest limit,
	  // no sublayer is created
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
    cout << "done (" << timerend(time) << "s)." << endl; }
  
  // Return the array containing the sublayers
  return pointers;
}

  
// A function to find the absolute value of a number.
// Takes as input a number (double), returns its positive value.
double mod(double val)
{
  if ( val < 0 )
    {
      val = -1 * val;
    }
  return val;
}

// A function to transpose the top-layer matrix and its sublayers
// onto a single unified matrix, which is then returned.
// Takes as input the top-layer matrix, the array of sublayers,
// their dimensions, and the dimensions of the largest sublayer.
// Returns the unified matrix.
double*** printmesh(double*** toplayer, Sublayer** mesh,
		   int rowsize, int columnsize, int maxres, int silence)
{
  // Define the dimensions of the output matrix
  int rdim = rowsize * maxres;
  int cdim = columnsize * maxres;

  // Create the output matrix
  double*** output = new double**[rdim];
  for (int r = 0; r < rdim; r++)
    {
      output[r] = new double*[cdim];
      for (int s = 0; s < cdim; s++) {
	output[r][s] = new double[2]; }
    }
  
  // Loop through the top-level matrix
  int count = 0;
  double percent = rowsize / 100.0;
  for (int r = 0; r < rowsize; r++)
    {
      for (int c = 0; c < columnsize; c++)
	{
	  
	  // In the case of there being a sublayer at this point
	  if ( mesh[r][c].index == 0 )
	    {
	      // Establish the resolution of the sublayer
	      int subsize = mesh[r][c].size;
	      int scale = maxres / subsize;
	      
	      // Print the sublayer values to the output matrix
	      // For low-resolution sublayers, values will be printed
	      // more than once
	      for (int y = 0; y < subsize; y++)
		{
		  for (int yc = 0; yc < scale; yc++)
		    {
		      
		      for (int x = 0; x < subsize; x++)
			{
			  for (int xc = 0; xc < scale; xc++)
			    
			    {
			      output[(r*maxres) +(y*scale) +yc]
				[(c*maxres) +(x*scale) +xc][0] =
				mesh[r][c].array[y][x];
			      
			      // Check if the point is mean to remain static
			      if (toplayer[r][c][2] == 2) {
				output[(r*maxres) +(y*scale) +yc]
				  [(c*maxres) +(x*scale) +xc][1] = 0; }
			      else {
				output[(r*maxres) +(y*scale) +yc]
				  [(c*maxres) +(x*scale) +xc][1] = 0; }
			    }
			}
		    }
		}
	    }
	  
	  // In the case of there not being a sublayer at this point
	  else
	    {
	      // Assign the top-layer value to all the output matrix points
	      for (int y = 0; y < maxres; y++)
		{
		  for (int x = 0; x < maxres; x++)
		    {
		      {
			output[(r*maxres) +y]
			  [(c*maxres) +x][0] =
			  toplayer[r][c][1];
			
			// Check if the point is mean to remain static
			if (toplayer[r][c][2] == 2) {
			  output[(r*maxres) +y]
			    [(c*maxres) + x][1] = 0; }
			else {
			  output[(r*maxres) + y]
			    [(c*maxres) + x][1] = 0; }
		      }
		    }
		}
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
  
  // Return the output matrix
  return output;
}

  
// A function which smooths out the blocky-looking results of meshing.
// Uses the Five-Point Difference Method.
// Takes as input a matrix, its dimensions, the number of times it should
// be smoothed, and whether terminal output is enabled.
int refine5point
(double*** input, int rowsize, int columnsize, int iter, int silence)
{
  // Create a temporary matrix to store the output
  double*** output = new double**[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      output[r] = new double*[columnsize];
      for (int s = 0; s < columnsize; s++) {
	output[r][s] = new double[2]; }
    }
  
  // Set the output equal to the input
  output = input;

  // Declare adjacent points
  double left, right, up, down;
  int count = 0;
  double percent = (iter * (rowsize-2)) / 100.0;

  // Loop for the required number of times
  // On each loop, each point is averaged with its neighbours
  for (int i = 0; i < iter; i++)
    {
      for(int row = 1; row < rowsize-1; row++)
	{
	  for(int column = 1; column < columnsize-1; column++)
	    {
	      // Check if the point is meant to remain static
	      if (output[row][column][1] == 0)
		{
		  // Define adjacent points (up, down, left, right)
		  left = input[row][column-1][0];
		  right = input[row][column+1][0];
		  up = input[row+1][column][0];
		  down = input[row-1][column][0];
		  
		  // Take the average of the surrounding points
		  output[row][column][0] = (up + right + left + down ) / 4.0;
		}
	    }
	  
	  // Display percentage completion
	  if (((i * (rowsize-2)) + row) > (count*percent) && silence == 0)
	    {
	      if (count < 10) {
		cout << count << "%\b\b" << flush; }
	      else if (count < 100) {
		cout << count << "%\b\b\b" << flush; }
	      count++;
	    }
	}
      
      // At the end of each loop, update the initial matrix
      input = output;
    }
      
  return 0;
}


// A function which smooths out the blocky-looking results of meshing.
// Uses the Nine-Point Difference Method.
// Takes as input a matrix, its dimensions, the number of times it should
// be smoothed, and whether terminal output is enabled.
int refine9point
(double*** input, int rowsize, int columnsize, int iter, int silence)
{
  // Create a temporary matrix to store the output
  double*** output = new double**[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      output[r] = new double*[columnsize];
      for (int s = 0; s < columnsize; s++) {
	output[r][s] = new double[2]; }
    }
  
  // Set the output equal to the input
  output = input;

  // Declare adjacent points
  double left, right, up, down, upleft, upright, downleft, downright;

  // Declare coefficients
  double dAdjCoeff = (1.00/5.00), dDiagCoeff = (1.00/20.00);
  
  // Delare the components of the progress display
  int count = 0;
  double percent = (iter * (rowsize-2)) / 100.0;

  // Loop for the required number of times
  // On each loop, each point is averaged with its neighbours
  for (int i = 0; i < iter; i++)
    {
      for(int row = 1; row < rowsize-1; row++)
	{
	  for(int column = 1; column < columnsize-1; column++)
	    {
	      // Check if the point is meant to remain static
	      if (output[row][column][1] == 0)
		{
		  // Define adjacent points
		  left = input[row][column-1][0];
		  right = input[row][column+1][0];
		  up = input[row+1][column][0];
		  down = input[row-1][column][0];
		  upleft = input[row-1][column-1][0];
		  upright = input[row-1][column+1][0];
		  downleft = input[row+1][column-1][0];
		  downright = input[row+1][column+1][0];
		  
		  // Take the average of the surrounding points
		  output[row][column][0] = 
		    dAdjCoeff*(up + down + left + right) +
		    dDiagCoeff*(upleft + upright + downleft + downright);
		}
	    }
	  
	  // Display percentage completion
	  if (((i * (rowsize-2)) + row) > (count*percent) && silence == 0)
	    {
	      if (count < 10) {
		cout << count << "%\b\b" << flush; }
	      else if (count < 100) {
		cout << count << "%\b\b\b" << flush; }
	      count++;
	    }
	}
      
      // At the end of each loop, update the initial matrix
      input = output;
    }
      
  return 0;
}



// Function that returns a larger version of the top-layer matrix,
// without meshing and with the values directly transposed.
// Takes as input the top-layer matrix, its dimensions, and the scale factor
// for the output matrix. Returns an output matrix.
double** nomeshing(double*** toplayer, int rowsize, int columnsize, int maxres)
{
  // Define the dimensions of the ouput matrix
  int rdim = rowsize * maxres;
  int cdim = columnsize * maxres;

  // Create the output matrix
  double** output = new double*[rdim];
  for (int r = 0; r < rdim; r++)
    {
      output[r] = new double[cdim];
    }

  // Loop through the top-level matrix
  for (int r = 0; r < rowsize; r++)
    {
      for (int c = 0; c < columnsize; c++)
	{
	  // Assign the top-layer value to all the output matrix points
	  for (int y = 0; y < maxres; y++)
	    {
	      for (int x = 0; x < maxres; x++)
		{
		  {
		    output[(r*maxres) +y][(c*maxres) +x] = toplayer[r][c][1];
		  }
		}
	    }
	  
	}
    }

  // Return the output matrix
  return output;
}
      
#endif
