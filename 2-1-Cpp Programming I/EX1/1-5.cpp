/* --------------------------------------------------------------------------------------
		Exercsie 1: Simple C++ Programming
			Test 5: LCM & GCD
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
using namespace std;

int gcd( int x, int y)
{
	int i = 1;
	while ( x % i == 0 && y % i == 0)
		i++;
	return --i;
}

int lcm( int x, int y)
{
	int i = x;
	while ( i % x != 0 || i % y != 0)
		i++;
	return i;
}

int main()
{
	int x, y;
	cout << "Please input two integers:" << endl;
	cin >> x >> y;
	cout << "The greatest multiple divisor of two input values is " << gcd(x, y) << endl;
	cout << "The least common mutiple of two input values is " << lcm(x, y) << endl;
}
