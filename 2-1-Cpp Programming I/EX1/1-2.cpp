/* --------------------------------------------------------------------------------------
		Exercsie 1: Simple C++ Programming
			Test 2: Prime Number
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#include <math.h>
#include <fstream>
using namespace std;

int main()
{
	int i = 2, upper;
	cout << "Please input the upper number N:" << endl;
	cin >> upper;
	fstream tar;
	tar.open("Record.txt",ios::app|ios::in);
	tar << "The prime numbers below " << upper << " are: " << i;

	while( i <= upper)
	{
		int circ = 2, sig = 0;
		while( circ <= (int)sqrt(i))
		{
			if ( i % circ == 0)
			{
				sig = 0;
				break;
			}
			else
				sig = 1;
			++circ;		
		}
		if (sig == 1)
			tar << ", " << i;
		else;
		++i;
	}
	
	tar << "." << endl;
}
