#ifndef MESHING
#define MESHING
#include "sublayer.h"
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
		   int smoothing = 0)
{
  // Establish a maximum gradient
  double maxgrad = 0;

   // Loop through the matrix, checking the gradient of each point
  for (int r = 0; r < rowsize; r++)
    {
      for (int c = 0; c < columnsize; c++)
	{
	  double g = toplayer[r][c][2];

	  // Check if it is the highest gradient encountered
	  if (mod(g) > maxgrad)
	    {
	      maxgrad = g;
	    }
	}
    }

  // Set the boundary conditions for sublayer size
  // Curently these are somewhat arbitrary
  double lim9 = maxgrad * 0.5;
  double lim3 = maxgrad * 0.25;

  // Create an array to store the pointers
  Sublayer** pointers = new Sublayer*[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      pointers[r] = new Sublayer[columnsize];
    }
 
  // Create the sublayers where necessary
  // The outermost points are excluded as sublayer() is not designed to
  // handle them
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
    }
 
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
double** printmesh(double*** toplayer, Sublayer** mesh,
		      int rowsize, int columnsize, int maxres)
{
  // Define the dimensions of the output matrix
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
				[(c*maxres) +(x*scale) +xc] =
				mesh[r][c].array[y][x];
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
			  [(c*maxres) +x] =
			  toplayer[r][c][1];
		      }
		    }
		}
	    }

	}
    }

  // Return the output matrix
  return output;
}


// A function which smooths out the blocky-looking results of meshing.
// Takes as input a matrix, its dimensions, and the number of times it should
// be smoothed.
int refine(double** input, int rowsize, int columnsize, int iter)
{
  // Create a temporary matrix to store the output
  double** output = new double*[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      output[r] = new double[columnsize];
    }
  
  // Set the output equal to the input
  output = input;

  // Declare adjacent points
  double left, right, up, down;

  // Loop for the required number of times
  // On each loop, each point is averaged with its neighbours
  for (int i = 0; i < iter; i++)
    {
      for(int row = 1; row < rowsize-1; row++)
	{
	  for(int column = 1; column < columnsize-1; column++)
	    {
	      // Define adjacent points (up, down, left, right)
	      left = input[row][column-1];
	      right = input[row][column+1];
	      up = input[row+1][column];
	      down = input[row-1][column];
	      
	      // Take the average of the surrounding points
	      output[row][column] = (up + right + left + down ) / 4.0;	      
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
