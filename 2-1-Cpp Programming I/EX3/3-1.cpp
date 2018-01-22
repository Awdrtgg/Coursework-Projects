/* --------------------------------------------------------------------------------------
		Exercsie 3: Class and Object
			Test 1: Matrix 1
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#define MAX_COLUMN 5
#define MAX_ROW 4
using namespace std;

class Matrix
{
	friend Matrix MatrixMinus(Matrix& A, Matrix& B);
	friend Matrix MatrixAnd(Matrix& A, Matrix& B);
	private:
		int value[MAX_ROW][MAX_COLUMN];
	public:
		Matrix(int a);
		Matrix();
		Matrix(Matrix & m);
		void matrixOutput();
};

Matrix::Matrix(int a)
{	
	if( a == 1)
		for( int x = 0; x < MAX_ROW; ++x)
			for( int y = 0; y < MAX_COLUMN; ++y)
				cin >> value[x][y];
	else if( a == 0)
		for( int x = 0; x < MAX_ROW; ++x)
			for( int y = 0; y < MAX_COLUMN; ++y)
				value[x][y] = 0;
}

Matrix::Matrix()
{	
	for( int x = 0; x < MAX_ROW; ++x)
		for( int y = 0; y < MAX_COLUMN; ++y)
			value[x][y] = 0;
}
	

Matrix::Matrix(Matrix & m)
{
	for( int x = 0; x < MAX_ROW; ++x)
		for( int y = 0; y < MAX_COLUMN; ++y)
			value[x][y] = m.value[x][y]; 
}

void Matrix::matrixOutput()
{
	for( int x = 0; x < MAX_ROW; ++x){
		for( int y = 0; y < MAX_COLUMN; ++y)
			cout << value[x][y] << "  ";
		cout << endl;
	}
	cout << endl;
}


Matrix MatrixAnd(Matrix& A, Matrix& B)
{
	Matrix C;
	for( int x = 0; x < MAX_ROW; ++x)
		for( int y = 0; y < MAX_COLUMN; ++y)
			C.value[x][y] = A.value[x][y] + B.value[x][y];
	return C;
}

Matrix MatrixMinus(Matrix& A, Matrix& B)
{
	Matrix C;
	for( int x = 0; x < MAX_ROW; ++x)
		for( int y = 0; y < MAX_COLUMN; ++y)
			C.value[x][y] = A.value[x][y] - B.value[x][y];
	return C;
}


int main()
{
	cout << "Please input the values of the matrix A1 and A2:" <<endl;
	Matrix A1(1), A2(1), A3;
	cout << "A3 = A1 + A2.here are the values of A3:" << endl;
	A3 = MatrixAnd(A1, A2);
	A3.matrixOutput();
	cout << "A3 = A1 - A2.here are the values of A3:" << endl;	
	A3 = MatrixMinus(A1, A2);
	A3.matrixOutput();
}
