/* --------------------------------------------------------------------------------------
		Exercsie 3: Class and Object
			Test 2: Matrix 2
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#define MAX_COLUMN 5
#define MAX_ROW 4
using namespace std;

int** MatrixInit()
{
	int **p = new int*[MAX_ROW];
	for(int i = 0; i < MAX_COLUMN; ++i)
		p[i] = new int[MAX_COLUMN];
		
	for( int x = 0; x < MAX_ROW; ++x)
		for( int y = 0; y < MAX_COLUMN; ++y)
			cin >> p[x][y];
	return p;
}


void MatrixOutput(int **p)
{
	for( int x = 0; x < MAX_ROW; ++x){
		for( int y = 0; y < MAX_COLUMN; ++y)
			cout << p[x][y] << "  ";
		cout << endl;
	}
	cout << endl;
}

int** MatrixAnd(int** A, int** B)
{
	
	int **C = new int*[MAX_ROW];
	for(int i = 0; i < MAX_COLUMN; ++i)
		C[i] = new int[MAX_COLUMN];
		
	for( int x = 0; x < MAX_ROW; ++x)
		for( int y = 0; y < MAX_COLUMN; ++y)
			C[x][y] = A[x][y] + B[x][y];
	return C;
}

int** MatrixMinus(int** A, int** B)
{
	int** C;
	C = new int*[MAX_ROW];
	for(int i = 0; i < MAX_COLUMN; ++i)
		C[i] = new int[MAX_COLUMN];
	
	for( int x = 0; x < MAX_ROW; ++x)
		for( int y = 0; y < MAX_COLUMN; ++y)
			C[x][y] = A[x][y] - B[x][y];
	return C;
}

void MatrixDelete(int **p)
{
	for( int i = 0; i < MAX_ROW; ++i)
		delete []p[i];
	delete []p;
}


int main()
{
	int **A, **B, **C1, **C2;
	cout << "Please input the values of the matrix A1 and A2:" <<endl;
	A = MatrixInit();
	B = MatrixInit();
	cout << "A3 = A1 + A2.here are the values of A3:" << endl;
	C1 = MatrixAnd(A, B);
	MatrixOutput(C1);
	cout << "A3 = A1 - A2.here are the values of A3:" << endl;	
	C2 = MatrixMinus(A, B);
	MatrixOutput(C2);
	
	MatrixDelete(A);
	MatrixDelete(B);
	MatrixDelete(C1);
	MatrixDelete(C2);
}
