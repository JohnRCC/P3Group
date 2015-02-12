#ifndef FUNCS_H
#define FUNCS_H
//#include "./fieldline/fieldline.h"


//function to take calculated matrix and generate field line file
int fldline(int row,int col,double** matrix,float dx,float dy);

//function to calculate potential analytically
float potential(float x,float y,float r,float Plate_separation);

//function to generate file for analytic solution
int analytic(float smin,float ds,float smax,float r);

//coordify function
float cf(float matind,float min,float ds);

#endif
