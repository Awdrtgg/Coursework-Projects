/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		BankCard.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Realization of the BankCard class.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include "Bank.h"

BankCard::BankCard(string bank, long long card_number, sql::Connection *con) {
	this->bank = bank;
	this->card_number = card_number;
	this->con = con;

	string cmd = "select * from " + bank + " where card_number=" + to_string(card_number);
	sql::Statement *stmt = this->con->createStatement();
	sql::ResultSet *res;
	cout << cmd << endl;
	res = stmt->executeQuery(cmd);
	res->next();
	this->password = res->getString(2);
	this->user_id = res->getString(3);
	this->balance = res->getDouble(4);
	this->credit_line = res->getDouble(5);
}

BankCard::~BankCard() {
	// do nothing
}

BankCard::BankCard(const BankCard &b) {
	this->bank = b.bank;
	this->card_number = b.card_number;
	this->con = b.con;
	this->password = b.password;
	this->user_id = b.user_id;
	this->balance = b.balance;
}

string BankCard::Where() {
	string where = " where ";
	where += "(card_number=" + to_string(this->card_number) + " AND ";
	where += "password=\'" + this->password + "\' AND ";
	where += "user_id=\'" + this->user_id + "\')";
	return where;
}

void BankCard::Refresh() {
	string cmd = "select * from " + bank + " where card_number=" + to_string(this->card_number);
	sql::Statement *stmt = this->con->createStatement();
	sql::ResultSet *res;
	res = stmt->executeQuery(cmd);
	res->next();
	this->password = res->getString(2);
	this->user_id = res->getString(3);
	this->balance = res->getDouble(4);
	this->credit_line = res->getDouble(5);
}

int BankCard::LogIn(string password) {
	if (this->password == password)
		return SUCCESS;
	else
		return FAIL;
}

int BankCard::ChangePass(string old_password, string new_password) {
	if (old_password != this->password) {
		return FAIL;
	}
	if (this->password == new_password) {
		return FAIL;
	}

	string cmd = "update " + this->bank + " set password=\'" + new_password + "\'" + this->Where();
	sql::Statement *stmt = this->con->createStatement();
	cout << cmd << endl;
	stmt->executeUpdate(cmd);
	this->password = new_password;
	return SUCCESS;
}

int BankCard::Save(float amount) {
	this->balance += amount;

	string cmd = "update " + this->bank + " set balance=" + to_string(this->balance) + this->Where();
	sql::Statement *stmt = this->con->createStatement();
	cout << cmd << endl;
	stmt->executeUpdate(cmd);
	return SUCCESS;
}

int BankCard::Withdraw(float amount) {
	if (this->balance < amount) {
		return FAIL;
	}
	this->balance -= amount;

	string cmd = "update " + this->bank + " set balance=" + to_string(this->balance) + this->Where();
	sql::Statement *stmt = this->con->createStatement();
	cout << cmd << endl;
	stmt->executeUpdate(cmd);
	return SUCCESS;
}

int BankCard::Pay(float amount) {
	if (this->balance + this->credit_line < amount) {
		return FAIL;
	}
	else if (this->credit_line < amount) {
		amount -= this->credit_line;
		this->credit_line = 0;
		this->balance -= amount;
	}
	else {
		this->credit_line -= amount;
	}

	string cmd = "update " + this->bank + " set balance=" + to_string(this->balance)
		+ ", credit=" + to_string(this->credit_line) + this->Where();
	sql::Statement *stmt = this->con->createStatement();
	cout << cmd << endl;
	stmt->executeUpdate(cmd);
	return SUCCESS;
}

float BankCard::CheckBalance() {
	return this->balance;
}

float BankCard::CheckCredit() {
	return this->credit_line;
}

string BankCard::HistoryList() {
	// not used yet
	return "history test";
}