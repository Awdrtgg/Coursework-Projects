/* --------------------------------------------------------------------------------------
		Exercsie 3: Class and Object
			Test 3: Matrix 3
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>

using namespace std;

class Matrix
{
	friend void MatrixAnd(Matrix& c, Matrix& a, Matrix& b);
	friend void MatrixMinus(Matrix& c, Matrix& a, Matrix& b);
	private:
		int rows, lines;
		int **matrix;
		
	public:
		Matrix();
		Matrix(int l, int r);
		Matrix(Matrix & m);
		~Matrix();
		void Input();
		void Output();
};


Matrix::Matrix()
{
	lines = 4;
	rows = 5;
	matrix = new int*[4];
	for(int i = 0; i < 4; ++i)
		matrix[i] = new int[5];
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 5; ++j)
			matrix[i][j] = 0;
}

Matrix::Matrix(int l, int r)
{
	rows = l;
	lines = r;
	matrix = new int*[r];
	for (int i = 0; i < r; ++i)
		matrix[i] = new int[l];
	
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 5; ++j)
			matrix[i][j] = 0;
}

Matrix::Matrix(Matrix & m)
{
	lines = m.lines;
	rows = m.rows;
	
	// new the matrix
	matrix = new int*[4];
	for(int i = 0; i < 4; ++i)
		matrix[i] = new int[5];
		
	// copy the value of matrix
	for (int i = 0; i < lines; ++i)
		for (int j = 0; j < rows; ++j)
			matrix[i][j] = m.matrix[i][j];
}

Matrix::~Matrix()
{	
	for( int i = 0; i < lines; ++i)
		delete []matrix[i];
	delete []matrix;
}

void Matrix::Input()
{
	for( int i = 0; i < lines; ++i)
		for( int j = 0; j < rows; ++j)
			cin >> matrix[i][j];
}

void Matrix::Output()
{
	for( int x = 0; x < lines; ++x){
		for( int y = 0; y < rows; ++y)
			cout << matrix[x][y] << "  ";
		cout << endl;
	}
	cout << endl;
}

void MatrixAnd(Matrix& c, Matrix& a, Matrix& b)
{
	if (a.lines == b.lines && a.rows == b.rows)
	{
		for (int i = 0; i < a.lines; ++i)
			for (int j = 0; j < a.rows; ++j)
			{
				c.matrix[i][j] = a.matrix[i][j] + b.matrix[i][j];
			}
	}
	else
		cout << "The matrixes' number of lines or rows doesn't match." << endl;
}


void MatrixMinus(Matrix& c, Matrix& a, Matrix& b)
{
	if (a.lines == b.lines && a.rows == b.rows)
	{
		for (int i = 0; i < a.lines; ++i)
			for (int j = 0; j < a.rows; ++j)
			{
				c.matrix[i][j] = a.matrix[i][j] - b.matrix[i][j];
			}
	}
	else
		cout << "The matrixes' number of lines or rows doesn't match." << endl;
}

int main()
{
	int lines, rows;
	// input the matrix A
	cout << "Please input the lines and rows of matrix A:" << endl;
	cin >> lines >> rows;
	cout << "Please input the values of A:" << endl;
	Matrix A(lines, rows);
	A.Input();
	// input the matrix B
	cout << "Please input the lines and rows of matrix B:" << endl;
	cin >> lines >> rows;
	cout << "Please input the values of B:" << endl;
	Matrix B(lines, rows);
	B.Input();
	
	// initial matrix C1, C2
	Matrix C1(lines, rows), C2(lines, rows);
	
	// operate and output C1 & C2
	cout << "A3 = A1 + A2.here are the values of A3:" << endl;
	MatrixAnd(C1, A, B);
	C1.Output();
	cout << "A3 = A1 - A2.here are the values of A3:" << endl;	
	MatrixMinus(C2, A, B);
	C2.Output();
	
}
