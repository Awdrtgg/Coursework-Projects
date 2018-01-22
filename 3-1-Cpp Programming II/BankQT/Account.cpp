/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		Account.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Realization of the Admin class.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include "Bank.h"

Admin::Admin(string name, sql::Connection *con) {
	this->con = con;
	this->name = name;

	string cmd = "select * from admin where name=\'" + this->name + "\'";
	sql::Statement *stmt = this->con->createStatement();
	sql::ResultSet *res;
	cout << cmd << endl;
	res = stmt->executeQuery(cmd);
	while (res->next())
		this->password = res->getString(2);
}

Admin::~Admin() {
	// do nothing
}

Admin::Admin(const Admin &a) {
	this->con = a.con;
	this->name = a.name;
	this->password = a.password;
}

int Admin::LogIn(string password) {
	if (this->password == password)
		return SUCCESS;
	else
		return FAIL;
}