/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		main.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
The entrance of this program.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include "storeqt.h"
#include <QtWidgets/QApplication>

Store store("tcp://127.0.0.1:3306", "store", "store");

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StoreQT w;

	QFile qss(".\\black.css");
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();

	w.show();
	return a.exec();
}
