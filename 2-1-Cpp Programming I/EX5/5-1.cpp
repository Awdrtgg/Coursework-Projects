/* --------------------------------------------------------------------------------------
		Exercsie 5: Polymorphism
			Test 1: ++ & -- 
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>

using namespace std;

class Point
{
	private:
		double x, y;
	public:
		Ponit()	
			{ x = 0; y = 0;}
		Point(double xx = 0, double yy = 0)
			{ x = xx; y = yy;}
		Point & operator++()	// ++c
		{
			x++;
			y++;
			return *this;
		}
		Point operator++(int)	// c++
		{
			Point temp = *this;
			++*this;
			return temp;
		}
		Point & operator--()	// --c
		{
			x--;
			y--;
			return *this;
		}
		Point operator--(int)	// c--
		{
			Point temp = *this;
			--*this;
			return temp;
		}
		void Show()
		{
			cout << "(" << x << "," << y << ")" << endl;
		}
};

int main()
{
	Point a(1, 1);
	a++;
	a.Show();
	a--;
	a.Show();
	--a;
	a.Show();
	++a;
	a.Show();
}
