/* --------------------------------------------------------------------------------------
		Exercsie 6: IO stream
			Test 3: IO stream 3
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>

using namespace std;

int main()
{
	char filename[20], filetype[5], f1n[25] = "", f2n[30] = "";
	cout << "Please input the file name:";
	cin >> filename;
	cout << "Please input the type of the file:";
	cin >> filetype;
	
	// get the name of both files f1n and f2n
	strcat(f2n, filename);
	strcat(f2n, "(order)");
	strcat(f2n, ".");
	strcat(f2n, filetype);
	strcat(f1n, filename);
	strcat(f1n, ".");
	strcat(f1n, filetype);
	
	fstream f1, f2;
	f1.open(f1n, ios::in);
	f2.open(f2n, ios::out|ios::app);
	
	f1.seekg(0);	// let the get pointer of f1 go to the beginning
	f2 << "1.";		// write the first number of f2
	
	int i = 2;
	
	char temp;
	while ( (temp = f1.get()) != EOF)
	{
		cout << temp;	// show the f1 in the console
		
		// when f1's get pointer faces a enter, write the order number to f2
		if( temp == '\n')
		{
			f2 << temp << i << ".";		// write down the number of the lines
			++i;
		}
		else
			f2 << temp;
	}
	f1.close();
	f2.close();
}
