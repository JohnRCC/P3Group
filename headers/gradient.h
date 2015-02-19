#ifndef GRAD_H
#define GRAD_H

// A function to determine the gradient of a point. Takes as input a matrix,
// the co-ordinates of the point to take the gradient of, and the change in
// distance between points. Returns the gradient.
double grad(double*** toplayer, int row, int column, float ds)
{
  // Define adjacent values
  double above = toplayer[row-1][column][1];
  double below = toplayer[row+1][column][1];
  double left = toplayer[row][column-1][1];
  double right = toplayer[row][column+1][1];

  // Determine the gradient
  double result = pow( pow( (right-left) / ds, 2.00 )
		       + pow( (below-above) / ds, 2.00 ),
		       0.50 );

  // Return the gradient
  return result;
}


// A function to determine and record the gradient at each point of a matrix.
// Takes as input a matrix, its dimensions, and the distance between each of
// its points.
int getgrad(double*** vals, int rowsize, int columnsize, float ds)
{
  // Declare variables
  int row, column;

  // Loop through the matrix, excluding the outermost points, and determine
  // the gradient at each point
  for (row = 1; row < rowsize-1; row++)
    {
      for (column = 1; column < columnsize-1; column++)
	{
	  vals[row][column][2] = grad(vals, row, column, ds);
	}
    }
  
  // Set the gradients of the outermost points to zero
  for (row = 0; row < rowsize; row++)
    {
      vals[row][0][2] = 0;
      vals[row][columnsize-1][2] = 0;
    }
  for (column = 0; column < columnsize; column++)
    {
      vals[0][column][2] = 0;
      vals[rowsize-1][column][2] = 0;
    }

  return 0;
}

#endif
