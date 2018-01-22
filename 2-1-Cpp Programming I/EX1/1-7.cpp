/* --------------------------------------------------------------------------------------
		Exercsie 1: Simple C++ Programming
			Test 7: Legendre Polynomials
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
using namespace std;

double Lpoly(double x, double n)
{
	if (n == 0.0)
		return 1;
	else if (n == 1.0)
		return x;
	else
		return ((2*n-1)*x*Lpoly(x, n-1) - (n-1)*Lpoly(x, n-2))/n;
}

int main()
{
	double x, n;
	cout << "Please input the initial value x and n in order:" << endl;
	cin >> x >> n;
	cout << "The result is: " << Lpoly(x, n) << endl;
}
