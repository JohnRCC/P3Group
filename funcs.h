#ifndef FUNCS_H
#define FUNCS_H

// A list of functions not detailed enough to merit their own header files
// as well as function prototypes

//function to take calculated matrix and generate field line file
int fldline(int row,int col,double** matrix,float dx,float dy);

//function to calculate potential analytically
float potential(float x,float y,float r,float Plate_separation);

//function to generate file for analytic solution
int analytic(float smin,float ds,float smax,float r);

//CoordiFy converts the matrix location of a point into its physical coordinate
float cf(float matind,float min,float ds) {
  // matind = index of value in array, min = min true coord value,
  // ds = coord division
  return min + (ds*matind);
}

// Check on gradient. Returns a value based on 4 surrounding points
double grad(double*** toplayer, int row, int column)
{
double above = toplayer[row-1][column][1];
double below = toplayer[row+1][column][1];
double left = toplayer[row][column-1][1];
double right = toplayer[row][column+1][1];
double result = pow( pow((right-left)/ds, 2.00) + pow((down-up)/ds, 2.00), 0.50);
return result;
}


#endif
