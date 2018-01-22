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

// ------------------------- Store initializtion -------------------------

Store::Store(string site = "tcp://127.0.0.1:3306", string user = "store", string password = "store") {
	// try to connect to the bank server
	try {
		this->driver = get_driver_instance();
		this->con = driver->connect(site, user, password);
		this->con->setSchema("store");
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
	this->current_user = NULL;
}

Store::~Store() {
	delete this->con;
	delete this->stmt;
}

Store::Store(const Store &s) {
	this->driver = s.driver;
	this->con = s.con;
	this->stmt = s.stmt;
	this->current_user = s.current_user;
	this->current_admin = s.current_admin;
}

// ------------------------- Store operations -------------------------

vector<string> Store::StoreList() {
	sql::ResultSet *res;
	this->con->setSchema("store");
	res = this->stmt->executeQuery("show tables;");
	vector<string> temp;
	while (res->next()) {
		temp.push_back(res->getString(1));
	}
	return temp;
}

sql::ResultSet* Store::CheckRepeat(string databse, string table, string id, string value) {
	string current_database = this->con->getSchema();

	this->con->setSchema(databse);
	string cmd = "select * from " + table + " where (";
	cmd += id + " = \'" + value + "\');";
	sql::ResultSet *res = this->stmt->getResultSet();
	res = this->stmt->executeQuery(cmd);

	this->con->setSchema(current_database);
	return res;
}

vector<vector<string>> Store::Search(string kind, string condition, string value, int exact_match) {
	this->con->setSchema("store");

	vector<vector<string>> result;
	sql::ResultSet *res = this->stmt->getResultSet();

	if (kind == "book" || kind == "any") {
		string cmd = "select * from book where (";
		if (exact_match)
			cmd += condition + "=\'" + value + "\')";
		else 
			cmd += condition + " like \'\%" + value + "\%\')";
		res = this->stmt->executeQuery(cmd);
		while (res->next()) {
			vector<string> temp;
			temp.push_back(res->getString(1));
			temp.push_back(res->getString(2));
			temp.push_back(to_string(res->getDouble(3)));
			temp.push_back(to_string(res->getInt(4)));
			temp.push_back(res->getString(5));
			temp.push_back(to_string(res->getDouble(6)));
			temp.push_back(res->getString(7));
			temp.push_back(res->getString(8));
			temp.push_back(res->getString(9));
			result.push_back(temp);
		}
	}
	if (kind == "food" || kind == "any") {
		string cmd = "select * from food where ("; 
		if (exact_match)
			cmd += condition + "=\'" + value + "\')";
		else
			cmd += condition + " like \'\%" + value + "\%\')";
		res = this->stmt->executeQuery(cmd);
		while (res->next()) {
			vector<string> temp;
			temp.push_back(res->getString(1));
			temp.push_back(res->getString(2));
			temp.push_back(to_string(res->getDouble(3)));
			temp.push_back(to_string(res->getInt(4)));
			temp.push_back(res->getString(5));
			temp.push_back(to_string(res->getDouble(6)));
			temp.push_back(res->getString(7));
			result.push_back(temp);
		}
	}
	if (kind == "clothes" || kind == "any") {
		string cmd = "select * from clothes where (";
		if (exact_match)
			cmd += condition + "=\'" + value + "\')";
		else
			cmd += condition + " like \'\%" + value + "\%\')";
		res = this->stmt->executeQuery(cmd);
		while (res->next()) {
			vector<string> temp;
			temp.push_back(res->getString(1));
			temp.push_back(res->getString(2));
			temp.push_back(to_string(res->getDouble(3)));
			temp.push_back(to_string(res->getInt(4)));
			temp.push_back(res->getString(5));
			temp.push_back(to_string(res->getDouble(6)));
			temp.push_back(res->getString(7));
			temp.push_back(res->getString(8));
			result.push_back(temp);
		}
	}
	else;
	return result;
}

// ------------------------- User operations -------------------------

int Store::UserRegister(string user_id, string password) {
	try {
		// check if there has been an established user with same user id
		sql::ResultSet *res = this->CheckRepeat("account", "user", "user_id", user_id);
		if (res->next()) {
			return ACCOUNT_EXIST;
		}

		// insert to table account
		this->con->setSchema("account");
		sql::PreparedStatement *prep_stmt;
		string cmd = "insert into user(user_id, password) ";
		cmd += "values (?, ?)";
		cout << cmd << endl;
		prep_stmt = this->con->prepareStatement(cmd);
		prep_stmt->setString(1, user_id);
		prep_stmt->setString(2, password);
		prep_stmt->execute();

		// the user id corresponding database
		cmd = "create database " + user_id;
		cout << cmd << endl;
		this->stmt->execute(cmd);
		this->con->setSchema(user_id);

		cmd = "create table bank_card(bank char(20), card_number bigint)";
		cout << cmd << endl;
		this->stmt->execute(cmd);

		cmd = "create table history(time int, name char(30), amount int, total_cost double, descr char(100))";
		cout << cmd << endl;
		this->stmt->execute(cmd);

		cmd = "create table favour(kind char(20), name char(30))";
		cout << cmd << endl;
		this->stmt->execute(cmd);

		cmd = "create table orders(kind char(20), name char(30), amount int)";
		cout << cmd << endl;
		this->stmt->execute(cmd);

		this->con->setSchema("store");
		return SUCCESS;
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
}

int Store::UserLogIn(string user_id, string password) {
	try {
		User *user = new User(user_id, this->con);
		if (user->LogIn(password) == SUCCESS) {
			this->current_user = user;
			return SUCCESS;
		}
		else {
			delete user;
			return FALSE_PASS;
		}
	}
	catch (sql::SQLException &e) {
		return NO_ACCOUNT;
	}
}

int Store::UserLogOut() {
	delete this->current_user;
	this->current_user = NULL;
	return SUCCESS;
}

int Store::UserChangePass(string old_password, string new_password) {
	if (this->current_user == NULL) {
		return NO_LOGIN;
	}
	return this->current_user->ChangePass(old_password, new_password);
}

int Store::Favour(string kind, string name) {
	// user access control
	if (this->current_user == NULL) {
		return NO_LOGIN;
	}

	// string length control
	if (kind.length() > 30 || name.length() > 30)
		return FAIL;

	// check if the item is already in the favour list
	this->con->setSchema(this->current_user->user_id);
	sql::ResultSet *res = CheckRepeat(this->current_user->user_id, "favour", "name", name);
	if (res->next()) {
		return ACCOUNT_EXIST;
	}

	// if no, insert one
	sql::PreparedStatement *prep_stmt;
	string cmd = "insert into favour(kind, name) ";
	cmd += "values (?, ?)";
	cout << cmd << endl;
	prep_stmt = this->con->prepareStatement(cmd);
	prep_stmt->setString(1, kind);
	prep_stmt->setString(2, name);
	prep_stmt->execute();

	return SUCCESS;
}

int Store::CancelFavour(string kind, string name) {
	if (this->current_user == NULL) {
		return NO_LOGIN;
	}
	this->con->setSchema(this->current_user->user_id);
	string cmd = "delete from favour ";
	cmd += "where (kind=\'" + kind + "\' AND name=\'" + name + "\')";
	cout << cmd << endl;
	try {
		this->stmt->execute(cmd);
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	return SUCCESS;
}

int Store::Order(string kind, string name, int amount) {
	// user access control
	if (this->current_user == NULL) {
		return NO_LOGIN;
	}

	// string length control
	if (kind.length() > 30 || name.length() > 30)
		return INPUT_ERROR;

	// operate in store
	this->con->setSchema("store");
	vector<vector<string>> result = Search(kind, "name", name, 1);
	if (result.size() <= 0)
		return FAIL;
	int left = stoi(result[0][3]) - amount;
	if (left < 0)
		return NO_ACCOUNT;
	result[0][3] = to_string(left);
	if (EditMerchan(result[0]) != SUCCESS)
		return FAIL;

	// add new item in the user's account
	this->con->setSchema(this->current_user->user_id);
	sql::ResultSet *res = CheckRepeat(this->current_user->user_id, "orders", "name", name);
	if (res->next()) {
		string cmd = "update orders set amount=" + to_string(res->getInt(3) + amount);
		cmd += " where (kind=\'" + kind + "\' AND name=\'" + name + "\')";
		cout << cmd << endl;
		this->stmt->executeUpdate(cmd);
	}
	else {
		sql::PreparedStatement *prep_stmt;
		string cmd = "insert into orders(kind, name, amount) ";
		cmd += "values (?, ?, ?)";
		cout << cmd << endl;
		prep_stmt = this->con->prepareStatement(cmd);
		prep_stmt->setString(1, kind);
		prep_stmt->setString(2, name);
		prep_stmt->setInt(3, amount);
		prep_stmt->execute();
	}
	return SUCCESS;
}

int Store::CancelOrder(string kind, string name) {
	// user access control
	if (this->current_user == NULL) {
		return NO_LOGIN;
	}

	this->con->setSchema(this->current_user->user_id);
	string cmd = "delete from orders ";
	cmd += "where (kind=\'" + kind + "\' AND name=\'" + name + "\')";
	cout << cmd << endl;
	try {
		this->stmt->execute(cmd);
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	return SUCCESS;
}

int Store::Pay(string kind, string name, int amount) {
	// We don't actually use this function anymore
	// should refer to the Admin::Pay()

	// user access control
	if (this->current_user == NULL) {
		return FAIL;
	}

	return FAIL;
}

int Store::CancelCard(string bank, long long card) {
	// user access control
	if (this->current_user == NULL) {
		return NO_LOGIN;
	}

	this->con->setSchema(this->current_user->user_id);
	string cmd = "delete from bank_card ";
	cmd += "where (bank=\'" + bank + "\' AND card_number=" + to_string(card) + ")";
	cout << cmd << endl;
	try {
		this->stmt->execute(cmd);
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	return SUCCESS;
}

vector<vector<string>> Store::CheckFavour() {
	vector<vector<string>> result;
	
	// user access control
	if (this->current_user == NULL) {
		return result;
	}

	this->con->setSchema(this->current_user->user_id);
	sql::ResultSet *res = this->stmt->getResultSet();
	res = this->stmt->executeQuery("select * from favour");
	while (res->next()) {
		vector<vector<string>> temp;
		temp = this->Search(res->getString(1), "name", res->getString(2), 1);
		if (temp.size() > 0)
			result.push_back(temp[0]);
	}
	return result;
}

vector<vector<string>> Store::CheckOrder() {
	vector<vector<string>> result;

	// user access control
	if (this->current_user == NULL) {
		return result;
	}

	this->con->setSchema(this->current_user->user_id);
	sql::ResultSet *res = this->stmt->getResultSet();
	res = this->stmt->executeQuery("select * from orders");
	while (res->next()) {
		vector<vector<string>> temp;
		temp = this->Search(res->getString(1), "name", res->getString(2), 1);
		if (temp.size() > 0) {
			temp[0][3] = to_string(res->getInt(3));
			result.push_back(temp[0]);
		}
	}
	return result;
}

vector<vector<string>> Store::CheckBankCard() {
	vector<vector<string>> result;

	// user access control
	if (this->current_user == NULL) {
		return result;
	}

	this->con->setSchema(this->current_user->user_id);
	sql::ResultSet *res = this->stmt->getResultSet();
	res = this->stmt->executeQuery("select * from bank_card");
	while (res->next()) {
		vector<string> temp;
		temp.push_back(res->getString(1));
		temp.push_back(res->getString(2));
		result.push_back(temp);
	}
	return result;
}

int Store::BindBankCard(string bank_name, long long card_number) {
	// user access control
	if (this->current_user == NULL) {
		return NO_LOGIN;
	}

	this->con->setSchema(this->current_user->user_id);
	sql::PreparedStatement *prep_stmt;
	string cmd = "insert into bank_card(bank, card_number) ";
	cmd += "values (?, ?)";
	cout << cmd << endl;
	prep_stmt = this->con->prepareStatement(cmd);
	prep_stmt->setString(1, bank_name);
	prep_stmt->setInt64(2, card_number);
	prep_stmt->execute();

	return SUCCESS;
}

sql::ResultSet* Store::HistoryList() {
	// user access control
	if (this->current_user == NULL) {
		return nullptr;
	}

	this->con->setSchema(this->current_user->user_id);
	sql::ResultSet *res = this->stmt->getResultSet();
	res = this->stmt->executeQuery("select * from history");
	return res;
}

// ------------------------- Admin operations -------------------------

int Store::AddMerchan(vector<string> v) {
	// user access control
	if (this->current_admin == NULL)
		return FAIL;

	string type = v[0];
	try {
		if (type == "book") {
			Book b(v, this->con);
			return b.Add();
		}
		else if (type == "food") {
			Food f(v, this->con);
			return f.Add();
		}
		else if (type == "clothes") {
			Clothes c(v, this->con);
			return c.Add();
		}
		else
			return FAIL;
	}
	catch (std::exception &e) {
		return INPUT_ERROR;
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
}

int Store::EditMerchan(vector<string> v) {
	try {
		string type = v[0];
		if (type == "book") {
			Book b(v, this->con);
			return b.Edit();
		}
		else if (type == "food") {
			Food f(v, this->con);
			return f.Edit();
		}
		else if (type == "clothes") {
			Clothes c(v, this->con);
			return c.Edit();
		}
		else
			return FAIL;
	}
	catch (std::exception &e) {
		return INPUT_ERROR;
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
}

int Store::SetCutOff(string kind, float cutoff) {
	// admin access control
	if (this->current_admin == NULL)
		return FAIL;

	this->con->setSchema("store");
	string cmd = "update " + kind + " set cut_off=" + to_string(cutoff);
	try {
		this->stmt->execute(cmd);
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	return SUCCESS;
}

int Store::AdminLogIn(string user_id, string password) {
	try {
		Admin *admin = new Admin(user_id, this->con);
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

int Store::AdminLogOut() {
	delete this->current_admin;
	this->current_admin = NULL;
	return SUCCESS;
}

void Store::TimeMachine() {
	// not used yet
}