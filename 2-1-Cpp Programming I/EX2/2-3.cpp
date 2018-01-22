/* --------------------------------------------------------------------------------------
		Exercsie 2: Class and Object
			Test 3: Friend
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
using namespace std;

class Boat;
class Car;
double TotalWeight(Boat& b, Car& c);

class Car
{
	friend double TotalWeight(Boat& b, Car& c);
	private:
		double weight;
	public:
		Car() {weight = 0;};
		Car(double w)
			{weight = w;};
};

class Boat
{
	friend double TotalWeight(Boat& b, Car& c);
	private:
		double weight;
	public:
		Boat() 
			{weight = 0;};
		Boat(double w)
			{weight = w;};
};

double TotalWeight(Boat& b, Car& c)
{
	return b.weight + c.weight;
}

int main()
{
	int b, c;
	cout << "Please input the weight of the car:";
	cin >> c;
	cout << "Please input the weifht of the boat";
	cin >> b;
	
	Car car(c);
	Boat boat(b);
	cout << "The total weight of the car and the boat is: " << TotalWeight(boat, car) << endl;
	return 0;
}
