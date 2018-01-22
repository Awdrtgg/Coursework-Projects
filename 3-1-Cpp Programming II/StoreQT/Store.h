/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		Store.h
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Define the classes of Account, Merchandise, Store and their rehir classes.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

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
#include <numeric>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

// --------------------------- CONST --------------------------------

const string admin_bank;
const string admin_card;
const string admin_password;

const int NUM_ATTR_MERCH = 5;
const int NUM_ATTR_BOOK = 4;
const int NUM_ATTR_FOOD = 2;
const int NUM_ATTR_CLOTHES = 3;
const int MAX_PER_PAGE = 20;

const int SUCCESS = 0;
const int NO_LOGIN = -1;
const int FAIL = 1;
const int NO_ACCOUNT = -2;
const int FALSE_PASS = -3;
const int ACCOUNT_EXIST = -4;
const int INPUT_ERROR = -5;


// --------------------- CLASS DELARATION ---------------------------

class Store;

// -------------------------- MERCH ------------------------------ 

class Merchandise {
protected:
	string kind;
	string name;
	float origin_price;
	int amount;
	string intro;
	sql::Connection *con;

public:
	Merchandise(vector<string> v, sql::Connection *con);
	~Merchandise() {};
	Merchandise(const Merchandise &m);

	virtual float GetPrice() = 0;
	virtual int Add() = 0;
	virtual int Edit() = 0;
	virtual string Where() = 0;
};

class Book : public Merchandise {
private:
	float cutoff;
	string isbn;
	string writer;
	string publisher;

public:
	Book(vector<string> v, sql::Connection *con);
	~Book() {};
	Book(const Book &b);

	string Where();
	float GetPrice();
	int Add();
	int Edit();
};

class Food : public Merchandise {
private:
	float cutoff;
	string exp_date;

public:
	Food(vector<string> v, sql::Connection *con);
	~Food() {};
	Food(const Food &f);

	string Where();
	float GetPrice();
	int Add();
	int Edit();
};

class Clothes : public Merchandise {
private:
	float cutoff;
	string material;
	string size;

public:
	Clothes(vector<string> v, sql::Connection *con);
	~Clothes() {};
	Clothes(const Clothes &c);

	string Where();
	float GetPrice();
	int Add();
	int Edit();
};

// ------------------------- ACCOUNT ------------------------------

class Account {
protected:
	sql::Connection *con;
	string password;

public:
	string user_id;

	Account(string user_id, sql::Connection *con);
	~Account();
	Account(const Account &a);

	int LogIn(string password);
};

class User : public Account {
public:
	User(string user_id, sql::Connection *con);
	~User();
	User(const User &u);

	int ChangePass(string old_password, string new_password);
	string Where();
};

class Admin : public Account {
public:
	Admin(string user_id, sql::Connection *con);
	~Admin();
	Admin(const Admin &a);
};

// ---------------------------- STORE --------------------------------

class Store {
protected:
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	User *current_user;
	Admin *current_admin;

public:
	string name;

	Store(string site, string user, string password);
	~Store();
	Store(const Store &s);

	// Store operations
	vector<string> StoreList();
	sql::ResultSet* CheckRepeat(string databse, string table, string id, string value);
	vector<vector<string>> Search(string kind, string condition, string value, int exact_match = 0);

	// User operations
	int UserRegister(string user_id, string password);
	int UserLogIn(string user_id, string password);
	int UserLogOut();
	int UserChangePass(string old_password, string new_password);

	int Favour(string kind, string name);
	int CancelFavour(string kind, string name);
	int Order(string kind, string name, int amount);
	int CancelOrder(string kind, string name);
	int CancelCard(string bank, long long card);
	int Pay(string kind, string name, int amount);
	vector<vector<string>> CheckFavour();
	vector<vector<string>> CheckOrder();
	vector<vector<string>> CheckBankCard();
	int BindBankCard(string bank_name, long long card_number);
	sql::ResultSet* HistoryList();

	// Admin operations
	int AddMerchan(vector<string> v);
	int EditMerchan(vector<string> v);
	int SetCutOff(string kind, float cutoff);

	int AdminLogIn(string user_id, string password);
	int AdminLogOut();


	bool IfUser() { return !(this->current_user == NULL); };
	bool IfAdmin() { return !(this->current_admin == NULL); };
	void TimeMachine();
};

// -------------------------- STORE OBJ -------------------------------

vector<string> StringParse(string s, string delim);
extern Store store;