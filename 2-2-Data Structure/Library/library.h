#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <cstdarg>
#include <utility>

using namespace std;

//----------------------- Operation --------------------------------

#define SUCCESS 0
#define SYN_ERROR -2
#define FAIL -1
#define NONE -1
//#define BOOK_NOT_FOUND 2
#define BOOK_NOT_LEFT 3
#define NO_ACCESS 4
#define NO_LOGIN -3
#define OVER_TIME 6
#define DISHONESTY 7
#define FALSE_PASSWORD 8
#define FALSE_ACCOUNT 9
#define ACCOUNT_EXIST 10
#define BOOK_EXIST 11
#define USER_NOT_FOUND 12
//#define FUZZY_MATCH 13
//#define EXACT_MATCH 14
#define PURCHASE_NEW 15
#define PURCHASE_OLD 16
#define BOOK_OVER_LIMIT 17
#define BOOK_ALREADY_BORROWED 18
#define BOOK_ALREADY_RESERVED 19

#define HEX_MODE 0
#define STRING_MODE 1

#define DEFAULT_KEEP_TIME 30

//----------------------- USER TYPE --------------------------------

#define STUDENT 0
#define TEACHER 1

//----------------------- USER LIMIT --------------------------------

#define STU_LIMIT 3
#define TEACHER_LIMIT 5
#define BORROW_LIMIT 60
#define ORDER_LIMIT 80

//----------------------- CONSTANT --------------------------------

// FILE COMMON
#define LENGTH_NUM 6
#define LENGTH_TIME 6

// FILE BOOK
#define LENGTH_BOOK_NAME 100
#define LENGTH_BOOK_PUBLISHER 40
#define LENGTH_BOOK_WRITER 40
#define LENGTH_BOOK_RATING 3
#define LENGTH_BOOK_KEY_WORD 48
#define LENGTH_BOOK_ISBN 13
#define LENGTH_BOOK_TIME 14
#define LENGTH_BOOK_IMG 45
#define LENGTH_BOOK_STAT 16
#define LENGTH_BOOK_INTRO 2558

#define LENGTH_BOOK_LINE 3072

// FILE USER & ADMIN
#define LENGTH_REAL_NAME 20
#define LENGTH_STAFF_NAME 10
#define LENGTH_STAFF_PASSWORD 16
#define LENGTH_USER_SCHOOL 40

#define LENGTH_USER_LINE 308
#define LENGTH_ADMIN_LINE 29

// OTHERS
#define EXTEND_TIME 15

#define MS_PER_DAY 500

#define MAX_PER_PAGE 20

#define MAX_KEYWORD_NUM 3

#define MAX_BOOK_NUM 64

#define MAX_ORDER_NUM 3

#define RESERVE_TIME 5

#define BORROW_RANK 5

//--------------------- CLASS DECLARE --------------------------

class Book;
class Staff;
class User;
class Library;

//----------------------- TIME --------------------------------

class Time
{
public:
    int time;
    Library *l;

    Time();
    Time(int time);
    ~Time() {}
    Time(const Time &t) { time = t.time; }

    int year_real() { return ((time / 365) + 2017); }
    int year_relat() { return (time / 365); }
    int month();
    int day();
    string date();
    void NextDay();
    void Save();

    int operator+(const int &t) { return (time + t); }
    int operator-(const int &t) { return (time - t); }
    bool operator==(const int &t) { return (time == t); }
    bool operator<(const int &t) { return (time < t); }
    bool operator>(const int &t) { return (time > t); }
    int operator+(const Time &t) { return (time + t.time); }
    int operator-(const Time &t) { return (time - t.time); }
    bool operator==(const Time &t) { return (time == t.time); }
    bool operator<(const Time &t) { return (time < t.time); }
    bool operator>(const Time &t) { return (time > t.time); }

    operator int() { return time; }
};

extern Time* currentTime;

//----------------------- Utils --------------------------------

class StringParse
{
public:
    vector<string> str;
    StringParse(string s, string delim, int len_limit = 0, ...);
    ~StringParse();
    StringParse(const StringParse &s);
};

class Search_Result
{
public:
    vector<Book*> current_book;
    vector<int> book;
    int current_page;

    Search_Result();
    Search_Result(vector<int> b);
    Search_Result(int b);
    ~Search_Result();
    Search_Result(const Search_Result &s);

    int NextPage();
    int FirstPage();
    int PreviousPage();
    int LastPage();
    int JumpPage(int page);
    void Fresh();

    operator int();
};

class KeyTree
{
public:
    KeyTree(string key, int value);
    KeyTree() {}
    ~KeyTree();
    KeyTree(const KeyTree &k);

    void insert(string key, int value);
	void erase(string key, int value);
    Search_Result find(string key);

private:
    map<char, KeyTree> children;
    vector<int> result;
};

class KeyWord
{
public:
    KeyWord();
    KeyWord(string keyword, int mode); // HEX_MODE & STRING_MODE
    ~KeyWord();
    KeyWord(const KeyWord &k);

    string ToFile();
    int ToInt();
    operator string();
    KeyWord operator+=(KeyWord &k);

private:
    unsigned long long xx[MAX_KEYWORD_NUM]; //0 is the lowest part
};

extern vector<string>keywords;

class AES
{

};

//----------------------- Book --------------------------------

class Book
{
public:
    string name, org_name, publisher, writer, translator, publish_time, img_url, intro;
    KeyWord key_word;
    float rating;
    Time purchase_time;
    int page_num, left_num, total_num, inter_num, reserve_num, reserving_num;
    unsigned long long ISBN, book_left;

    Book() {};
    Book(int index); // read from the file
    Book(string line, int index); // new a book with info in line
    ~Book();
    Book(const Book &b);
    bool operator==(Book &b);
    Book operator+=(Book &b);

    void Save();

    void Display();
    int Borrow();
    int Return(int book_num);
    int Reserve();
    int ReserveCancel();
    int Reserving();
    int ReservingCancel();
    int CheckReserve();
	int Edit(string line);

    int get_index() { return inter_num; }
    unsigned long long get_ISBN() { return ISBN; }
    int get_left_num() { return left_num; }
    int get_v1_num() { return left_num - reserving_num; };
    int get_v2_num() { return left_num - reserve_num - reserving_num; };
    string get_name() { return name; }
    string get_org_name() { return org_name; }
    string get_publisher() { return publisher; }
    string get_writer() { return writer; }
    string get_translator() { return translator; }
    KeyWord get_keyword() { return key_word; };
};

class Borrowed_Book
{
public:
    Time start_time;
    int keep_time;
    Book* book;
    int book_num;

    Borrowed_Book(Book &b, int keep_timea); // operate in the ram
    Borrowed_Book(int book_index, int book_num, int start_time, int keep_time); // read from the file
    ~Borrowed_Book();
    Borrowed_Book(const Borrowed_Book &b);

    int Return();
    int Extend(int extend_time); // time!
    int OverDue();
};

class Reserved_Book
{
public:
    Time reserve_time, v_time;
    int book;
    User* user;

    Reserved_Book();
    Reserved_Book(int index, int time, int v_time, User &u);
    Reserved_Book(Book &b, User &u);
    ~Reserved_Book();
    Reserved_Book(const Reserved_Book &b);

    int Cancel();

    operator Book();
    operator int();
    bool operator==(const Reserved_Book &b);
    bool operator<(const Reserved_Book &b);
};

//----------------------- Staff --------------------------------

class Staff
{
protected:
    //int honesty;

public:
    int index;
    int honesty;
    string user_name, password;

    Staff(string line);
    Staff() {}
    ~Staff();
    Staff(const Staff &s);

    int Check(string password);
    void ChangePassWord(string line);
	int Edit(string line);

    bool operator==(const Staff &s) { return this->user_name == s.user_name; };
};

class Administrator :public Staff
{
public:
    Administrator(string line);
    Administrator() {}
    ~Administrator();
    Administrator(const Administrator &a);

    int Purchase(Search_Result &s, Book &b);
    void Save();
};

class User :public Staff
{
public:
    string school, real_name;
    int type;

    vector<Borrowed_Book> borrowed_book;
    vector<Reserved_Book> reserved_book;
    vector<Reserved_Book> v_reserved_book;

    User(string line, int index);
    User(int index);
    User() {}
    ~User();
    User(const User &u);

    void Save();

    int Borrow(Book &b, int keep_time);
    int Borrow(Reserved_Book &b, int keep_time);
    int Return(Book &b);
    int Reserve(Book &b);
    int ReserveCancel(Reserved_Book &b);
    int Extend(Book &b, int extend_time);
	int Edit(string line);
};


//----------------------- Library --------------------------------

class Library
{
protected:
    map<string, int> user;
    map<string, Administrator> admin;

    KeyTree name, publisher, writer;
    //vector<vector<int>> keyword;
    vector<int> keyword[145];
    map<long long, int> isbn;
	map<int, int> borrow_stat;

public:
    int book_size, user_size, admin_size;
    User* currentUser;
    Administrator* currentAdmin;

    Library();
    ~Library();
    Library(const Library &l);

    void TimeMachine();
    void CheckReserve();

    int Borrow(Book &b, int keep_time);
    int Borrow(Reserved_Book &b, int keep_time);
    int Return(Book &b);
    int Reserve(Book &b);
    int ReserveCancel(Reserved_Book &b);
    int Extend(Book &b, int extend_time);

    int UserLogin(string username, string password);
    int AdminLogin(string username, string password);
    void UserLogout();
    void AdminLogout();
    int UserRegister(string line);
    int AdminRegister(string line);
    int UserChangePassword(string line);
    int AdminChangePassword(string line);
	int UserEdit(User &u, string line);
	int BookEdit(Book &u, string line);
	vector<Book> BorrowRank();

    int Purchase(string line);
    vector<User> CurrentState();

    Search_Result Search(string type, string value);
    Search_Result Search(string type, long long value);

    // somehow interact with js
};
