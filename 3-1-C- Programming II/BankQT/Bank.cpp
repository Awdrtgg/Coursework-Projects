/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		Bank.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Realization of the Bank class.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include "Bank.h"

// --------------------------------------------- Basic Op ---------------------------------------------

Bank::Bank(string site = "tcp://127.0.0.1:3306", string user = "store", string password = "store") {
	try {
		this->driver = get_driver_instance();
		this->con = driver->connect(site, user, password);
		this->con->setSchema("bank");
		this->stmt = this->con->createStatement();
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	// including all the cards		
	this->current_admin = NULL;
	this->current_card = NULL;
	this->name = "";
}

Bank::~Bank() {
	delete this->con;
	delete this->stmt;
}

Bank::Bank(const Bank &b) {
	this->name = b.name;
	this->driver = b.driver;
	this->con = b.con;
	this->stmt = b.stmt;
	this->current_card = b.current_card;
	this->current_admin = b.current_admin;
}

// --------------------------------------------- Bank Op ---------------------------------------------

vector<string> Bank::BankList() {
	sql::ResultSet *res;
	res = this->stmt->executeQuery("show tables;");
	vector<string> temp;

	while (res->next()) {
		if (res->getString(1) == "admin")
			continue;
		temp.push_back(res->getString(1));
	}
	return temp;
}

int Bank::SetBank(string name) {
	if (name == "") {
		this->name = "";
		return SUCCESS;
	}

	vector<string> names = this->BankList();
	vector<string>::iterator r = std::find(names.begin(), names.end(), name);
	if (r == names.end()) {
		return FAIL;
	}

	this->name = name;
	return SUCCESS;
}

// --------------------------------------------- Card Op ---------------------------------------------

long long Bank::CardRegister(string password, string user_id) {
	if (this->name == "") {
		return FAIL;
	}

	// automatically create a new card number for the user
	sql::ResultSet *res;
	long long max_card = 0;
	res = this->stmt->getResultSet();
	res = this->stmt->executeQuery("select max(card_number) from " + this->name);
	if (res == nullptr) {
		max_card = 0;
	}
	else {
		res->next();
		max_card = res->getInt64(1);
	}
	cout << "max_card_number = " << max_card << endl;

	// add to the database
	sql::PreparedStatement *prep_stmt;
	string cmd = "insert into " + this->name + "(card_number, password, user_id, balance, credit) ";
	cmd += "values (?, ?, ?, ?, ?)";
	cout << cmd << endl;
	prep_stmt = this->con->prepareStatement(cmd);
	prep_stmt->setInt64(1, max_card + 1);
	prep_stmt->setString(2, password);
	prep_stmt->setString(3, user_id);
	prep_stmt->setDouble(4, 0.0);
	prep_stmt->setDouble(5, 0.0);
	prep_stmt->execute();
	return (max_card+1);
}

int Bank::CardLogIn(long long card_number, string password) {
	if (this->name == "") {
		return FAIL;
	}

	try {
		BankCard *card = new BankCard(this->name, card_number, this->con);
		if (card->LogIn(password) == SUCCESS) {
			this->current_card = card;
			return SUCCESS;
		}
		else {
			delete card;
			return FALSE_PASS;
		}
	}
	catch (sql::SQLException &e) {
		return NO_ACCOUNT;
	}
}

int Bank::CardLogOut() {
	delete this->current_card;
	this->current_card = NULL;
	return SUCCESS;
}

int Bank::CardChangePass(string old_password, string new_password) {
	if (this->current_card == NULL) {
		return NO_LOGIN;
	}

	return this->current_card->ChangePass(old_password, new_password);
}

int Bank::Save(float amount) {
	if (this->current_card == NULL) {
		return NO_LOGIN;
	}
	return this->current_card->Save(amount);
}

int Bank::Withdraw(float amount) {
	if (this->current_card == NULL) {
		return NO_LOGIN;
	}
	return this->current_card->Withdraw(amount);
}

int Bank::Transfer(string bank, long long bank_card_account, float amount) {
	if (this->current_card == NULL) {
		return NO_LOGIN;
	}

	int balance = this->current_card->CheckBalance();
	if (balance < amount) {
		return FAIL;
	}

	try {
		BankCard *target = new BankCard(bank, bank_card_account, this->con);
		if (target == NULL) {
			return NO_ACCOUNT;
		}
		this->current_card->Withdraw(amount);
		target->Save(amount);
		delete target;
		return SUCCESS;
	}
	catch (sql::SQLException &e) {
		return NO_ACCOUNT;
	}
}

int Bank::Pay(string bank, long long bank_card_account, string password, float amount) {
	try {
		BankCard *c = new BankCard(bank, bank_card_account, this->con);
		if (c->LogIn(password) != SUCCESS)
			return FALSE_PASS;

		int balance = c->CheckBalance();
		int credit = c->CheckCredit();
		if (balance + credit < amount) {
			return FAIL;
		}

		return c->Pay(amount);
	}
	catch (sql::SQLException &e) {
		return NO_ACCOUNT;
	}
}

float Bank::CheckBalance() {
	if (this->current_card == NULL) {
		return NULL;
	}

	return this->current_card->CheckBalance();
}

float Bank::CheckCredit() {
	if (this->current_card == NULL) {
		return NULL;
	}

	return this->current_card->CheckCredit();
}

string Bank::HistoryList() {
	if (this->current_card == NULL) {
		return NULL;
	}

	return this->current_card->HistoryList();
}

// --------------------------------------------- Admin Op ---------------------------------------------

int Bank::CreateBank(string name) {
	if (this->current_admin == NULL) {
		return NO_LOGIN;
	}

	vector<string> exist = this->BankList();
	vector<string>::iterator it = find(exist.begin(), exist.end(), name);
	if (it != exist.end()) {
		return FAIL;
	}

	string cmd = "create table " + name;
	cmd += "(card_number bigint, password char(16), user_id char(18), balance double, credit double)";
	cout << cmd << endl;
	this->stmt->execute(cmd);
	return SUCCESS;
}

int Bank::AdminLogIn(string name, string password) {
	try {
		Admin *admin = new Admin(name, this->con);
		if (admin->LogIn(password) == SUCCESS) {
			this->current_admin = admin;
			return SUCCESS;
		}
		else {
			delete admin;
			return FALSE_PASS;
		}
	}
	catch (sql::SQLException &e) {
		return NO_ACCOUNT;
	}
}

int Bank::AdminLogOut() {
	delete this->current_admin;
	this->current_admin = NULL;
	return SUCCESS;
}
