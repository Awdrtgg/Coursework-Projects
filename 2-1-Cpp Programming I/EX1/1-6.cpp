/* --------------------------------------------------------------------------------------
		Exercsie 1: Simple C++ Programming
			Test 6: Fibonacci
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
using namespace std;

int fibonacci(int x)
{
	if (x == 1 || x == 2)
		return 1;
	else
		return fibonacci(x-1)+fibonacci(x-2);
}

int main()
{
	int x;
	cout << "Please input the initial value of Fibonacci:" << endl;
	cin >> x;
	cout << "The Fibonacci of " << x << " is: " << fibonacci(x) << endl;
}
