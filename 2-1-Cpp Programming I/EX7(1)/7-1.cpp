/* --------------------------------------------------------------------------------------
		Exercsie 7: C++ Programming
			Test 1: Telephone Book
		Programmer: Xiao Yunming
	********************************************************************************
	
----------------------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>


#define MAX_BOOK 100
#define PHONE_NUMBER 11
#define MAX_NAME 20 

using namespace std;

class PhoneBook
{
	private:
		char **number;
		char **name;
		int length;
		
	public:
		PhoneBook();
		PhoneBook(const char *bookName);	// get the record from a file
		PhoneBook(PhoneBook & p);
		~PhoneBook();
		int Search(char *sName);
		void Add(char *aName, char *aNumber);
		void Delete(char *dName);
		void Save(char *bookName);
		void ReadFromFile(char *bookName);
		void Cover(char *cName);
		void Show();
		void Clear();
}; 

PhoneBook::PhoneBook()
{
	number = new char*[MAX_BOOK];
	for (int i = 0; i < MAX_BOOK; ++i) {
		number[i] = new char[PHONE_NUMBER];
		for (int j = 0; j < PHONE_NUMBER; ++j)
			number[i][j] = '\0';		// initialize
	}
	name = new char*[MAX_BOOK];
	for (int i = 0; i < MAX_BOOK; ++i) {
		name[i] = new char[MAX_NAME];
		for (int j = 0; j < MAX_NAME; ++j)
			name[i][j] = '\0';			// initianlize
	}
	
	length = 0;
}

PhoneBook::PhoneBook(const char *bookName)
{
	// this is the same as the funcion PhoneBook()
	number = new char*[MAX_BOOK];
	for (int i = 0; i < MAX_BOOK; ++i) {
		number[i] = new char[PHONE_NUMBER];
		for (int j = 0; j < PHONE_NUMBER; ++j)
			number[i][j] = '\0';
	}
	name = new char*[MAX_BOOK];
	for (int i = 0; i < MAX_BOOK; ++i) {
		name[i] = new char[MAX_NAME];
		for (int j = 0; j < MAX_NAME; ++j)
			name[i][j] = '\0';
	}
	length = 0;
	
	// take the file
	ifstream f;
	f.open(bookName, ios::in);
	f.seekg(0);
	
	int i = 0, j = 0;
	char temp[2] = "";			// the array is just for convinient for strcat()
	while ( (temp[0] = f.get()) != EOF && i < MAX_BOOK && j < MAX_BOOK)
	{
		// 'i' is the number of name have got while 'j' is the number of numbers.
		// Once i is greater than j, that means we have actually get a new name
		// and the next shall be a number. Else i is equal to j takes that we shall
		// take a new record
		if (i > j)
		{
			if (temp[0] == '\n')
				++j;
			else
				strcat(number[j], temp); 
		}
		else
		{
			if (temp[0] == '\n')
				++i;
			else
				strcat(name[i], temp); 
		}
	}
	length = i;
	
	f.close();
}

PhoneBook::PhoneBook(PhoneBook & p)
{
	for (int i = 0; i < p.length; ++i)
	{
		strcpy(name[i], p.name[i]);
		strcpy(number[i], p.number[i]);
	}
	length = p.length;
}

PhoneBook::~PhoneBook()
{
	for (int i = 0; i < MAX_BOOK; ++i)
	{
		delete []name[i];
		delete []number[i];
	}
	delete []name;
	delete []number;
} 

int PhoneBook::Search(char *sName)
{
	// compare the name with each name in the book
	for (int i = 0; i < length; ++i)
	{
		if (strcmp(name[i], sName) == 0)
		{
			cout << "The Phone Number of " << sName << " is: " << number[i] << endl;
			return ++i;
		}
	}
	return 0;
}

void PhoneBook::Add(char *aName, char *aNumber)
{
	// check if the book is full
	if (length < MAX_BOOK){
		strcpy(name[length], aName);
		strcpy(number[length], aNumber);
	}
	++length;
}

void PhoneBook::Delete(char *dName)
{
	int i = 0;
	// Search the name and locate
	for (; i < length; ++i)
		if (strcmp(name[i], dName) == 0)
			break;
	
	if (i >= length)
		cout << "Sorry, the name is not in the book." << endl;
	else
	{
		// make every record move forward to complement the deleted one
		for (; i < length; ++i)
		{
			strcpy(name[i], name[i+1]);
			strcpy(number[i], number[i+1]);
		}
		--length;
		cout << "Record deleted!" << endl;
	}
}

void PhoneBook::Save(char *bookName)
{
	ofstream f;
	f.open(bookName, ios::out|ios::app);
	
	for (int i = 0; i < length; ++i)
	{
		f << name[i] << endl;
		f << number[i] << endl;
	}
	f.close();
}

void PhoneBook::ReadFromFile(char *bookName)
{
	ifstream f;
	f.open(bookName, ios::in);
	f.seekg(0);
	
	// all the same as in the function PhoneBook(const char *bookName)
	int i = 0, j = 0;
	char temp[2] = "";			// the array is just for convinient for strcat()
	while ( (temp[0] = f.get()) != EOF && i < MAX_BOOK && j < MAX_BOOK)
	{
		if (i > j)
		{
			if (temp[0] == '\n')
				++j;
			else
				strcat(number[j], temp); 
		}
		else
		{
			if (temp[0] == '\n')
				++i;
			else
				strcat(name[i], temp); 
		}
	}
	length = i;
	
	f.close();
}

void PhoneBook::Cover(char *cName)
{
	int i = 0;
	// Search and locate the record of cName
	for (; i < length; ++i)
	{
		if (strcmp(name[i], cName) == 0)
			break;
	}
	cout << "Please input the new number: ";
	cin >> number[i];	// change the number[i]
	strcpy(name[i], cName);	// change the name[i]
}

void PhoneBook::Show()
{
	// print the Name and Number
	cout << setiosflags(ios::left) << setw(MAX_NAME) << "Name" << " ";
	cout << setiosflags(ios::left) << setw(PHONE_NUMBER) << "Number" << endl;
	for (int i = 0; i < length; i++)
	{
		cout << setiosflags(ios::left) << setw(MAX_NAME) << name[i] << " ";
		cout << setiosflags(ios::left) << setw(PHONE_NUMBER) << number[i] << endl;
	}
	cout << "There are totally " << length << " records." << endl;
}

void PhoneBook::Clear()
{
	// make all the elements in the book '\0'
	for (int i = 0; i < MAX_BOOK; ++i)
	{
		for (int j = 0; j < PHONE_NUMBER; ++j)
			number[i][j] = '\0';
		for (int k = 0; k < MAX_NAME; ++k)
			name[i][k] = '\0';
	}
	length = 0;
}

void mainShow()
{
	cout << "1: Search the phone number with the name;" << endl;
	cout << "2: Show all the information on current phone book;" << endl;
	cout << "3: Add a number to the Number Book;" << endl;
	cout << "4: Delete a information in the Number Book;" << endl;
	cout << "5: Save the current number book in a file;" << endl;
	cout << "6: Read the phone book from a book." << endl;
	cout << "0: End the program." << endl << endl;
	cout << "Please input the number for the operation you want: ";
}

int main()
{
	PhoneBook p;
	
	char tempName[MAX_NAME], tempNumber[PHONE_NUMBER];
	int operate, tempInt;
	
	mainShow();
	cin >> operate;
	while(1)
	{
		switch(operate)
		{
			case 1: 
				cout << "Please input the name you want to check: ";
				cin >> tempName;
				if(p.Search(tempName) == 0)
					cout << "There is no records with name " << tempName << "." << endl;
				break;
				
			case 2:
				p.Show();
				break;
				
			case 3:
				cout << "Pease input the new name: ";
				cin >> tempName;
				
				// happened that the new name is in the book already
				if (p.Search(tempName) != 0)
				{
					cout << "Do you want to cover that record? type in 1 for Yes, 2 for No: ";
					cin >> tempInt;
					// input not 1 or 2
					while (tempInt != 1 && tempInt != 2){
						cout << "Please type 1 or 2: ";
						cin >> tempInt;
					}	
					if (tempInt == 1)	p.Cover(tempName);
					else if (tempInt == 2) 	cout << "Return to the main menu" << endl;
					break;
				}
				
				// there's no record being the same name with the new name
				else 
				{
					cout << "Please input the new number(11 elements): ";
					cin >> tempNumber;
					p.Add(tempName, tempNumber);
					break;
				}
				
			case 4:
				cout << "Please input the name you want to delete: ";
				cin >> tempName;
				p.Delete(tempName);
				break;
				
			case 5:
				cout << "Give a name to the current phone book: ";
				cin >> tempName;
				p.Save(tempName);
				break;
				
			case 6:
				cout << "Input the phone book name: ";
				cin >> tempName;
				p.Clear();			// clear the data or strcat will perish it!
				p.ReadFromFile(tempName);
				break;
				
			default:
				cout << "Input error! " << endl;			
		}
		
		cout << endl;
		mainShow();
		cin >> operate;
		if (operate == 0){
			cout << endl;
			break;
		}
	}
}
