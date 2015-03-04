#include <cmath>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{

float sum, m, mean, ball;

sum = strtof(argv[1],NULL);
m = strtof(argv[2],NULL);

cout << "sum = " << sum << " and m = " << m << endl;
mean=((50.0*sum)/m); 
cout << "mean = " << mean << endl;

ball=0;
cout << "ball = " << ball << endl << endl;;
int i=0;

for (i; i<= m; i++)
{
ball += i;
cout << ball << endl;
}

cout << endl << endl << endl << ball << endl;

ifstream derp;
derp.open ("test");
float data[6], a, b, c, d, e, f;
for (int p=0;p<=6;p++)
{
derp>>data[i];
}
derp.close();

for (int x=0;x<=6;x++)
{
cout << data[x] << endl;
}


return 0;

}
