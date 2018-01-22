/* --------------------------------------------------------------------------------------
		Exercsie 1: Simple C++ Programming
			Test 4: Mutiplification Sheet
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#include <string>
using namespace std;

int main()
{
	int a, b;
	for( a = 1; a <= 9; ++a)
	{
		for( b = 1; b <= 9; ++b)
		{
			if ( b >= a) {
				cout << a << "*" << b << "=";
				cout.width(5);
				cout.fill(' ');
				cout.setf(ios::left);
				cout << a*b;
			}
			else {
				cout.width(9);
				cout.fill(' ');
				cout << " ";
			}
		}
		cout << endl;
	}
}
