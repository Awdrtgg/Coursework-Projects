/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		Store.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Complete the methods of class Store.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include "Store.h"

// ------------------------- Account -------------------------

Account::Account(string user_id, sql::Connection *con) {
	this->user_id = user_id;
	this->con = con;
}

Account::~Account() {
	// do nothing
}

Account::Account(const Account &a) {
	this->user_id = a.user_id;
	this->password = a.password;
	this->con = a.con;
}

int Account::LogIn(string password) {
	if (this->password == password)
		return SUCCESS;
	else
		return FAIL;
}

// ------------------------- User -------------------------

User::User(string user_id, sql::Connection *con) : Account(user_id, con) {
	con->setSchema("account");
	string cmd = "select * from user where user_id=\'" + user_id + "\';";
	sql::Statement *stmt = this->con->createStatement();
	sql::ResultSet *res;
	cout << cmd << endl;
	res = stmt->executeQuery(cmd);
	res->next();
	this->password = res->getString(2);
}

User::~User() {
	// do nothing
}

User::User(const User &u) : Account(u) {

}

int User::ChangePass(string old_password, string new_password) {
	if (old_password != this->password) {
		return FAIL;
	}
	if (this->password == new_password) {
		return FAIL;
	}

	con->setSchema("account");
	string cmd = "update user set password=\'" + new_password + "\'" + this->Where();
	sql::Statement *stmt = this->con->createStatement();
	cout << cmd << endl;
	stmt->executeUpdate(cmd);
	this->password = new_password;
	return SUCCESS;
}

string User::Where() {
	string where = " where ";
	where += "(user_id=\'" + this->user_id + "\' AND ";
	where += "password=\'" + this->password + "\')";
	return where;
}

// ------------------------- Admin -------------------------

Admin::Admin(string user_id, sql::Connection *con) : Account(user_id, con) {
	con->setSchema("account");
	string cmd = "select * from admin where user_id=\'" + user_id + "\';";
	sql::Statement *stmt = this->con->createStatement();
	sql::ResultSet *res;
	cout << cmd << endl;
	res = stmt->executeQuery(cmd);
	res->next();
	this->password = res->getString(2);
}

Admin::~Admin() {
	// do nothing
}

Admin::Admin(const Admin &a) : Account(a) {

}