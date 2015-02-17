#ifndef GRAD_H
#define GRAD_H

// Check on gradient. Returns a value based on 4 surrounding points
double grad(double*** toplayer, int row, int column, float ds)
{
  double above = toplayer[row-1][column][1];
  double below = toplayer[row+1][column][1];
  double left = toplayer[row][column-1][1];
  double right = toplayer[row][column+1][1];
  double result = pow( pow( (right-left) / ds, 2.00 )
		       + pow( (below-above) / ds, 2.00 ),
		       0.50 );
  return result;
}

// Determine the gradient at each point of a matrix and return it
double*** getgrad(double*** vals, int rowsize, int columnsize, float ds)
{
  int row, column;

  for (row = 1; row < rowsize-1; row++)
    {
      for (column = 1; column < columnsize-1; column++)
	{
	  vals[row][column][2] = grad(vals, row, column, ds);
	}
    }
  
  // Set edge gradients to zero
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

  return vals;
}

#endif
