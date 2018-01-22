/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		storeqt.h
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Define the class of StoreQT, which is the GUI realization of our program.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#ifndef STOREQT_H
#define STOREQT_H

#include <QtWidgets/QMainWindow>
#include "ui_storeqt.h"
#include "Store.h"
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
#include <QObject>
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
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QSpinBox>

#include <QtNetwork>

class StoreQT : public QMainWindow
{
	Q_OBJECT

public:
	StoreQT(QWidget *parent = 0);
	~StoreQT();

	void Init();
	void RefreshTable1();
	void RefreshTable2();

	void LogIn();
	void UserLogIn(string user);
	void UserLogOut();
	void UserRegister();
	void AdminLogIn(string admin);
	void AdminLogOut();
	
	void Search();

	void AddItem();
	void SetCutOff();
	void SetSaleCampaign();
	void Edit();

	void CheckItem();
	void Favour();
	void CheckFavour();
	void Order(int index);
	void CheckOrder();
	void CheckBankCard();
	void Pay();
	void PayCommuicate(string bank, long long card_number, string password, float amount);

	void ConnectUp();
	void BindCard_1();
	void BindCard_2(string bank_name);

private slots:
	void SearchSort(int index);
	void ConnectionError(QAbstractSocket::SocketError);
	void ReceiveMsg();

private:
	QStackedLayout *stack;
	QVBoxLayout *main_layout;
	QWidget *center_window;

	QLineEdit *temp_line_1;
	QLineEdit *temp_line_2;
	QLineEdit *temp_line_3;
	QLineEdit *temp_line_4;
	QVBoxLayout *common_v_layout_1;
	QVBoxLayout *common_v_layout_2;
	QHBoxLayout *common_h_layout;
	QHBoxLayout *common_h_layout_2;
	QGridLayout *common_g_layout;
	QPushButton *common_button;
	QComboBox *common_combo_1;
	QComboBox *common_combo_2;
	QComboBox *common_combo_3;
	QTableWidget *common_table_1;
	QTableWidget *common_table_2;
	QTableWidget *common_table_3;
	QStringList *common_string;
	vector<QLineEdit*> common_inputs;
	vector<QLabel*> common_labels;
	QSpinBox *common_spin;
	QDialog *common_dlg;

	string remember;
	vector<vector<string>> search_result_1;
	vector<vector<string>> search_result_2;
	vector<vector<string>> search_result_3;

	vector<float> final_price;
	int tcp_state = -1;
	quint16 blockSize;
	QTcpSocket *tcpSocket;
	QDialog *tcp_dlg;
	QHBoxLayout *tcp_h_layout;

	float promotion_bound = 100;
	float promotion_amount = 50;
};

#endif // STOREQT_H
