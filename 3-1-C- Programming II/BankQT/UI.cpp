/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		UI.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Realize the methods of the class BankQT.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include "bank.h"
#include "ui.h"

// ----------------------- Useful function ----------------------------

void ClearLayout(QLayout *layout)
{
	if (layout == nullptr)
		return;
	QLayoutItem *item;
	while ((item = layout->takeAt(0)) != 0) {
		// delete the widget
		if (item->widget()) {
			delete item->widget();
			//item->widget()->deleteLater();
		}
		// delete child layout
		QLayout *childLayout = item->layout();
		if (childLayout) {
			ClearLayout(childLayout);
		}
		delete item;
	}
}

// ----------------------- Init and many GUI transfer ----------------------------

BankQT::BankQT(QWidget *parent)
	: QMainWindow(parent)
{
	center_window = new QWidget;
	this->setCentralWidget(center_window);

	main_layout = new QVBoxLayout;
	CreateServer();
	Init();
}

void BankQT::Init() {
	ClearLayout(main_layout);
	bank.SetBank("");

	QLabel *welcom = new QLabel;
	welcom->setText(QString::fromWCharArray(L"欢迎来到集成银行系统！"));
	this->setWindowTitle(QString::fromWCharArray(L"集成银行系统"));

	QPushButton *admin = new QPushButton;
	admin->setText(QString::fromWCharArray(L"管理员登陆"));
	common_v_layout = new QVBoxLayout;
	common_v_layout->addWidget(admin);
	connect(admin, &QPushButton::clicked, this, [=] {AdminLogin(); });
	if (bank.IfAdmin()) {
		AdminOp();
	}

	QHBoxLayout *upper = new QHBoxLayout;
	upper->addWidget(welcom);
	upper->addStretch();
	upper->addLayout(common_v_layout);

	QLabel *choose = new QLabel;
	choose->setText(QString::fromWCharArray(L"请选择一个银行以继续操作："));

	vector<string> bank_name = bank.BankList();
	QPushButton **bank_in = new QPushButton*[bank_name.size()];
	QHBoxLayout *banks = new QHBoxLayout;
	banks->addStretch();
	for (int i = 0; i < bank_name.size(); i++) {
		bank_in[i] = new QPushButton;
		bank_in[i]->setText(QString::fromStdString(bank_name[i]));
		banks->addWidget(bank_in[i]);
		banks->addStretch();
	}
	QVBoxLayout *down = new QVBoxLayout();
	down->addWidget(choose);
	down->addLayout(banks);

	this->main_layout->addLayout(upper);
	this->main_layout->addStretch();
	this->main_layout->addLayout(down);
	this->main_layout->addStretch();
	center_window->setLayout(main_layout);

	for (int i = 0; i < bank_name.size(); i++) {
		// the buttons corresponds to the banks
		connect(bank_in[i], &QPushButton::clicked, this, [=]{UserLogin(bank_name[i]); });
	}
}

void BankQT::AdminLogin() {
	ClearLayout(common_v_layout);

	QLabel *user = new QLabel;
	user->setText(QString::fromWCharArray(L"用户名："));
	temp_line_1 = new QLineEdit;
	QHBoxLayout *u = new QHBoxLayout;
	u->addWidget(user);
	u->addWidget(temp_line_1);

	QLabel *pass = new QLabel;
	pass->setText(QString::fromWCharArray(L"密码："));
	temp_line_2 = new QLineEdit;
	temp_line_2->setEchoMode(QLineEdit::Password);
	QHBoxLayout *p = new QHBoxLayout;
	u->addWidget(pass);
	u->addWidget(temp_line_2);

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"登陆"));
	QPushButton *cancel = new QPushButton;
	cancel->setText(QString::fromWCharArray(L"取消"));
	QHBoxLayout *d = new QHBoxLayout;
	d->addWidget(confirm);
	d->addWidget(cancel);

	common_v_layout->addLayout(u);
	common_v_layout->addLayout(p);
	common_v_layout->addLayout(d);

	connect(confirm, &QPushButton::clicked, this, 
		[=] {
		int success = bank.AdminLogIn(temp_line_1->text().toStdString(), temp_line_2->text().toStdString());
		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功"), 
				QString::fromWCharArray(L"登陆成功")
			);
			AdminOp();
		}
		else 
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"用户名或密码不正确")
			);
	});
	connect(cancel, &QPushButton::clicked, this, 
		[&] {
		ClearLayout(common_v_layout);
		Init();
	});
}

void BankQT::AdminOp() {
	ClearLayout(common_v_layout);

	QLabel *welcome = new QLabel;
	welcome->setText(QString::fromWCharArray(L"欢迎回来，管理员"));

	QPushButton *add_bank = new QPushButton;
	add_bank->setText(QString::fromWCharArray(L"添加银行"));
	QPushButton *logout = new QPushButton;
	logout->setText(QString::fromWCharArray(L"登出"));
	QHBoxLayout *d = new QHBoxLayout;
	d->addWidget(add_bank);
	d->addWidget(logout);

	common_v_layout->addWidget(welcome);
	common_v_layout->addLayout(d);
	connect(add_bank, &QPushButton::clicked, this, [&] {CreateBank(); });
	connect(logout, &QPushButton::clicked, this,
		[&] {
		bank.AdminLogOut();
		ClearLayout(common_v_layout);
		Init();
	});
}

void BankQT::CreateBank() {
	QDialog *temp = new QDialog;
	QHBoxLayout *a = new QHBoxLayout;
	QLabel *l = new QLabel;
	l->setText(QString::fromWCharArray(L"银行名称："));
	QLineEdit *name = new QLineEdit;
	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"确定"));
	a->addWidget(l);
	a->addWidget(name);
	a->addWidget(confirm);

	temp->setLayout(a);

	connect(confirm, &QPushButton::clicked, this, 
		[&] {
		int success = bank.CreateBank(name->text().toStdString());
		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功"), 
				QString::fromWCharArray(L"创建成功")
			);
			temp->close();
			Init();
		}
		else
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"已有同名银行")
			);
	});

	temp->exec();
	ClearLayout(a);
	delete a;
	temp->close();
	delete temp;
}

void BankQT::UserLogin(string bank_name) {
	int b = bank.SetBank(bank_name);
	if (b != SUCCESS) {
		QMessageBox::information(this, 
			QString::fromStdString(to_string(b)), 
			tr("Set Bank Error.")
		);
		return;
	}
	ClearLayout(main_layout);

	string title = bank_name;
	this->setWindowTitle(
		QString::fromWCharArray(L"银行 ") 
		+ QString::fromStdString(bank_name)
		);
	QLabel *welcom = new QLabel;
	welcom->setText(
		QString::fromWCharArray(L"欢迎来到银行") 
		+ QString::fromStdString(bank_name)
	);

	QPushButton *back = new QPushButton;
	back->setText(QString::fromWCharArray(L"返回"));

	QHBoxLayout *upper = new QHBoxLayout;
	upper->addWidget(welcom);
	upper->addStretch();
	upper->addWidget(back);
	
	QLabel *propganda = new QLabel;
	QPixmap image;
	string jpg_name = bank_name + ".jpg";
	image.load(QString::fromStdString(jpg_name));
	propganda->setPixmap(image);
	//propganda->setText(tr("This is a propaganda image."));

	QLabel *log = new QLabel;
	log->setText(QString::fromWCharArray(L"登陆："));
	QLabel *card = new QLabel;
	card->setText(QString::fromWCharArray(L"卡号："));
	temp_line_1 = new QLineEdit;
	temp_line_1->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
	QLabel *pass = new QLabel;
	pass->setText(QString::fromWCharArray(L"密码："));
	temp_line_2 = new QLineEdit;
	temp_line_2->setEchoMode(QLineEdit::Password);

	QPushButton *in = new QPushButton;
	in->setText(QString::fromWCharArray(L"登陆"));
	QPushButton *reg = new QPushButton;
	reg->setText(QString::fromWCharArray(L"注册"));
	QHBoxLayout *button = new QHBoxLayout;
	button->addWidget(in);
	button->addWidget(reg);

	common_v_layout = new QVBoxLayout;
	common_v_layout->addWidget(log);
	common_v_layout->addStretch();
	common_v_layout->addWidget(card);
	common_v_layout->addWidget(temp_line_1);
	common_v_layout->addStretch();
	common_v_layout->addWidget(pass);
	common_v_layout->addWidget(temp_line_2);
	common_v_layout->addStretch();
	common_v_layout->addLayout(button);
	common_v_layout->addStretch();

	QHBoxLayout *down = new QHBoxLayout;
	down->addStretch();
	down->addWidget(propganda);
	down->addStretch();
	down->addLayout(common_v_layout);
	down->addStretch();

	main_layout->addLayout(upper);
	main_layout->addLayout(down);
	center_window->setLayout(main_layout);

	connect(back, SIGNAL(clicked()), this, SLOT(Init()));
	connect(reg, SIGNAL(clicked()), this, SLOT(UserRegister()));
	connect(in, &QPushButton::clicked, this,
		[=] {
		long long card_number;
		istringstream is(this->temp_line_1->text().toStdString());
		is >> card_number;
		if (this->temp_line_2->text().toStdString().length() > 16) {
			QMessageBox::information(this, 
				this->temp_line_2->text(), 
				QString::fromWCharArray(L"密码不能超过16个字母")
			);
			return;
		}
		int success = bank.CardLogIn(card_number, 
			this->temp_line_2->text().toStdString()
		);
		temp_line_2->setText("");
		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功"), 
				QString::fromWCharArray(L"登陆成功")
			);
			this->remember = temp_line_1->text();
			UserOp();
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"卡号或密码不正确")
			);
		}
	});
}

void BankQT::UserRegister() {
	ClearLayout(common_v_layout);

	QLabel *msg_id = new QLabel;
	msg_id->setText(QString::fromWCharArray(L"真实姓名："));
	temp_line_1 = new QLineEdit;
	temp_line_1->setMaxLength(18);
	QHBoxLayout *one = new QHBoxLayout;
	one->addWidget(msg_id);
	one->addWidget(temp_line_1);

	QLabel *msg_pass1 = new QLabel;
	msg_pass1->setText(QString::fromWCharArray(L"设置密码："));
	temp_line_2 = new QLineEdit;
	temp_line_2->setMaxLength(16);
	temp_line_2->setEchoMode(QLineEdit::Password);
	QHBoxLayout *two = new QHBoxLayout;
	two->addWidget(msg_pass1);
	two->addWidget(temp_line_2);

	QLabel *msg_pass2 = new QLabel;
	msg_pass2->setText(QString::fromWCharArray(L"重复密码："));
	temp_line_3 = new QLineEdit;
	temp_line_3->setMaxLength(16);
	temp_line_3->setEchoMode(QLineEdit::Password);
	QHBoxLayout *three = new QHBoxLayout;
	three->addWidget(msg_pass2);
	three->addWidget(temp_line_3);

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"确认注册"));
	QPushButton *cancel = new QPushButton;
	cancel->setText(QString::fromWCharArray(L"取消"));
	QHBoxLayout *four = new QHBoxLayout;
	four->addWidget(cancel);
	four->addWidget(confirm);

	common_v_layout->addLayout(one);
	common_v_layout->addLayout(two);
	common_v_layout->addLayout(three);
	common_v_layout->addLayout(four);
	setLayout(common_v_layout);

	connect(cancel, &QPushButton::clicked, this, [=] {UserLogin(bank.name); });
	connect(confirm, &QPushButton::clicked, this,
		[=] {
		if (temp_line_2->text() != temp_line_3->text()) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"两次输入密码不一致")
			);
			return;
		}
		long long success = bank.CardRegister(
			temp_line_2->text().toStdString(), 
			temp_line_1->text().toStdString()
		);
		if (success == FAIL) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"输入错误")
			);
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"注册成功"), 
				QString::fromWCharArray(L"你的卡号是：") 
				+ QString::fromStdString(to_string(success))
			);
			UserLogin(bank.name);
		}
	});
}

void BankQT::UserOp() {
	ClearLayout(main_layout);

	QLabel *welcom = new QLabel;
	welcom->setText(QString::fromWCharArray(L"欢迎回来，用户") + this->remember);

	QPushButton *back = new QPushButton;
	back->setText(QString::fromWCharArray(L"返回"));

	QHBoxLayout *upper = new QHBoxLayout;
	upper->addWidget(welcom);
	upper->addStretch();
	upper->addWidget(back);

	QPushButton *save = new QPushButton;
	save->setText(QString::fromWCharArray(L"存款"));
	QPushButton *withdraw = new QPushButton;
	withdraw->setText(QString::fromWCharArray(L"取款"));
	QPushButton *transfer = new QPushButton;
	transfer->setText(QString::fromWCharArray(L"转账"));
	QPushButton *check = new QPushButton;
	check->setText(QString::fromWCharArray(L"查看余额"));
	QPushButton *change_pass = new QPushButton;
	change_pass->setText(QString::fromWCharArray(L"修改密码"));

	QGridLayout *operation = new QGridLayout;
	operation->addWidget(save, 0, 0, 1, 1);
	operation->addWidget(withdraw, 0, 1, 1, 1);
	operation->addWidget(transfer, 1, 0, 1, 1);
	operation->addWidget(check, 1, 1, 1, 1);
	operation->addWidget(change_pass, 2, 0, 1, 2);

	common_v_layout = new QVBoxLayout;
	Save();

	QHBoxLayout *down = new QHBoxLayout;
	down->addLayout(operation);
	down->addStretch();
	down->addLayout(this->common_v_layout);

	main_layout->addLayout(upper);
	main_layout->addStretch();
	main_layout->addLayout(down);
	main_layout->addStretch();
	center_window->setLayout(main_layout);

	connect(save, SIGNAL(clicked()), this, SLOT(Save()));
	connect(withdraw, SIGNAL(clicked()), this, SLOT(WithDraw()));
	connect(transfer, SIGNAL(clicked()), this, SLOT(Transfer()));
	connect(check, SIGNAL(clicked()), this, SLOT(Check()));
	connect(back, &QPushButton::clicked, this, [=] {UserLogin(bank.name); });
	connect(change_pass, &QPushButton::clicked, this, [=] {UserChangePass(); });
}

void BankQT::UserChangePass() {
	ClearLayout(common_v_layout);

	QLabel *msg = new QLabel;
	msg->setText(QString::fromWCharArray(L"请输入当前密码："));
	temp_line_1 = new QLineEdit;
	temp_line_1->setMaxLength(16);
	temp_line_1->setEchoMode(QLineEdit::Password);
	QHBoxLayout *one = new QHBoxLayout;
	one->addWidget(msg);
	one->addWidget(temp_line_1);

	QLabel *msg2 = new QLabel;
	msg2->setText(QString::fromWCharArray(L"请输入新的密码："));
	temp_line_2 = new QLineEdit;
	temp_line_2->setMaxLength(16);
	temp_line_2->setEchoMode(QLineEdit::Password);
	QHBoxLayout *two = new QHBoxLayout;
	two->addWidget(msg2);
	two->addWidget(temp_line_2);

	QLabel *msg3 = new QLabel;
	msg3->setText(QString::fromWCharArray(L"再次确认新密码："));
	temp_line_3 = new QLineEdit;
	temp_line_3->setMaxLength(16);
	temp_line_3->setEchoMode(QLineEdit::Password);
	QHBoxLayout *three = new QHBoxLayout;
	three->addWidget(msg3);
	three->addWidget(temp_line_3);

	QPushButton *confirm = new QPushButton();
	confirm->setText(QString::fromWCharArray(L"确认"));

	common_v_layout->addLayout(one);
	common_v_layout->addLayout(two);
	common_v_layout->addLayout(three);
	common_v_layout->addWidget(confirm);
	setLayout(common_v_layout);

	connect(confirm, &QPushButton::clicked, this,
		[=] {	
		if (temp_line_2->text().toStdString() 
			!= temp_line_3->text().toStdString()) 
		{
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"两次密码输入不一致")
			);
			return;
		}
		int success = bank.CardChangePass(
			temp_line_1->text().toStdString(), 
			temp_line_2->text().toStdString()
		);
		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功"), 
				QString::fromWCharArray(L"密码修改成功")
			);
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"请重新尝试")
			);
		}
	});
}

void BankQT::Save() {
	ClearLayout(common_v_layout);

	QLabel *msg = new QLabel;
	msg->setText(QString::fromWCharArray(L"请输入存款金额"));
	temp_line_1 = new QLineEdit;
	temp_line_1->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));
	
	QPushButton *confirm = new QPushButton();
	confirm->setText(QString::fromWCharArray(L"确认"));

	common_v_layout->addWidget(msg);
	common_v_layout->addWidget(temp_line_1);
	common_v_layout->addWidget(confirm);
	setLayout(common_v_layout);

	connect(confirm, &QPushButton::clicked, this,
		[=] {
		float amount;
		istringstream is(this->temp_line_1->text().toStdString());
		is >> amount;
		int success = bank.Save(amount);
		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功"), 
				QString::fromWCharArray(L"存款成功")
			);
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"请重新尝试")
			);
		}
	});
}

void BankQT::WithDraw() {
	ClearLayout(common_v_layout);

	QLabel *msg = new QLabel;
	msg->setText(QString::fromWCharArray(L"请输入取款金额"));
	temp_line_1 = new QLineEdit;
	temp_line_1->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));

	QPushButton *confirm = new QPushButton();
	confirm->setText(QString::fromWCharArray(L"确认"));

	common_v_layout->addWidget(msg);
	common_v_layout->addWidget(temp_line_1);
	common_v_layout->addWidget(confirm);
	setLayout(common_v_layout);

	connect(confirm, &QPushButton::clicked, this,
		[=] {
		float amount;
		istringstream is(this->temp_line_1->text().toStdString());
		is >> amount;
		int success = bank.Withdraw(amount);
		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功"), 
				QString::fromWCharArray(L"取款成功")
			);
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"余额不足")
			);
		}
	});
}

void BankQT::Transfer() {
	ClearLayout(common_v_layout);

	vector<string> list = bank.BankList();

	QLabel *msg_list = new QLabel;
	msg_list->setText(QString::fromWCharArray(L"请输入目标银行名称："));
	common_combo = new QComboBox;
	for (int i = 0; i < list.size(); i++)
		common_combo->addItem(QString::fromStdString(list[i]));
	QHBoxLayout *one = new QHBoxLayout;
	one->addWidget(msg_list);
	one->addWidget(common_combo);

	QLabel *msg_account = new QLabel;
	msg_account->setText(QString::fromWCharArray(L"请输入目标账号："));
	temp_line_1 = new QLineEdit;
	temp_line_1->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
	QHBoxLayout *two = new QHBoxLayout;
	two->addWidget(msg_account);
	two->addWidget(temp_line_1);

	QLabel *msg_amount = new QLabel;
	msg_amount->setText(QString::fromWCharArray(L"请输入转账金额："));
	temp_line_2 = new QLineEdit;
	temp_line_2->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));
	QHBoxLayout *three = new QHBoxLayout;
	three->addWidget(msg_amount);
	three->addWidget(temp_line_2);

	QPushButton *confirm = new QPushButton();
	confirm->setText(QString::fromWCharArray(L"确认"));

	common_v_layout->addLayout(one);
	common_v_layout->addLayout(two);
	common_v_layout->addLayout(three);
	common_v_layout->addWidget(confirm);
	setLayout(common_v_layout);

	connect(confirm, &QPushButton::clicked, this,
		[=] {
		long long account;
		float amount;
		istringstream is(
			this->temp_line_1->text().toStdString()),
			iss(this->temp_line_2->text().toStdString()
		);
		is >> account;
		iss >> amount;
		string target_bank = this->common_combo->currentText().toStdString();

		int success = bank.Transfer(target_bank, account, amount);
		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功"), 
				QString::fromWCharArray(L"转账成功")
			);
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"目标卡号不存在或余额不足")
			);
		}
	});
}

void BankQT::Check() {
	ClearLayout(common_v_layout);

	float balance = bank.CheckBalance();
	float credit = bank.CheckCredit();

	QLabel *msg_balance = new QLabel;
	msg_balance->setText(
		QString::fromWCharArray(L"余额：") 
		+ QString::fromStdString(to_string(balance))
	);

	QLabel *msg_credit = new QLabel;
	msg_credit->setText(
		QString::fromWCharArray(L"信用额度：") 
		+ QString::fromStdString(to_string(credit))
	);

	common_v_layout->addWidget(msg_balance);
	common_v_layout->addWidget(msg_credit);
	setLayout(common_v_layout);
}

BankQT::~BankQT()
{
	ClearLayout(main_layout);
	//if (this->server)
	//	this->server->deleteLater();
	//if (this->tcp)
	//	this->tcp->deleteLater();
}

// ----------------------- Communication Part ----------------------------

void BankQT::CreateServer() {
	this->server = new QTcpServer(this);
	this->blockSize = (quint16)0;
	if (!this->server->listen(QHostAddress::LocalHost, 6000))
	{
		QMessageBox::information(this, 
			QString::fromWCharArray(L"出错了"), 
			QString::fromWCharArray(L"服务器设置失败")
		);
		this->server->deleteLater();
	}
	QMessageBox::information(this, 
		QString::fromWCharArray(L"提示"), 
		QString::fromWCharArray(L"监听端口：6000")
	);
	connect(server, &QTcpServer::newConnection, this, [=] {AcceptConnection(); });
}

void BankQT::AcceptConnection() {
	QMessageBox::information(this, 
		QString::fromWCharArray(L"提示"), 
		QString::fromWCharArray(L"有一个新连接")
	);
	QTcpSocket* new_tcp = this->server->nextPendingConnection();

	connect(new_tcp, &QTcpSocket::readyRead, this, [=] {ReceiveMsg(new_tcp); });
	connect(new_tcp, &QTcpSocket::disconnected, this, [=] {new_tcp->deleteLater(); });
}

void BankQT::DisplaySocketError(QTcpSocket *tcp) {
	QMessageBox::information(this, 
		QString::fromWCharArray(L"连接失败"), 
		tcp->errorString()
	);
}

void BankQT::ReceiveMsg(QTcpSocket *tcp) {
	QString temp;
	temp = tcp->readAll();

	vector<string> result = StringParse(temp.toStdString(), ",");
	if (result.size() <= 0)
		return;
	int request = stoi(result[0]);

	switch (request) {
	case 0: {
		// pay with card
		string bank_name = result[1];
		long long card_number = stoll(result[2]);
		string password = result[3];
		float amount = stof(result[4]);
		int success = bank.Pay(bank_name, card_number, password, amount);
		string msg_s = to_string(success);
		const char *msg = msg_s.data();
		tcp->write(msg);
		break;
	}
	case 1: {
		// request bank list
		vector<string> list = bank.BankList();
		string msg_s = to_string(list.size());
		for (int i = 0; i < list.size(); i++)
			msg_s += "," + list[i];
		const char *msg = msg_s.data();
		tcp->write(msg);
		break;
	}
	default: {
		QByteArray block;
		const char *msg = "-1";
		tcp->write(msg);
		tcp->write(block);
		break;
	}
	}
}