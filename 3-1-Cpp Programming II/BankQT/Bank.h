/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		Bank.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Define the classes of Admin, BankCard and Bank.
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

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


using namespace std;

// --------------------------- CONST --------------------------------

const int SUCCESS = 0;
const int NO_LOGIN = -1;
const int FAIL = 1;
const int NO_ACCOUNT = -2;
const int FALSE_PASS = -3;


// --------------------- CLASS DELARATION ---------------------------

class Bank;

// -------------------------- BANKCARD ------------------------------ 

class BankCard {
protected:
	sql::Connection *con;
	string bank;
	long long card_number;
	string password;
	string user_id;
	float balance;
	float credit_line;

public:
	BankCard(string bank, long long card_number, sql::Connection *con);
	~BankCard();
	BankCard(const BankCard &b);

	string Where();
	void Refresh();

	int LogIn(string password);
	int ChangePass(string old_password, string new_password);
	int Save(float amount);
	int Withdraw(float amount);
	int Pay(float amount);
	float CheckBalance();
	float CheckCredit();
	string HistoryList();
};

// --------------------------- ADMIN --------------------------------

class Admin {
private:
	sql::Connection *con;
	string name;
	string password;

public:
	Admin(string name, sql::Connection *con);
	~Admin();
	Admin(const Admin &a);

	int LogIn(string password);
};

// ---------------------------- BANK --------------------------------

class Bank {
protected:
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	BankCard *current_card;
	Admin *current_admin;

public:
	string name;

	Bank(string site, string user, string password);
	~Bank();
	Bank(const Bank &b);

	// Bank operations
	vector<string> BankList();
	int SetBank(string name);

	// Card operations
	long long CardRegister(string password, string user_id);
	int CardLogIn(long long card_number, string password);
	int CardLogOut();
	int CardChangePass(string old_password, string new_password);

	int Save(float amount);
	int Withdraw(float amount);
	int Transfer(string bank, long long bank_card_account, float amount);
	int Pay(string bank, long long bank_card_account, string password, float amount);
	float CheckBalance();
	float CheckCredit();
	string HistoryList();

	// Admin operations
	int CreateBank(string name);

	int AdminLogIn(string name, string password);
	int AdminLogOut();

	// Some useful tips
	bool IfAdmin() { return !(this->current_admin == nullptr); };
	bool IfUser() { return !(this->current_card == nullptr); };
};

// -------------------------- BANK OBJ -------------------------------

vector<string> StringParse(string s, string delim);
extern Bank bank;