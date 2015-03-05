#ifndef FUNCS_H
#define FUNCS_H
//#include "./fieldline/fieldline.h"


//function to take calculated matrix and generate field line file
int fldline(int row,int col,double** matrix,float dx,float dy);

//function to calculate potential analytically
float potential(float x,float y,float r,float Plate_separation);

//function to generate file for analytic solution
int analytic(float smin,float ds,float smax,float r,int maxres,int silence);

<<<<<<< HEAD:funcs.h
//coordify function
float cf(float matind,float min,float ds);

=======
//CoordiFy converts the matrix location of a point into its physical coordinate
float cf(float matind,float min,float ds,int maxres);

// Check on gradient. Returns a value based on 4 surrounding points
double grad(double*** toplayer, int row, int column, float ds);

// Timer functions
double timerstart(void);
double timerend(double, int);
double timer(int);

//Function to run five-point Difference Method algorithm
int algFivePointDM(double*** vals, int columnsize, int rowsize, double errtol, int silence);

// Function to run the nine-point Difference Method algorithm
int algNinePointDM(double*** vals, int columnsize, int rowsize,double errtol, int silence); 

>>>>>>> master:headers/funcs.h
#endif
