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
  int tic=5;

  ofstream field;
  field.open("field.dat");
  //Output coords and vals
  for(r=1;r<row-1;r++)
  {
    for(c=1;c<col-1;c++)
    {
      if(c%tic==0 && r%tic==0)
      {
	Fieldline Test = *fline(r,c,matrix,dx,dy);
	xavg+=Test.fieldvals[0];
	yavg+=Test.fieldvals[1];
	//field<<Test.xcord<<" "<<Test.ycord<<" "<<Test.fieldvals[0]<<" "<<Test.fieldvals[1]<<"\n";
      }
    }
    //field<<"\n";
  }

  xavg = tic*(xavg/row);
  yavg = tic*(yavg/col);

  for(r=1;r<row-1;r++)
  {
    for(c=1;c<col-1;c++)
    {
      if(c%tic==0 && r%tic==0)
      {
	Fieldline Test = *fline(r,c,matrix,dx,dy);
	field<<Test.xcord<<" "<<Test.ycord<<" "<<(Test.fieldvals[0])/xavg<<" "<<(Test.fieldvals[1])/yavg<<"\n";
      }
    }
  field<<"\n";
  }
  field.close();
  
  return 0;

}
