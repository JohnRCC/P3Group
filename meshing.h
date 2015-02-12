#ifndef MESHING
#define MESHING
#include "sublayer.h"

// A function to implement meshing. Takes the top-level array and uses the
// pre-existing gradient data to create submatrices and store them in an array,
// which is then returned.

// Example of use:
// Sublayer** example = meshing(toplayer, 5);


// Function prototype
double mod(double);


// The meshing function. Takes as input the top-level array, its size,
// and optionally whether to smooth edges (for use with circles)
Sublayer** meshing(double*** toplayer, int rowsize, int columnsize,
		   int smoothing = 0)
{
  // Establish a maximum gradient
  double maxgrad = 0;

   // Loop through the matrix, checking the gradient of each point
  for ( int r = 0; r < rowsize; r++ )
    {
      for ( int c = 0; c < columnsize; c++)
	{
	  double g = toplayer[r][c][2];

	  // Check if it is the highest gradient encountered
	  if ( abs(g) > maxgrad )
	    {
	      maxgrad = g;
	    }
	}
    }

  // Set the boundary conditions for sublayer size
  int lim9 = maxgrad * 0.75;
  int lim3 = maxgrad * 0.5;

  // Create an array to store the pointers
  Sublayer** pointers = new Sublayer*[rowsize];
  for (int r = 0; r < rowsize; r++)
    {
      pointers[r] = new Sublayer[columnsize];
    }

  // Create the sublayers where necessary
  for ( int r = 1; r < rowsize-1; r++ )
    {
      for ( int c = 1; c < columnsize-1; c++ )
	{
	  // Maximum sublayer size
	  if ( mod(toplayer[r][c][2]) > lim9 )
	    {
	      pointers[r][c] = sublayer(c, r, toplayer, 9, 100, smoothing);
	      //pointers[r][c] = sublayer(c, r, toplayer, 9, 15, smoothing);
	    }
	  // Moderate sublayer size
	  else if ( mod(toplayer[r][c][2]) > lim3 )
	    {
	      pointers[r][c] = sublayer(c, r, toplayer, 3, 100, smoothing);
	      //pointers[r][c] = sublayer(c, r, toplayer, 3, 3, smoothing);
	    }
	  // Else there is no sublayer
	}
    }

  // Return the array containing the sublayers
  return pointers;
}


// A function to find the absolute value of a number
double mod(double val)
{
  if ( val < 0 )
    {
      val = -1 * val;
    }
  return val;
}

#include <iostream>
#include <iomanip>

// A function to transpose the top-layer matrix and its sublayers
// onto a single unified matrix, which is then returned
double** printmeshalt(double*** toplayer, Sublayer** mesh,
		      int rowsize, int columnsize, int maxres)
{
  int rdim = rowsize * maxres;
  int cdim = columnsize * maxres;

  // Create a super-array
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

	  // In the case of there being a sublayer
	  if ( mesh[r][c].index == 0 )
	    {
	      // Establish the resolution of the sublayer
	      int subsize = mesh[r][c].size;
	      int scale = maxres / subsize;
	      
	      // Print the sublayer values to the supermatrix
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

	  // In the case of there not being a sublayer
	  else
	    {
	      // The top-layer value is assigned to all the supermatrix points
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

  return output;
}



double** nomeshing(double*** toplayer, int rowsize, int columnsize, int maxres)
{
  int rdim = rowsize * maxres;
  int cdim = columnsize * maxres;

  // Create a super-array
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
	  // The top-layer value is assigned to all the supermatrix points
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
  
  return output;
}
      
#endif
