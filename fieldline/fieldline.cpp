#include <iostream>
#include "fieldline.h"
#include <fstream>


using namespace std;

int fldline(int row,int col,double** matrix,float dx,float dy)
{
  int r;
  int c;
  float xavg=0;
  float yavg=0;
  int tic=10;

  ofstream field;
  field.open("field.dat");
  //Output coords and vals

  //initialise dxmax & dymax values
  float dxmax = 0;
  float dymax = 0;

  for(r=1;r<row-1;r++)
  {
    for(c=1;c<col-1;c++)
    {
      if(c%tic==0 && r%tic==0)
      {
	Fieldline Test = *fline(r,c,matrix,dx,dy);
	if(Test.fieldvals[0] > dxmax)
	  dxmax = Test.fieldvals[0];
	if(Test.fieldvals[1] > dymax)
	  dymax = Test.fieldvals[1];
	//field<<Test.xcord<<" "<<Test.ycord<<" "<<Test.fieldvals[0]<<" "<<Test.fieldvals[1]<<"\n";
      }
    }
    //field<<"\n";
  }

  for(r=1;r<row-1;r++)
  {
    for(c=1;c<col-1;c++)
    {
      if(c%tic==0 && r%tic==0)
      {
	Fieldline Test = *fline(r,c,matrix,dx,dy);
	field<<Test.xcord<<" "<<Test.ycord<<" "<<10*(Test.fieldvals[0])/dxmax<<" "<<10*(Test.fieldvals[1])/dymax<<"\n";
      }
    }
  field<<"\n";
  }
  field.close();
  
  return 0;

}
