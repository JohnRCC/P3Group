#ifndef FUNCS_H
#define FUNCS_H

// A list of function prototypes

//function to take calculated matrix and generate field line file
int fldline(int row,int col,double** matrix,float dx,float dy);

//function to calculate potential analytically
float potential(float x,float y,float r,float Plate_separation);

//function to generate file for analytic solution
int analytic(float smin,float ds,float smax,float r);

//CoordiFy converts the matrix location of a point into its physical coordinate
float cf(float matind,float min,float ds);

// Check on gradient. Returns a value based on 4 surrounding points
double grad(double*** toplayer, int row, int column, float ds);


#endif
