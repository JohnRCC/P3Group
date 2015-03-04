#include <fstream>
#include <iostream>
#include <string>

// update number of rows in main.
// update file name in main.

float** read_file(std::string filename,int rows,int cols)
{
	std::fstream file;
	file.open(filename.c_str(), std::ios::in);
	if(!file.is_open()){return 0;}

	float** floats = new float*[cols+1];
	for(int i = 0; i <cols;++i){ floats[i] = new float[rows+1]; }

	//read each row
	for(int i =0;i<rows;++i)
	{
		for(int j =0;j<cols;++j)//push into the col
		{ file >>floats[j][i]; }
	}
	file.close();

	return floats;
}

int main()
{
	int rows = 3;
	int cols = 3;
	float** floats;
	if( !(floats = read_file("test",rows,cols) ) ){return 0;}

	//write out the data
	for(int i =0;i<rows;++i)
	{
		for(int j =0;j<cols;++j){ std::cout <<floats[j][i] <<"\t"; }
		std::cout <<"\n";
	}
	char wait;
	std::cin >>wait;

	return 0;
}
