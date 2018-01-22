/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		UI.h
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Define the class of BankQT, which is the GUI class of this program.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#ifndef BANKQT_H
#define BANKQT_H

#include <QtWidgets/QMainWindow>
#include "ui_bankqt.h"
#include "bank.h"
#include <QObject>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QBoxLayout>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QModelIndex>
#include <QStringMatcher>
#include <QSlider>
#include <QSplitter>
#include <QTimer>
#include <QTextEdit>
#include <QPicture>
#include <QPixmap>
#include <QFileDialog>
#include <QProcess>
#include <QIntValidator>
#include <QDateEdit>
#include <QCheckBox>
#include <QDir>
#include <QStackedLayout>  
#include <QStackedWidget>
#include <QListWidget>
#include <Qstring>
#include <QSignalMapper>
#include <QMessageBox>

#include <QtNetwork>
#include <QHostAddress>
#include <QTcpServer>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

// ----------------------- Function Declear ----------------------------

void ClearLayout(QLayout *layout);

// --------------------------- BankQT --------------------------------

class BankQT : public QMainWindow
{
	Q_OBJECT

public:
	BankQT(QWidget *parent = 0);
	~BankQT();

private slots:
	// GUI 
	void Init();
	void AdminLogin();
	void AdminOp();
	void UserLogin(string bank_name);
	void UserRegister();
	void UserOp();
	void UserChangePass();

	// Admin operation
	void CreateBank();

	// BankCard operation
	void Save();
	void WithDraw();
	void Transfer();
	void Check();

	// As a server
	void CreateServer();
	void ReceiveMsg(QTcpSocket* tcp);
	void DisplaySocketError(QTcpSocket* tcp);

	void AcceptConnection();

private:
	QStackedLayout *stack;
	QVBoxLayout *main_layout;
	QWidget *center_window;

	QLineEdit *temp_line_1;
	QLineEdit *temp_line_2;
	QLineEdit *temp_line_3;
	QVBoxLayout *common_v_layout;
	QHBoxLayout *common_h_layout;
	QPushButton *common_button;
	QComboBox *common_combo;

	QString remember;

	QTcpServer *server;
	quint16 blockSize;
};

#endif // BANKQT_H
