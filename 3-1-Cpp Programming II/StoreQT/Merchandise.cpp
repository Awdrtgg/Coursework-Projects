/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		Merchandise.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Complete the methods of class Merchandise, Book, Food and Clothes.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming 
*************************************************************************/

#include "Store.h"

// ------------------------- Merchan operations -------------------------

Merchandise::Merchandise(vector<string> v, sql::Connection *con) {
	this->kind = v[0];
	this->name = v[1];
	this->origin_price = stof(v[2]);
	this->amount = stoi(v[3]);
	this->intro = v[4];
	this->con = con;
}

Merchandise::Merchandise(const Merchandise &m) {
	this->kind = m.kind;
	this->name = m.name;
	this->origin_price = m.origin_price;
	this->amount = m.amount;
	this->intro = m.intro;
	this->con = m.con;
}

// ------------------------------- Book ----------------------------------

Book::Book(vector<string> v, sql::Connection *con) : Merchandise(v, con) {
	this->cutoff = stof(v[NUM_ATTR_MERCH]);
	this->isbn = v[NUM_ATTR_MERCH + 1];
	this->writer = v[NUM_ATTR_MERCH + 2];
	this->publisher = v[NUM_ATTR_MERCH + 3];
}

Book::Book(const Book &b) : Merchandise(b) {
	this->cutoff = b.cutoff;
	this->isbn = b.isbn;
	this->writer = b.writer;
	this->publisher = b.publisher;
}

float Book::GetPrice() {
	return (this->origin_price * this->amount * this->cutoff);
}

string Book::Where() {
	// return the tail string for this specific item in a MySQL sentence
	string where = " where ";
	where += "(kind=\'" + this->kind + "\' AND ";
	where += "name=\'" + this->name + "\' AND ";
	where += "intro=\'" + this->intro + "\' AND ";
	where += "isbn=\'" + this->isbn + "\' AND ";
	where += "writer=\'" + this->writer + "\')";
	return where;
}

int Book::Add() {
	// Add a new item
	try {
		this->con->setSchema("store");
		sql::PreparedStatement *prep_stmt;
		string cmd = "insert into book(kind, name, org_price, amount, intro, cut_off, isbn, writer, publisher) ";
		cmd += "values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
		cout << cmd << endl;
		prep_stmt = this->con->prepareStatement(cmd);
		prep_stmt->setString(1, this->kind);
		prep_stmt->setString(2, this->name);
		prep_stmt->setDouble(3, this->origin_price);
		prep_stmt->setInt(4, this->amount);
		prep_stmt->setString(5, this->intro);
		prep_stmt->setDouble(6, this->cutoff);
		prep_stmt->setString(7, this->isbn);
		prep_stmt->setString(8, this->writer);
		prep_stmt->setString(9, this->publisher);
		prep_stmt->execute();
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	catch (std::invalid_argument &e) {
		return FAIL;
	}
	return SUCCESS;
}

int Book::Edit() {
	// Edit this item in the database
	try {
		this->con->setSchema("store");
		sql::PreparedStatement *prep_stmt;
		string cmd = "update book set kind=?, name=?, org_price=?, amount=?, intro=?, cut_off=?, isbn=?, writer=?, publisher=? ";
		cmd += this->Where();
		cout << cmd << endl;
		prep_stmt = this->con->prepareStatement(cmd);
		prep_stmt->setString(1, this->kind);
		prep_stmt->setString(2, this->name);
		prep_stmt->setDouble(3, this->origin_price);
		prep_stmt->setInt(4, this->amount);
		prep_stmt->setString(5, this->intro);
		prep_stmt->setDouble(6, this->cutoff);
		prep_stmt->setString(7, this->isbn);
		prep_stmt->setString(8, this->writer);
		prep_stmt->setString(9, this->publisher);
		prep_stmt->executeUpdate();
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	catch (std::invalid_argument &e) {
		return FAIL;
	}
	return SUCCESS;
}

// ------------------------------- Food ----------------------------------

Food::Food(vector<string> v, sql::Connection *con) : Merchandise(v, con) {
	this->cutoff = stof(v[NUM_ATTR_MERCH]);
	this->exp_date = v[NUM_ATTR_MERCH + 1];
}

Food::Food(const Food &f) : Merchandise(f) {
	this->cutoff = f.cutoff;
	this->exp_date = f.exp_date;
}

float Food::GetPrice() {
	return (this->origin_price * this->amount * this->cutoff);
}

string Food::Where() {
	// return the tail string for this specific item in a MySQL sentence
	string where = " where ";
	where += "(kind=\'" + this->kind + "\' AND ";
	where += "name=\'" + this->name + "\' AND ";
	where += "intro=\'" + this->intro + "\' AND ";
	where += "exp_date=\'" + this->exp_date + "\')";
	return where;
}

int Food::Add() {
	// Add a new item
	try {
		this->con->setSchema("store");
		sql::PreparedStatement *prep_stmt;
		string cmd = "insert into food(kind, name, org_price, amount, intro, cut_off, exp_date) ";
		cmd += "values (?, ?, ?, ?, ?, ?, ?)";
		cout << cmd << endl;
		prep_stmt = this->con->prepareStatement(cmd);
		prep_stmt->setString(1, this->kind);
		prep_stmt->setString(2, this->name);
		prep_stmt->setDouble(3, this->origin_price);
		prep_stmt->setInt(4, this->amount);
		prep_stmt->setString(5, this->intro);
		prep_stmt->setDouble(6, this->cutoff);
		prep_stmt->setString(7, this->exp_date);
		prep_stmt->execute();
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	catch (std::invalid_argument &e) {
		return FAIL;
	}
	return SUCCESS;
}

int Food::Edit() {
	// Edit this item in the database
	try {
		this->con->setSchema("store");
		sql::PreparedStatement *prep_stmt;
		string cmd = "update food set kind=?, name=?, org_price=?, amount=?, intro=?, cut_off=?, exp_date=? ";
		cmd += this->Where();
		cout << cmd << endl;
		prep_stmt = this->con->prepareStatement(cmd);
		prep_stmt->setString(1, this->kind);
		prep_stmt->setString(2, this->name);
		prep_stmt->setDouble(3, this->origin_price);
		prep_stmt->setInt(4, this->amount);
		prep_stmt->setString(5, this->intro);
		prep_stmt->setDouble(6, this->cutoff);
		prep_stmt->setString(7, this->exp_date);
		prep_stmt->executeUpdate();
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	catch (std::invalid_argument &e) {
		return FAIL;
	}
	return SUCCESS;
}

// ------------------------------- Clothes ----------------------------------

Clothes::Clothes(vector<string> v, sql::Connection *con) : Merchandise(v, con) {
	this->cutoff = stof(v[NUM_ATTR_MERCH]);
	this->material = v[NUM_ATTR_MERCH + 1];
	this->size = v[NUM_ATTR_MERCH + 2];
}

Clothes::Clothes(const Clothes &c) : Merchandise(c) {
	this->cutoff = c.cutoff;
	this->material = c.material;
	this->size = c.size;
}

float Clothes::GetPrice() {
	return (this->origin_price * this->amount * this->cutoff);
}

string Clothes::Where() {
	// return the tail string for this specific item in a MySQL sentence
	string where = " where ";
	where += "(kind=\'" + this->kind + "\' AND ";
	where += "name=\'" + this->name + "\' AND ";
	where += "intro=\'" + this->intro + "\' AND ";
	where += "material=\'" + this->material + "\' AND ";
	where += "size=\'" + this->size + "\')";
	return where;
}

int Clothes::Add() {
	// Add a new item
	try {
		this->con->setSchema("store");
		sql::PreparedStatement *prep_stmt;
		string cmd = "insert into clothes(kind, name, org_price, amount, intro, cut_off, material, size) ";
		cmd += "values (?, ?, ?, ?, ?, ?, ?, ?)";
		cout << cmd << endl;
		prep_stmt = this->con->prepareStatement(cmd);
		prep_stmt->setString(1, this->kind);
		prep_stmt->setString(2, this->name);
		prep_stmt->setDouble(3, this->origin_price);
		prep_stmt->setInt(4, this->amount);
		prep_stmt->setString(5, this->intro);
		prep_stmt->setDouble(6, this->cutoff);
		prep_stmt->setString(7, this->material);
		prep_stmt->setString(8, this->size);
		prep_stmt->execute();
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	catch (std::invalid_argument &e) {
		return FAIL;
	}
	return SUCCESS;
}

int Clothes::Edit() {
	// Edit this item in the database
	try {
		this->con->setSchema("store");
		sql::PreparedStatement *prep_stmt;
		string cmd = "update clothes set kind=?, name=?, org_price=?, amount=?, intro=?, cut_off=?, material=?, size=? ";
		cmd += this->Where();
		cout << cmd << endl;
		prep_stmt = this->con->prepareStatement(cmd);
		prep_stmt->setString(1, this->kind);
		prep_stmt->setString(2, this->name);
		prep_stmt->setDouble(3, this->origin_price);
		prep_stmt->setInt(4, this->amount);
		prep_stmt->setString(5, this->intro);
		prep_stmt->setDouble(6, this->cutoff);
		prep_stmt->setString(7, this->material);
		prep_stmt->setString(8, this->size);
		prep_stmt->executeUpdate();
	}
	catch (sql::SQLException &e) {
		return FAIL;
	}
	catch (std::invalid_argument &e) {
		return FAIL;
	}
	return SUCCESS;
}
