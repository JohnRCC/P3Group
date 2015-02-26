#ifndef SUBLAYER_CLASS
#define SUBLAYER_CLASS

// A class to contain a sublayer of our grid, for when we want to look at one
// point at a higher resolution. Includes functions to create these objects.

class Sublayer
{
public:
  // Which constructor the class was built by
  int index;
  // The x co-ordinate of the top-layer point
  int topx;
  // The y co-ordinate of the top-layer point
  int topy;
  // The size of the submatrix (length along one side)
  int size;
  // The submatrix itself
  double** array;

  // The format is array[row][column]

  // The constructor functions
  Sublayer(int, int, double, double, double, double, double, int, int);
  Sublayer();
};


// Constructor function to create a the submatrix.
// Takes as input the grid co-ordinates, the value of the point on the
// top layer, the values of the adjacent points, grid size
// (number of points to a side), and whether smoothing is enabled.
Sublayer::Sublayer(int xpoint, int ypoint, double self,
		   double above, double below, double left, double right,
		   int gridsize, int smoothing)
{
  // Convert arguments into struct values
  index = 0;
  topx = xpoint;
  topy = ypoint;
  size = gridsize;

  // Create the submatrix
  array = new double*[size];
  for (int r = 0; r < size; r++)
    {
      array[r] = new double[size];
    }

  // Define the submatrix's initial values
  // All non-edge values are set to the top-layer value
  for (int r = 1; r < (size - 1); r++)
    {
      for (int c = 1; c < (size - 1); c++)
	{
	  array[r][c] = self;
	}
    }

  // Edge values are then set to the average of the adjacent and central
  // top-layer values

  // Top row
  int r = 0;
  for ( int c = 1; c < size-1; c++ )
    {
      array[r][c] = (above + self) / 2.0;
    }
  // Bottom row
  r = size - 1;
  for ( int c = 1; c < size-1; c++ )
    {
      array[r][c] = (below + self) / 2.0;
    }
  // Left column
  int c = 0;
  for ( int r = 1; r < size-1; r++ )
    {
      array[r][c] = (left + self) / 2.0;
    }
  // Right column
  c = size - 1;
  for ( int r = 1; r < size-1; r++ )
    {
      array[r][c] = (right + self) / 2.0;
    }

  // Corners are the average of the two ajacent top-layer points
  array[0][0] = ( above + left ) / 2.0;
  array[0][size-1] = ( above + right ) / 2.0;
  array[size-1][0] = ( below + left ) / 2.0;
  array[size-1][size-1] = ( below + right) / 2.0;

  // Apply smoothing if enabled and if adjacent edges are equal-valued
  if ( smoothing == 1 )
    {
      if ( above == left )
	{
	  int count = 0;
	  while ( count < size )
	    {
	      for (int c = 0; c < (size - count); c++)
		{
		  array[count][c] = above;
		}
	      count++;
	    }
	}
      
      if ( above = right )
	{
	  int count = 0;
	  while ( count < size )
	    {
	      for (int c = count; c < size; c++)
		{
		  array[count][c] = above;
		}
	      count++;
	    }
	}
      
      if ( below == left )
	{
	  int count = size-1;
	  while ( count >= 0 )
	    {
	      for ( int c = 0; c <= count; c++ )
		{
		  array[count][c] = below;
		}
	      count--;
	    }
	}

      if ( below == right )
	{
	  int count = size-1;
	  while ( count >= 0 )
	    {
	      for ( int c = (size - count + 1); c < size; c++)
		{
		  array[count][c] = below;
		}
	      count--;
	    }
	}
    }

  // Done
}


// A constructor used for arrays of sublayers.
// Creates an object containing as little data as possible.
Sublayer::Sublayer()
{
  // Default values
  index = 1;
  topx = 0;
  topy = 0;
  size = 1;

  // Create the submatrix
  array = new double*[size];
  for (int r = 0; r < size; r++)
    {
      array[r] = new double[size];
    } 
}  


// Function to create a sublayer for a given point in the top layer.
// Takes as input the x and y co-ordiantes, the array containing the top layer,
// and optionally the size to make the sublayer, the number of balancing
// pases to use, and whether smoothing is enabled.
// Returns the sublayer.
// Function will break if used on top, bottom, leftmost or rightmost points.
Sublayer sublayer(int column, int row, double*** toplayer,
		  int size = 13, int iter = 15, int smooth = 0)
{

  // Define adjacent toplayer values
  double above = toplayer[row-1][column][1];
  double below = toplayer[row+1][column][1];
  double left = toplayer[row][column-1][1];
  double right = toplayer[row][column+1][1];
 
  // Create the sublayer
  Sublayer Temp(column, row, toplayer[row][column][1],
		above, below, left, right, size, smooth);
 
  // Check for smoothing (enabled and equal-valued adjacent edges)
  // If found, exclude the smoothed region from balancing
  if ( smooth == 1 )
    {
      if ( above == left )
	{
	  for(int i = 0; i < iter; i++)
	    {
	      int count = size - 1;
	      for(int r = 1; r < size-1; r++)
		{
		  for(int c = 1; c < count; c++)
		    {
		      Temp.array[r][c] =
			( Temp.array[r+1][c] + Temp.array[r-1][c] +
			  Temp.array[r][c+1] + Temp.array[r][c-1] )
			/ 4.0;
		    }
		  count--;
		}
	    }
	  return Temp;
	}

      else if ( above == right )
	{
	  for(int i = 0; i < iter; i++)
	    {
	      int count = size - 1;
	      for(int r = 1; r < size-1; r++)
		{
		  for(int c = (size - count); c < size-1; c++)
		    {
		      Temp.array[r][c] =
			( Temp.array[r+1][c] + Temp.array[r-1][c] +
			  Temp.array[r][c+1] + Temp.array[r][c-1] )
			/ 4.0;
		    }
		  count--;
		}
	    }
	  return Temp;
	}
 
     else if ( below == left )
	{
	  for(int i = 0; i < iter; i++)
	    {
	      int count = size - 1;
	      for(int r = 1; r < size-1; r++)
		{
		  for(int c = 1; c < (size - count); c++)
		    {
		      Temp.array[r][c] =
			( Temp.array[r+1][c] + Temp.array[r-1][c] +
			  Temp.array[r][c+1] + Temp.array[r][c-1] )
			/ 4.0;
		    }
		  count--;
		}
	    }
	  return Temp;
	}

      else if ( below == right )
	{
	  for(int i = 0; i < iter; i++)
	    {
	      int count = size - 1;
	      for(int r = 1; r < size-1; r++)
		{
		  for(int c = (size - count); c < size-1; c++)
		    {
		      Temp.array[r][c] =
			( Temp.array[r+1][c] + Temp.array[r-1][c] +
			  Temp.array[r][c+1] + Temp.array[r][c-1] )
			/ 4.0;
		    }
		  count--;
		}
	    }
	  return Temp;
	}
    }
   
  // Otherwise, iterate and balance across the whole grid
  for(int i = 0; i < iter; i++)
    {
      for(int r = 1; r < size-1; r++)
	{
	  for(int c = 1; c < size-1; c++)
	    {
	      // Make the value of each point the average of its neighbours
	      Temp.array[r][c] =
		( Temp.array[r+1][c] + Temp.array[r-1][c] +
		  Temp.array[r][c+1] + Temp.array[r][c-1] )
		/ 4.0;
	    }
	}
    }
  
  // Return the sublayer
  return Temp;
}


// Function to create a sublayer for a given point in the top layer and
// return the average of its values.
// Takes as input the x and y co-ordiantes, the array containing the top layer,
// and optionally the size to make the sublayer, the number of balancing passes
// to use, and whether smoothing is enabled.
// Function will break if used on top, bottom, leftmost or rightmost points.
double submean(int column, int row, double*** toplayer,
	       int size = 13, int iter = 15, int smooth = 0)
{
  // Create the sublayer
  Sublayer Temp(column, row, toplayer[row][column][0],
		toplayer[row-1][column][0], toplayer[row+1][column][0],
		toplayer[row][column-1][0], toplayer[row][column+1][0],
		size, smooth);
  
  // Iterate through and average the points
  for(int i = 0; i < iter; i++)
    {
      for(int r = 1; r < size-1; r++)
	{
	  for(int c = 1; c < size-1; c++)
	    {
	      Temp.array[r][c] =
		( Temp.array[r+1][c] + Temp.array[r-1][c] +
		  Temp.array[r][c+1] + Temp.array[r][c-1] )
		/ 4.0;
	    }
	}
    }

  // Find the mean value for the submatrix and return it
  double mean;
  for(int r = 0; r < size; r++)
    {
      for(int c = 0; c < size; c++)
	{
	  mean = mean + Temp.array[r][c];
	}
    }
  mean = (double)mean / (size * size);
  return mean;
}


#endif
