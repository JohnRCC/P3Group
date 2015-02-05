#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include "EasyBMP.h"

using namespace std;

int main() {

//read in image from bitmap
BMP Image;
Image.ReadFromFile("lars.bmp");

//read in image dimensions
int rowsize = Image.TellHeight();
int columnsize = Image.TellWidth();

int iter;
cin >> iter;

//generate boundary conitions
// RED = +V
// BLUE = -V
// BLACK = 0
float vals[rowsize][columnsize][3];
int row,column,i;
for (row=0; row<rowsize; row++) {
	for (column=0; column<columnsize; column++) {
		if (Image(column,row)->Red == 255 && Image(column,row)->Green == 0 && Image(column,row)->Blue == 0) {
			vals[row][column][0] = 1;
			vals[row][column][1] = 1;
			vals[row][column][2] = 1;
		}
		else if (Image(column,row)->Red == 0 && Image(column,row)->Green == 0 && Image(column,row)->Blue == 255) {
			vals[row][column][0] = -1;
			vals[row][column][1] = -1;
			vals[row][column][2] = -1;
		}
		else if (Image(column,row)->Red == 0 && Image(column,row)->Green == 0 && Image(column,row)->Blue == 0) {
			vals[row][column][0] = 0;
			vals[row][column][1] = 0;
			vals[row][column][2] = 0;
		}
		else {
			vals[row][column][0] = 0;
			vals[row][column][1] = 0;
			vals[row][column][2] = 2;
		}
	}
}

//run algorithm to get average of values
float left, right, up, down;
for(i=0;i<iter;i++) {
	for(row=0;row<rowsize;row++) {
		for(column=0;column<columnsize;column++) {

			left = vals[row][column-1][(i%2)];
			right = vals[row][column+1][(i%2)];
			up = vals[row+1][column][(i%2)];
			down = vals[row-1][column][(i%2)];

			if (vals[row][column][2] != 2) {
				vals[row][column][-(i%2)+1] = vals[row][column][2];
			}
			else if(row==0 && column==0) {
				vals[row][column][-(i%2)+1] = (right+up)/2.00;
			}
			else if(row==0 && column==columnsize-1) {
				vals[row][column][-(i%2)+1] = (left+up)/2.00;
			}
			else if(row==rowsize-1 && column==0) {
				vals[row][column][-(i%2)+1] = (down+right)/2.00;
			}
			else if(row==rowsize-1 && column==columnsize-1) {
				vals[row][column][-(i%2)+1] = (left+down)/2.00;
			}
			else if(column==0) {
				vals[row][column][-(i%2)+1] = (right+down+up)/3.00;
			}
		        else if(column==columnsize-1) {
				vals[row][column][-(i%2)+1] = (left+down+up)/3.00;
			}
			else if(row==0) {
				vals[row][column][-(i%2)+1] = (right+up+left)/3.00;
			}
			else if(row==rowsize-1) {
				vals[row][column][-(i%2)+1] = (right+left+down)/3.00;
			}
			else {
				vals[row][column][-(i%2)+1] = (up+down+right+left)/4.00;
			}
		
		}
	}
}
 
//open file to write data to
ofstream datafile;
datafile.open("the_datafile.dat");

//output the data in suitbale format
for(row=0;row<rowsize;row++) {
	for(column=0;column<columnsize;column++) {
		datafile << row << " " << column << " " << vals[row][column][-(i%2)+1] << endl;
	}
	datafile << endl;
}

//close data file
datafile.close(); 

return 0;

}

