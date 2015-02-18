#ifndef ALGORITHM_FUNCTION
#define ALGORITHM_FUNCTION
#include <iostream>

using namespace std;

/*	Function for running the five-point difference method algorithm on the main program.

	Input:
		- 3-Dimensional matrix
		- Row size
		- Column size
		- "Silence" index - output algorithm progresson to stdout

	Output:
		- Pointer to original array

*/


double* algFivePointDM(double*** vals, int columnsize, int rowsize,int errtol, int silence) {

  double left, right, up, down;
  int count = 0,i,row,column;
  double percent = errtol / 100.0;
 
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

  

  // Ensure the first layer always contains the final matrix
  // Chose first layer rather than zeroth because the matrix will end up in
  // the first layer anyway for even numbers of iterations, which is what we'll
  // normally use


				
//  if (silence == 0) {			//add: silence to function declaration (funcs.h) for this
//   time = timerstart();
//    cout << "Normalising matrix layers... " << flush; }
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
  
//  if (silence == 0) {
//    cout << "done. (" << timerend(time) << "s)" << endl; }
  

}


#endif
