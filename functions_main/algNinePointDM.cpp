#ifndef ALGORITHM_FUNCTION
#define ALGORITHM_FUNCTION
#include <iostream>
#include <math.h>

using namespace std;

/*	Function for running the five-point difference method algorithm on the main program.

Input:
- 3-Dimensional matrix
- Row size
- Column size
- Chance tolerance (or iteration count)
- "Silence" index - output algorithm progresson to stdout

Output:
- Iteration count


      _|_____|_____|_____|__
       |     |     |     | 
 A     |  NW |  N  |  NE |
 |    _|_____|_____|_____|_
 |     |     |     |     |
 |     |  W  |  ME |  E  |
row   _|_____|_____|_____|_
incr.  |     |     |     |
       |  SW |  S  |  SE |
      _|_____|_____|_____|_
       |     |     |     |
        column incr. ---->

      The nine-point difference method formula gives: 

      10/3*ME = 2/3*(N + S + W + E) + 1/6*(NW + NE + SW + SE) 

      So that,

      ME = 1/5*(N + S + W + E) + 1/20*(NW + NE + SW + SE)
         = Ka(N + S + W + E) + Kd(NW + NE + SW + SE), 
         Ka the coefficeints for the adjacent elements and
         Kd for the diagonal

       Note that: 
                  4Ka + 4Kd = 1, and
                  Ka/Kd = 4

      Special cases are considered; when "ME", the value to be calculated
       is either on a corner or on an edge.

      ME(NW corner) = 4/9*(S + E) + 1/9*(SE)
       Eg, upper left corner, 2Ka + Kd = 1 with Ka/Kd = 4

      ME(edge) = 2/7*(N + E + S) + 1/14(NE + SE)
       Ed. western edge. 3Ka + 2Kd = 1, Ka/Kd = 4


  GAUSS-SEIDEL METHOD: 

    Turn Gauss on by setting is as true.

   This method utilizes the already calculated values for some elements 
    around the point of interest. 
   More specifically, the values below and the value to the left (S,SW,SE,W).




 */


int algNinePointDM(double*** vals, int columnsize, int rowsize,double errtol, int silence) {


  double  ME, PreviousME, ratio;
  int count = 0,i = 0,row,column;
  bool test = true;
  double percent = errtol / 100.0;

  // Coefficients for non-boundary elements
  double W, E, N, S, SW, SE, NW, NE;
  double dAdjCoeff = (1.00/5.00), dDiagCoeff = (1.00/20.00) ;

  // Coefficients for corner elements
  double dCornerAdjCoeff = (4.00/9.00), dCornerDiagCoeff = (1.00/9.00);

  //Coefficients for non-corner sides
  double dSideAdjCoeff = (2.00/7.00), dSideDiagCoeff = (1.00/14.00);

  // Gauss -Seidel method : true for ON
  bool bGauss = true;
  if (bGauss) {cout << "(Gauss-Seidel method ON) ";}

// RUN THE NINE-POINT DIFERRENCE METHOD ALGORITHM
  while(test == true) 
  { 
    if(errtol > 100 && i >= errtol) {
      test = false; }
    else if(errtol < 100 && i > columnsize) {
      test = false; }

    for(row = 0; row < rowsize; row++) {
      for(column = 0; column < columnsize; column++) {
      // DEFINE ELEMENTS AROUND POINT OF INTEREST
        // Define adjacent points (N, S, W, E)
        if (column != 0 ) {
          if (bGauss) {
            W = vals[row][column-1][-(i%2)+1];}
          else { W = vals[row][column-1][(i%2)]; }
            // Diagonal, westward points
          if (row != 0) {
            if (bGauss) {
              SW = vals[row-1][column-1][-(i%2)+1];}
            else { SW = vals[row-1][column-1][(i%2)]; }
          }
          if (row != rowsize-1){
            NW = vals[row+1][column-1][(i%2)]; }
        }
        if (column != columnsize-1) {
          E = vals[row][column+1][(i%2)];
            // Diagonal, eastward points
          if (row != 0) {
            if (bGauss) {
              SE = vals[row-1][column+1][-(i%2)+1];}
            else {SE = vals[row-1][column+1][(i%2)]; }
          }
          if (row != rowsize-1){
            NE = vals[row+1][column+1][(i%2)]; }
        }
        if (row != rowsize-1) {
          N = vals[row+1][column][(i%2)]; }
        if (row != 0) {
          if (bGauss) {
            S = vals[row-1][column][-(i%2)+1];}
          else {S = vals[row-1][column][(i%2)]; }
        }
        // APPLY PROPAGATION FOR VARIABLE ELEMENTS
          // Cast Boundary conditions (held in the third layer)
        if (vals[row][column][2] != 2) {
          vals[row][column][-(i%2)+1] = vals[row][column][2]; }
          // SW corner
        else if (row == 0 && column == 0) {
          vals[row][column][-(i%2)+1] = dCornerAdjCoeff*(S + E) + dCornerDiagCoeff*(NE); }
         // SE corner
        else if (row == 0 && column == columnsize-1) {
          vals[row][column][-(i%2)+1] = dCornerAdjCoeff*(N + W) + dCornerDiagCoeff*(NW); }
         // NW corner
        else if (row == rowsize-1 && column == 0) {
          vals[row][column][-(i%2)+1] = dCornerAdjCoeff*(S + E) + dCornerDiagCoeff*(SE); }
          // NE corner
        else if (row == rowsize-1 && column == columnsize-1) {
          vals[row][column][-(i%2)+1] = dCornerAdjCoeff*(S + W) + dCornerDiagCoeff*(SW); }
          // W - side
        else if (column == 0) {
          vals[row][column][-(i%2)+1] = dSideAdjCoeff*(S + E + N) + dSideDiagCoeff*(SE + NE); }
          // E - side
        else if (column == columnsize-1) {
            vals[row][column][-(i%2)+1] = dSideAdjCoeff*(S + W + N) + dSideDiagCoeff*(SW + NW); }
          // S - side
        else if (row == 0) {
          vals[row][column][-(i%2)+1] = dSideAdjCoeff*(N + W + E) + dSideDiagCoeff*(NE + NW); }
          // N - side
        else if (row == rowsize-1) {
          vals[row][column][-(i%2)+1] = dSideAdjCoeff*(S + W + E) + dSideDiagCoeff*(SW + SE); }
          // Inside
        else {
          vals[row][column][-(i%2)+1] = dAdjCoeff*(N + S + W + E) + dDiagCoeff*(NW + NE + SW + SE); }
        // Check if errtol accuracy has been achieved yet
          // ME = just calculated value, PreviousME = previous iteration value
        ME = vals[row][column][-(i%2)+1];
        PreviousME = vals[row][column][(i%2)];
        if (test == false && errtol < 100) { 		
          ratio = fabs(ME/PreviousME);
          if (ratio <= (1.0-errtol/100.0) || ratio >= (1.0+errtol/100.0)) {
            test = true; }
        }
      }
    } 
    // Display percentage completion for iteration case
    if (errtol > 100) {
      if (i > (count*percent) && silence == 0) {
        if (count < 10 ) {
          cout << count << "%\b\b" << flush; }
        else {
          cout << count << "%\b\b\b" << flush; }
        count++;
      }
    }
    i++;
    // Display current iteration number for tolerance case
    if (errtol < 100) {
      count = log10(i);
      int x = pow(10.0,double(count));
      if (i%x == 0) {
        cout << "Over " << i << " iterations...";
        for(x = 0; x < count+20; x++) {
          cout << "\b";
        }
        cout << flush;
      }
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

  return i;

}


#endif
