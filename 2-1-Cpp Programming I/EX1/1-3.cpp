/* --------------------------------------------------------------------------------------
		Exercsie 1: Simple C++ Programming
			Test 3: Enum
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
using namespace std;

enum Ball{ red, yellow, blue, white, black};

class SelectBall
{
	public:
		int x;
		SelectBall(int xx = red) { x = xx;};
		string name()
		{	
			switch(x)
			{
				case red: return "red     "; break;
				case yellow: return "yellow  "; break;
				case blue: return "blue    "; break;
				case white: return "white    "; break;
				case black: return "black    "; break;
				defualt: break;
			}
		}
};

int main()
{
	int n = 125;
	SelectBall a, b, c;
	cout << "There are totally " << n << " solutions:" << endl;
	for(a.x = red; a.x <= black; ++a.x)
		for(b.x = red; b.x <= black; ++b.x)
			for(c.x = red; c.x <= black; ++c.x)
			{
				cout << a.name() << b.name() << c.name() << endl; 
			}
}
