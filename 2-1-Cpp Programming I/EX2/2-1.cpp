/* --------------------------------------------------------------------------------------
		Exercsie 2: Class and Object
			Test 1: Point & Rectangle
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#include <math.h>

using namespace std;

class Point
{
	private:
		int x, y;
		
	public:
		Point()	
			{x = 0; y = 0;};
		Point(int xx = 0, int yy = 0)
			{ x = xx; y = yy;};
		Point(Point & p)
			{ 
			x = p.x; y = p.y;
			cout << "using Point's constructor" << endl;	
			};
			
		//~Point()
		//	{ cout << "using Point's destructor" << endl;};	
			
		int GetX()	{ return x;}
		int GetY()	{ return y;}
};

class Rectangle
{
	private:
		Point a, b;
		
	public:
		Rectangle();
		Rectangle(Point aa, Point bb): a(aa), b(bb) { };

		//~Rectangle()
		//	{ cout << "using Rectangle's destructor" << endl;};
		
		int SquareCalculate()
		{
			return abs((a.GetX() - b.GetX())*(a.GetY() - b.GetY()));
		}
};

int main()
{
	int x1, x2, y1, y2, s;
	cout << "Please input the x and y of the top left corner of the rectangle" << endl;
	cin >> x1 >> y1;
	cout << "Then input the x and y of the bottom right corner of the rectangle" << endl;
	cin >> x2 >> y2;
	
	Point pa(x1, y1), pb(x2, y2);
	Rectangle rec(pa, pb);
	
	cout << "The sqaure of the rectangle is " << rec.SquareCalculate() << endl;
}
