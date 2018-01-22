/* --------------------------------------------------------------------------------------
		Exercsie 1: Simple C++ programming
			Test 1: Guessing the price
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

using namespace std;

int main()
{
	srand((unsigned)time(NULL));
	int x = rand()%1000 + 1;			// get the rand number from 1 to 1000
	cout << "Please input a number between 1 and 1000: ";
	char g[30];
	int y = 0, sig = 0;		// y is the number user input, sig is to check if the input value is an integer
	cin >> g;
	while(1)
	{
		int i;
		// Check if the input value is an integer
		for( i = strlen(g) - 1; i >= 0; --i)
		{
			if (g[i] >= '0' && g[i] <= '9')
					y += (int)(g[i]-'0') * (int)pow(10, strlen(g) - 1 - i);
			else{
				cout << "You should input an integer rather than something else. Please input a number: ";
				sig = 1;
				break;
			}
		}
		// sig == 1 takes that the input value is not an integer
		if (sig == 1)
			sig = 0;
			
		// input value is an integer
		else
		{
			if( y < 0 || y > 1000)
				cout << "Please input a number between 1 and 1000. Try again:";
			else if( y > x)
				cout << "It is too large. Try again:";
			else if ( y < x)
				cout << "It is too small. Try again:";
			else {
				cout << "Congradulations! You've got the right answer" << endl;
				break;
			}
		}
		
		y = 0;	// initial the value of y
		cin >> g;
	}	
}
