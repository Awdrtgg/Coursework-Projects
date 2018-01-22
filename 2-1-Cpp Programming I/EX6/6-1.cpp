/* --------------------------------------------------------------------------------------
		Exercsie 6: IO stream
			Test 1: IO stream 1
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main()
{
	fstream f;
	f.open("6-1.txt",ios::app|ios::in);
	int a = 15;
	long int b = 1000000000;
	double x = 1.526234567;
	
	f << "This is an int:" << a << endl;
	f << "This is a long int:" << b << endl;
	f << "This is an unsigned int:" << (unsigned) a << endl;
	f << "This is a float:" << (float) x << endl;
	f << "This is a double:" << x << endl;
	
	f << "for the value 15," << endl;
	f << "Dec:" << dec << a << endl;
	f << "Hex:" << hex << a << endl;
	f << "Oct:" << oct << a << endl;
	
	f << "precision(5) for 1.526:" << setprecision(5) << x << endl;
	
	string s = "kindle";
	f << setfill('*') << setw(10) << s << endl;
	
	f.close();
	
}
