/* --------------------------------------------------------------------------------------
		Exercsie 4:  Inheritance and Derivation
			Test 1: Shape 1
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#define pai 3.14159

using namespace std;

class Shape
{	
	public:
		Shape()
			{ cout << "Using Shape's constructor." << endl;	system("pause"); }
		~Shape()
			{ cout << "Using Shape's destructor." << endl;	system("pause"); }
		float SquareCalculate();
};

class Rectangle: public Shape
{
	protected:
		float a, b;
	public:
		Rectangle();
		Rectangle(float aa, float bb);
		~Rectangle()
			{ cout << "Using Rectangle's destructor." << endl;	system("pause"); }
		float SquareCalculate()
			{ return a*b;}
};

Rectangle::Rectangle()
{
	a = b = 1;
	cout << "Using Rectangle's constructor." << endl;
	system("pause");
}

Rectangle::Rectangle(float aa, float bb)
{
	a = aa;
	b = bb;
	cout << "Using Rectangle's constructor." << endl;
	system("pause");
}

class Circle: public Shape
{
	protected:
		float r;
	public:
		Circle()
			{ r = 1; cout << "Using Circle's constructor." << endl; system("pause"); }
		~Circle()
			{ cout << "Using Circle's destructor." << endl;	system("pause"); }
		float SquareCalculate()
			{ return pai*r*r;}
};

class Quadrate: public Rectangle
{
	public:
		Quadrate()
			{ cout << "Using Quadrate's constructor." << endl; system("pause"); }
		Quadrate(float aa)
			{ a = b = aa; cout << "Using Quadrate's constructor." << endl; system("pause"); }
		~Quadrate()
			{ cout << "Using Quadrate's destructor." << endl; system("pause"); }
};

int main()
{
	Quadrate q(2.0);
	cout << q.SquareCalculate() << endl;
}
