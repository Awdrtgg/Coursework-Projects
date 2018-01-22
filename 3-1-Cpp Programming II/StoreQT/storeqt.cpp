/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		storeqt.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Realize the methods of storeqt class.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include "storeqt.h"

// ------------------------- Useful function -------------------------

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

// ------------------------- GUI realization -------------------------

void ClearWidgetFrom(QLayout *layout, int pos) {
	if (layout == nullptr)
		return;
	QLayoutItem *item;
	while ((item = layout->takeAt(pos)) != 0) {
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

StoreQT::StoreQT(QWidget *parent)
	: QMainWindow(parent)
{
	center_window = new QWidget;
	this->setCentralWidget(center_window);

	main_layout = new QVBoxLayout;
	ConnectUp();
	Init();
}

void StoreQT::Init() {
	ClearLayout(main_layout);

	QLabel *welcom = new QLabel;
	welcom->setText(QString::fromWCharArray(L"欢迎来到在线商城!"));
	this->setWindowTitle(QString::fromWCharArray(L"在线商城"));

	QPushButton *login = new QPushButton;
	login->setText(QString::fromWCharArray(L"登陆"));

	common_v_layout_1 = new QVBoxLayout;
	common_v_layout_1->addWidget(login);

	QHBoxLayout *upper = new QHBoxLayout;
	upper->addWidget(welcom);
	upper->addStretch();
	upper->addLayout(common_v_layout_1);

	QLabel *choose = new QLabel;
	choose->setText(QString::fromWCharArray(L"搜索商品"));

	QLabel *from = new QLabel;
	from->setText(QString::fromWCharArray(L"种类"));
	vector<string> store_kind = store.StoreList();
	common_combo_1 = new QComboBox;
	common_combo_1->addItem(tr("any"));
	for (int i = 0; i < store_kind.size(); i++)
		common_combo_1->addItem(QString::fromStdString(store_kind[i]));

	QLabel *msg_condition = new QLabel;
	msg_condition->setText(QString::fromWCharArray(L"搜索类型"));
	common_combo_2 = new QComboBox;
	common_combo_2->addItem(tr("name"));

	temp_line_3 = new QLineEdit;

	QPushButton *confirm_search = new QPushButton;
	confirm_search->setText(QString::fromWCharArray(L"搜索"));

	QHBoxLayout *search_area = new QHBoxLayout;
	search_area->addWidget(choose);
	search_area->addStretch();
	search_area->addWidget(from);
	search_area->addWidget(common_combo_1);
	search_area->addWidget(msg_condition);
	search_area->addWidget(common_combo_2);
	search_area->addWidget(temp_line_3);
	search_area->addStretch();
	search_area->addWidget(confirm_search);

	common_string = new QStringList;
	*common_string << QString::fromWCharArray(L"种类") 
		<< QString::fromWCharArray(L"名称") 
		<< QString::fromWCharArray(L"原价") 
		<< QString::fromWCharArray(L"剩余数量") 
		<< QString::fromWCharArray(L"简介");
	common_table_1 = new QTableWidget;
	common_table_1->setColumnCount(NUM_ATTR_MERCH);
	common_table_1->setHorizontalHeaderLabels(*common_string);
	common_table_1->verticalHeader()->setVisible(false);
	common_table_1->setSelectionBehavior(QAbstractItemView::SelectRows);
	common_table_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
	common_table_1->setSelectionMode(QAbstractItemView::SingleSelection);
	QHeaderView *head = common_table_1->horizontalHeader();
	head->setSortIndicator(0, Qt::AscendingOrder);
	head->setSortIndicatorShown(true);

	QPushButton *check = new QPushButton;
	check->setText(QString::fromWCharArray(L"查看"));
	common_v_layout_2 = new QVBoxLayout;
	common_v_layout_2->addWidget(check);

	common_h_layout = new QHBoxLayout;
	common_h_layout->addWidget(common_table_1);
	common_h_layout->addLayout(common_v_layout_2);

	this->main_layout->addLayout(upper);
	this->main_layout->addLayout(search_area);
	this->main_layout->addLayout(common_h_layout);
	center_window->setLayout(main_layout);

	connect(login, &QPushButton::clicked, this, [=] {LogIn(); });
	connect(confirm_search, &QPushButton::clicked, this, [=] {Search(); });
	connect(head, SIGNAL(sectionClicked(int)), this, SLOT(SearchSort(int)));
	connect(check, &QPushButton::clicked, this, [=] {CheckItem(); });
	connect(common_table_1, &QTableWidget::doubleClicked, this, [=] {CheckItem(); });
}

void StoreQT::LogIn() {
	common_dlg = new QDialog(this);
	common_dlg->setWindowTitle(QString::fromWCharArray(L"登陆"));

	QLabel *type = new QLabel;
	type->setText(QString::fromWCharArray(L"账户类型"));
	common_combo_3 = new QComboBox;
	common_combo_3->addItem(tr("Admin"));
	common_combo_3->addItem(tr("User"));

	QLabel *account = new QLabel;
	account->setText(QString::fromWCharArray(L"账户"));
	temp_line_1 = new QLineEdit;

	QLabel *pass = new QLabel;
	pass->setText(QString::fromWCharArray(L"密码"));
	temp_line_2 = new QLineEdit;
	temp_line_2->setEchoMode(QLineEdit::Password);

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"登陆"));

	QPushButton *reg = new QPushButton;
	reg->setText(QString::fromWCharArray(L"注册"));

	common_g_layout = new QGridLayout;
	common_g_layout->addWidget(type, 0, 0, 1, 1);
	common_g_layout->addWidget(common_combo_3, 0, 1, 1, 1);
	common_g_layout->addWidget(account, 1, 0, 1, 1);
	common_g_layout->addWidget(temp_line_1, 1, 1, 1, 1);
	common_g_layout->addWidget(pass, 2, 0, 1, 1);
	common_g_layout->addWidget(temp_line_2, 2, 1, 1, 1);
	common_g_layout->addWidget(confirm, 3, 0, 1, 1);
	common_g_layout->addWidget(reg, 3, 1, 1, 1);

	common_dlg->setLayout(common_g_layout);
	common_dlg->setModal(true);

	connect(confirm, &QPushButton::clicked, this,
		[=] {
		int success;
		string user = temp_line_1->text().toStdString();
		string pass = temp_line_2->text().toStdString();
		if (this->common_combo_3->currentText().toStdString() == "User") {
			success = store.UserLogIn(user, pass);
			if (success == SUCCESS) {
				QMessageBox::information(this, 
					tr("Successfull Log In!"), 
					QString::fromStdString(to_string(success))
				);
				common_dlg->close();
				UserLogIn(user);
				return;
			}
		}
		else {
			success = store.AdminLogIn(user, pass);
			if (success == SUCCESS) {
				QMessageBox::information(this, 
					tr("Successfull Log In!"), 
					QString::fromStdString(to_string(success))
				);
				common_dlg->close();
				AdminLogIn(user);
				return;
			}
		}
		QMessageBox::information(this, 
			QString::fromWCharArray(L"出错了"), 
			QString::fromWCharArray(L"账户或密码不正确")
		);
		this->temp_line_2->setText(tr(""));
	});
	connect(reg, &QPushButton::clicked, this,
		[=] {
		UserRegister();
	});

	common_dlg->exec();
	common_dlg->close();
	return;
}

void StoreQT::UserLogIn(string user) {
	ClearLayout(this->common_v_layout_1);

	QLabel *welcome = new QLabel;
	welcome->setText(
		QString::fromWCharArray(L"欢迎回来，用户") 
		+ QString::fromStdString(user)
	);

	QPushButton *logout = new QPushButton;
	logout->setText(QString::fromWCharArray(L"登出"));

	this->common_v_layout_1->addWidget(welcome);
	this->common_v_layout_1->addWidget(logout);

	QPushButton *favour = new QPushButton;
	favour->setText(QString::fromWCharArray(L"添加到收藏夹"));
	QPushButton *order = new QPushButton;
	order->setText(QString::fromWCharArray(L"添加到购物车"));
	QPushButton *check_favour = new QPushButton;
	check_favour->setText(QString::fromWCharArray(L"查看收藏夹"));
	QPushButton *check_order = new QPushButton;
	check_order->setText(QString::fromWCharArray(L"查看购物车"));
	QPushButton *bind_card = new QPushButton;
	bind_card->setText(QString::fromWCharArray(L"绑定银行卡"));
	QPushButton *check_card = new QPushButton;
	check_card->setText(QString::fromWCharArray(L"查看已绑定银行卡"));
	this->common_v_layout_2->addWidget(favour);
	this->common_v_layout_2->addWidget(order);
	this->common_v_layout_2->addWidget(check_favour);
	this->common_v_layout_2->addWidget(check_order);
	this->common_v_layout_2->addWidget(bind_card);
	this->common_v_layout_2->addWidget(check_card);

	connect(logout, &QPushButton::clicked, this, [=] {UserLogOut(); });
	connect(favour, &QPushButton::clicked, this, [=] {Favour(); });
	connect(order, &QPushButton::clicked, this, [=] {Order(1); });
	connect(check_favour, &QPushButton::clicked, this, [=] {CheckFavour(); });
	connect(check_order, &QPushButton::clicked, this, [=] {CheckOrder(); });
	connect(bind_card, &QPushButton::clicked, this, [=] {BindCard_1(); });
	connect(check_card, &QPushButton::clicked, this, [=] {CheckBankCard(); });
}

void StoreQT::UserRegister() {
	ClearLayout(common_g_layout);

	QLabel *msg_id = new QLabel;
	msg_id->setText(QString::fromWCharArray(L"输入账号"));
	temp_line_1 = new QLineEdit;
	temp_line_1->setMaxLength(16);

	QLabel *msg_pass1 = new QLabel;
	msg_pass1->setText(QString::fromWCharArray(L"设置密码"));
	temp_line_2 = new QLineEdit;
	temp_line_2->setMaxLength(16);
	temp_line_2->setEchoMode(QLineEdit::Password);
	QHBoxLayout *two = new QHBoxLayout;
	two->addWidget(msg_pass1);
	two->addWidget(temp_line_2);

	QLabel *msg_pass2 = new QLabel;
	msg_pass2->setText(QString::fromWCharArray(L"再次确认密码"));
	temp_line_4 = new QLineEdit;
	temp_line_4->setMaxLength(16);
	temp_line_4->setEchoMode(QLineEdit::Password);
	QHBoxLayout *three = new QHBoxLayout;
	three->addWidget(msg_pass2);
	three->addWidget(temp_line_4);

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"确认注册"));
	QPushButton *cancel = new QPushButton;
	cancel->setText(QString::fromWCharArray(L"取消"));
	QHBoxLayout *four = new QHBoxLayout;
	four->addWidget(cancel);
	four->addWidget(confirm);

	common_g_layout->addWidget(msg_id, 0, 0, 1, 1);
	common_g_layout->addWidget(temp_line_1, 0, 1, 1, 1);
	common_g_layout->addWidget(msg_pass1, 1, 0, 1, 1);
	common_g_layout->addWidget(temp_line_2, 1, 1, 1, 1);
	common_g_layout->addWidget(msg_pass2, 2, 0, 1, 1);
	common_g_layout->addWidget(temp_line_4, 2, 1, 1, 1);
	common_g_layout->addWidget(confirm, 3, 0, 1, 1);
	common_g_layout->addWidget(cancel, 3, 1, 1, 1);

	connect(cancel, &QPushButton::clicked, this, [=] {common_dlg->close(); LogIn(); });
	connect(confirm, &QPushButton::clicked, this,
		[=] {
		if (temp_line_2->text() != temp_line_4->text()) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"注册失败"), 
				QString::fromWCharArray(L"两次密码输入不一致")
			);
			return;
		}
		long long success = store.UserRegister(
			temp_line_1->text().toStdString(), 
			temp_line_2->text().toStdString()
		);

		if (success == FAIL) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"注册失败"), 
				QString::fromWCharArray(L"用户名已存在")
			);
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功!"), 
				QString::fromWCharArray(L"注册成功")
			);

			ClearLayout(common_g_layout);
			common_dlg->close();
			LogIn();
		}
	});
}

void StoreQT::AdminLogIn(string admin) {
	ClearLayout(this->common_v_layout_1);

	QLabel *welcome = new QLabel;
	welcome->setText(
		QString::fromWCharArray(L"欢迎回来，管理员") 
		+ QString::fromStdString(admin)
	);

	QPushButton *logout = new QPushButton;
	logout->setText(QString::fromWCharArray(L"登出"));

	this->common_v_layout_1->addWidget(welcome);
	this->common_v_layout_1->addWidget(logout);

	QPushButton *edit = new QPushButton;
	edit->setText(QString::fromWCharArray(L"修改"));
	QPushButton *add = new QPushButton;
	add->setText(QString::fromWCharArray(L"添加商品"));
	QPushButton *set_cut = new QPushButton;
	set_cut->setText(QString::fromWCharArray(L"商品打折"));
	QPushButton *set_promotion = new QPushButton;
	set_promotion->setText(QString::fromWCharArray(L"满减活动"));
	this->common_v_layout_2->addWidget(edit);
	this->common_v_layout_2->addWidget(add);
	this->common_v_layout_2->addWidget(set_cut);
	this->common_v_layout_2->addWidget(set_promotion);

	connect(logout, &QPushButton::clicked, this, [=] {AdminLogOut(); });
	connect(edit, &QPushButton::clicked, this, [=] {CheckItem(); });
	connect(add, &QPushButton::clicked, this, [=] {AddItem(); });
	connect(set_cut, &QPushButton::clicked, this, [=] {SetCutOff(); });
	connect(set_promotion, &QPushButton::clicked, this, [=] {SetSaleCampaign(); });
}

void StoreQT::UserLogOut() {
	store.UserLogOut();
	ClearLayout(this->common_v_layout_1);

	QPushButton *login = new QPushButton;
	login->setText(QString::fromWCharArray(L"登陆"));
	this->common_v_layout_1->addWidget(login);

	ClearWidgetFrom(this->common_v_layout_2, 1);

	connect(login, &QPushButton::clicked, this, [=] {LogIn(); });
}

void StoreQT::AdminLogOut() {
	store.AdminLogOut();
	ClearLayout(this->common_v_layout_1);

	QPushButton *login = new QPushButton;
	login->setText(QString::fromWCharArray(L"登陆"));
	this->common_v_layout_1->addWidget(login);

	ClearWidgetFrom(this->common_v_layout_2, 1);

	connect(login, &QPushButton::clicked, this, [=] {LogIn(); });
}

void StoreQT::RefreshTable1() {
	this->common_table_1->clearContents();
	this->common_table_1->setRowCount(0);
	if (this->search_result_1.size() == 0) {
		QMessageBox::information(this, 
			QString::fromWCharArray(L"提示"), 
			QString::fromWCharArray(L"没有相关商品结果")
		);
		return;
	}
	for (int i = 0; i < this->search_result_1.size(); i++) {
		int temp = common_table_1->rowCount();
		common_table_1->insertRow(temp);
		for (int j = 0; j < NUM_ATTR_MERCH; j++) {
			common_table_1->setItem(i, 
				j, 
				new QTableWidgetItem(
					QString::fromStdString(this->search_result_1[i][j])
				)
			);
		}
	}
}

void StoreQT::RefreshTable2() {
	this->common_table_2->clearContents();
	this->common_table_2->setRowCount(0);
	if (this->search_result_2.size() == 0) {
		QMessageBox::information(this, 
			QString::fromWCharArray(L"提示"), 
			QString::fromWCharArray(L"没有相关商品")
		);
		return;
	}
	for (int i = 0; i < this->search_result_2.size(); i++) {
		int temp = common_table_2->rowCount();
		common_table_2->insertRow(temp);
		for (int j = 0; j < NUM_ATTR_MERCH; j++) {
			common_table_2->setItem(i, 
				j, 
				new QTableWidgetItem(
					QString::fromStdString(this->search_result_2[i][j])
				)
			);
		}
	}
}

void StoreQT::Search() {
	this->search_result_1.clear();
	string kind = this->common_combo_1->currentText().toStdString();
	string condition = this->common_combo_2->currentText().toStdString();
	string value = this->temp_line_3->text().toStdString();
	this->search_result_1 = store.Search(kind, condition, value);

	this->common_table_1->clearContents();
	this->common_table_1->setRowCount(0);
	if (this->search_result_1.size() == 0) {
		QMessageBox::information(this, 
			QString::fromWCharArray(L"提示"), 
			QString::fromWCharArray(L"没有相关商品")
		);
		return;
	}
	for (int i = 0; i < this->search_result_1.size(); i++) {
		int temp = common_table_1->rowCount();
		common_table_1->insertRow(temp);
		for (int j = 0; j < NUM_ATTR_MERCH; j++) {
			common_table_1->setItem(i, 
				j, 
				new QTableWidgetItem(
					QString::fromStdString(this->search_result_1[i][j])
				)
			);
		}
	}
}

void StoreQT::SetCutOff() {
	QDialog *set_dlg = new QDialog(this);
	set_dlg->setWindowTitle(QString::fromWCharArray(L"设置折扣"));

	QLabel *hint = new QLabel;
	hint->setText(QString::fromWCharArray(L"设置折扣："));

	common_combo_3 = new QComboBox;
	common_combo_3->addItem(tr("book"));
	common_combo_3->addItem(tr("food"));
	common_combo_3->addItem(tr("clothes"));
	common_combo_3->setCurrentIndex(0);

	temp_line_1 = new QLineEdit;
	temp_line_1->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"确认"));

	QVBoxLayout *vertical = new QVBoxLayout;
	vertical->addWidget(hint);
	vertical->addWidget(common_combo_3);
	vertical->addWidget(temp_line_1);
	vertical->addWidget(confirm);

	set_dlg->setLayout(vertical);
	set_dlg->setModal(true);

	connect(confirm, &QPushButton::clicked, this, 
		[=] {
		int success = store.SetCutOff(
			common_combo_3->currentText().toStdString(),
			temp_line_1->text().toFloat()
		);

		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功！"), 
				QString::fromWCharArray(L"设置折扣成功")
			);
			set_dlg->close();
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"似乎出现了一些问题")
			);
		}
	});

	set_dlg->exec();
	ClearLayout(vertical);
	delete vertical;
	set_dlg->close();
	delete set_dlg;
	return;
}

void StoreQT::SetSaleCampaign() {
	QDialog *set_dlg = new QDialog(this);
	set_dlg->setWindowTitle(QString::fromWCharArray(L"满减活动"));

	QLabel *hint = new QLabel;
	hint->setText(QString::fromWCharArray(L"设置最小金额和返现"));

	temp_line_1 = new QLineEdit;
	temp_line_1->setValidator(new QDoubleValidator(0.0, 10000.0, 2, this));
	temp_line_1->setText(QString::fromStdString(to_string(this->promotion_bound)));

	temp_line_2 = new QLineEdit;
	temp_line_2->setValidator(new QDoubleValidator(0.0, 10000.0, 2, this));
	temp_line_2->setText(QString::fromStdString(to_string(this->promotion_amount)));

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"确定"));

	QVBoxLayout *vertical = new QVBoxLayout;
	vertical->addWidget(hint);
	vertical->addWidget(temp_line_1);
	vertical->addWidget(temp_line_2);
	vertical->addWidget(confirm);

	set_dlg->setLayout(vertical);
	set_dlg->setModal(true);

	connect(confirm, &QPushButton::clicked, this,
		[=] {
		this->promotion_bound = temp_line_1->text().toDouble();
		this->promotion_amount = temp_line_2->text().toDouble();
		QMessageBox::information(this, 
			QString::fromWCharArray(L"成功！"), 
			QString::fromWCharArray(L"设置成功")
		);
		set_dlg->close();
	});

	set_dlg->exec();
	ClearLayout(vertical);
	delete vertical;
	set_dlg->close();
	delete set_dlg;
	return;
}

void StoreQT::AddItem() {
	QDialog *add_dlg = new QDialog(this);
	add_dlg->setWindowTitle(QString::fromWCharArray(L"添加商品"));

	QLabel *type = new QLabel;
	type->setText(QString::fromWCharArray(L"类型"));
	common_combo_3 = new QComboBox;
	common_combo_3->addItem(tr("book"));
	common_combo_3->addItem(tr("food"));
	common_combo_3->addItem(tr("clothes"));
	common_combo_3->setCurrentIndex(-1);
	
	QLabel *msg1 = new QLabel;
	msg1->setText(QString::fromWCharArray(L"商品名"));
	common_labels.push_back(msg1);
	QLabel *msg2 = new QLabel;
	msg2->setText(QString::fromWCharArray(L"原价"));
	common_labels.push_back(msg2);
	QLabel *msg3 = new QLabel;
	msg3->setText(QString::fromWCharArray(L"数量"));
	common_labels.push_back(msg3);
	QLabel *msg4 = new QLabel;
	msg4->setText(QString::fromWCharArray(L"简介"));
	common_labels.push_back(msg4);

	QLineEdit *line1 = new QLineEdit;
	line1->setMaxLength(30);
	common_inputs.push_back(line1);
	QLineEdit *line2 = new QLineEdit;
	line2->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));
	common_inputs.push_back(line2);
	QLineEdit *line3 = new QLineEdit;
	line3->setValidator(new QIntValidator(0, 1000, this));
	common_inputs.push_back(line3);
	QLineEdit *line4 = new QLineEdit;
	line4->setMaxLength(100);
	common_inputs.push_back(line4);

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"确定"));

	QPushButton *cancel = new QPushButton;
	cancel->setText(QString::fromWCharArray(L"取消"));

	QHBoxLayout *down = new QHBoxLayout;
	down->addWidget(confirm);
	down->addWidget(cancel);

	QGridLayout *grid = new QGridLayout;
	grid->addWidget(type, 0, 0, 1, 1);
	grid->addWidget(common_combo_3, 0, 1, 1, 1);
	for (int i = 0; i < 4; i++) {
		grid->addWidget(common_labels[i], i + 1, 0, 1, 1);
		grid->addWidget(common_inputs[i], i + 1, 1, 1, 1);
	}

	QVBoxLayout *vertical = new QVBoxLayout;
	vertical->addLayout(grid);
	vertical->addLayout(down);

	add_dlg->setLayout(vertical);
	add_dlg->setModal(true);

	connect(confirm, &QPushButton::clicked, this,
		[=] {
		vector<string> inputs_string;
		inputs_string.push_back(this->common_combo_3->currentText().toStdString());
		vector<QLineEdit*>::iterator it = this->common_inputs.begin();
		while (it != common_inputs.end()) {
			inputs_string.push_back((*it)->text().toStdString());
			it++;
		}
		int success = store.AddMerchan(inputs_string);
		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功！"), 
				QString::fromWCharArray(L"添加商品成功")
			);
			add_dlg->close();
			return;
		}
		else 
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"已有此类商品")
			);
	});
	connect(cancel, &QPushButton::clicked, this,
		[=] {
		add_dlg->close();
	});
	connect(common_combo_3, 
		QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
		[=](const QString &text) { 
		while (this->common_inputs.size() > 4)
			common_inputs.erase(common_inputs.begin()+4);

		while (this->common_labels.size() > 4)
			common_labels.erase(common_labels.begin() + 4);

		ClearWidgetFrom(grid, 10);
		string kind = text.toStdString();

		if (kind == "book") {
			QLineEdit *line5 = new QLineEdit;
			line5->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
			common_inputs.push_back(line5);
			QLineEdit *line6 = new QLineEdit;
			line6->setMaxLength(13);
			common_inputs.push_back(line6);
			QLineEdit *line7 = new QLineEdit;
			line7->setMaxLength(30);
			common_inputs.push_back(line7);
			QLineEdit *line8 = new QLineEdit;
			line8->setMaxLength(30);
			common_inputs.push_back(line8);
			QLabel *msg5 = new QLabel;
			msg5->setText(QString::fromWCharArray(L"折扣："));
			common_labels.push_back(msg5);
			QLabel *msg6 = new QLabel;
			msg6->setText(tr("ISBN:"));
			common_labels.push_back(msg6);
			QLabel *msg7 = new QLabel;
			msg7->setText(QString::fromWCharArray(L"作者："));
			common_labels.push_back(msg7);
			QLabel *msg8 = new QLabel;
			msg8->setText(QString::fromWCharArray(L"出版方："));
			common_labels.push_back(msg8);
		}
		else if (kind == "food") {
			QLineEdit *line5 = new QLineEdit;
			line5->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
			common_inputs.push_back(line5);
			QLineEdit *line6 = new QLineEdit;
			line6->setMaxLength(30);
			common_inputs.push_back(line6);
			QLabel *msg5 = new QLabel;
			msg5->setText(QString::fromWCharArray(L"折扣："));
			common_labels.push_back(msg5);
			QLabel *msg6 = new QLabel;
			msg6->setText(QString::fromWCharArray(L"过期日期："));
			common_labels.push_back(msg6);
		}
		else if (kind == "clothes") {
			QLineEdit *line5 = new QLineEdit;
			line5->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
			common_inputs.push_back(line5);
			QLineEdit *line6 = new QLineEdit;
			line6->setMaxLength(30);
			common_inputs.push_back(line6);
			QLineEdit *line7 = new QLineEdit;
			line7->setMaxLength(20);
			common_inputs.push_back(line7);
			QLabel *msg5 = new QLabel;
			msg5->setText(QString::fromWCharArray(L"折扣："));
			common_labels.push_back(msg5);
			QLabel *msg6 = new QLabel;
			msg6->setText(QString::fromWCharArray(L"材质："));
			common_labels.push_back(msg6);
			QLabel *msg7 = new QLabel;
			msg7->setText(QString::fromWCharArray(L"大小："));
			common_labels.push_back(msg7);
		}
		else
			QMessageBox::information(this, QString::fromWCharArray(L"出错了"), text);

		for (int i = 4; i < common_inputs.size(); i++) {
			grid->addWidget(common_labels[i], i + 1, 0, 1, 1);
			grid->addWidget(common_inputs[i], i + 1, 1, 1, 1);
		}
	});

	add_dlg->exec();
	ClearLayout(vertical);
	delete vertical;
	add_dlg->close();
	delete add_dlg;
	common_labels.clear();
	common_inputs.clear();
	return;
}

void StoreQT::CheckItem() {
	QModelIndexList selection = common_table_1->selectionModel()->selectedRows();
	if (selection.size() <= 0)
		return;
	int row = selection.at(0).row();

	QDialog *add_dlg = new QDialog(this);
	add_dlg->setWindowTitle(QString::fromWCharArray(L"商品信息"));

	QLabel *type = new QLabel;
	type->setText(QString::fromWCharArray(L"类型："));
	common_combo_3 = new QComboBox;
	common_combo_3->addItem(tr("book"));
	common_combo_3->addItem(tr("food"));
	common_combo_3->addItem(tr("clothes"));

	common_labels.clear();
	QLabel *msg1 = new QLabel;
	msg1->setText(QString::fromWCharArray(L"商品名称："));
	common_labels.push_back(msg1);
	QLabel *msg2 = new QLabel;
	msg2->setText(QString::fromWCharArray(L"原价："));
	common_labels.push_back(msg2);
	QLabel *msg3 = new QLabel;
	msg3->setText(QString::fromWCharArray(L"剩余数量："));
	common_labels.push_back(msg3);
	QLabel *msg4 = new QLabel;
	msg4->setText(QString::fromWCharArray(L"简介："));
	common_labels.push_back(msg4);

	common_inputs.clear();
	QLineEdit *line1 = new QLineEdit;
	line1->setMaxLength(30);
	common_inputs.push_back(line1);
	QLineEdit *line2 = new QLineEdit;
	line2->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));
	common_inputs.push_back(line2);
	QLineEdit *line3 = new QLineEdit;
	line3->setValidator(new QIntValidator(0, 1000, this));
	common_inputs.push_back(line3);
	QLineEdit *line4 = new QLineEdit;
	line4->setMaxLength(100);
	common_inputs.push_back(line4);

	QGridLayout *grid = new QGridLayout;
	grid->addWidget(type, 0, 0, 1, 1);
	grid->addWidget(common_combo_3, 0, 1, 1, 1);
	for (int i = 0; i < 4; i++) {
		grid->addWidget(common_labels[i], i + 1, 0, 1, 1);
		grid->addWidget(common_inputs[i], i + 1, 1, 1, 1);
	}

	if (this->search_result_1[row][0] == "book") {
		common_combo_3->setCurrentIndex(0);
		QLineEdit *line5 = new QLineEdit;
		line5->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
		common_inputs.push_back(line5);
		QLineEdit *line6 = new QLineEdit;
		line6->setMaxLength(13);
		common_inputs.push_back(line6);
		QLineEdit *line7 = new QLineEdit;
		line7->setMaxLength(30);
		common_inputs.push_back(line7);
		QLineEdit *line8 = new QLineEdit;
		line8->setMaxLength(30);
		common_inputs.push_back(line8);
		QLabel *msg5 = new QLabel;
		msg5->setText(QString::fromWCharArray(L"折扣："));
		common_labels.push_back(msg5);
		QLabel *msg6 = new QLabel;
		msg6->setText(tr("ISBN:"));
		common_labels.push_back(msg6);
		QLabel *msg7 = new QLabel;
		msg7->setText(QString::fromWCharArray(L"作者："));
		common_labels.push_back(msg7);
		QLabel *msg8 = new QLabel;
		msg8->setText(QString::fromWCharArray(L"出版商："));
		common_labels.push_back(msg8);
		for (int i = 0; i < common_labels.size(); i++)
			common_inputs[i]->setText(QString::fromStdString(search_result_1[row][i + 1]));
	}
	else if (this->search_result_1[row][0] == "food") {
		common_combo_3->setCurrentIndex(1);
		QLineEdit *line5 = new QLineEdit;
		line5->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
		common_inputs.push_back(line5);
		QLineEdit *line6 = new QLineEdit;
		line6->setMaxLength(30);
		common_inputs.push_back(line6);
		QLabel *msg5 = new QLabel;
		msg5->setText(QString::fromWCharArray(L"折扣："));
		common_labels.push_back(msg5);
		QLabel *msg6 = new QLabel;
		msg6->setText(QString::fromWCharArray(L"过期日期："));
		common_labels.push_back(msg6);
		for (int i = 0; i < common_labels.size(); i++)
			common_inputs[i]->setText(QString::fromStdString(search_result_1[row][i + 1]));
	}
	else if (this->search_result_1[row][0] == "clothes") {
		common_combo_3->setCurrentIndex(2);
		QLineEdit *line5 = new QLineEdit;
		line5->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
		common_inputs.push_back(line5);
		QLineEdit *line6 = new QLineEdit;
		line6->setMaxLength(30);
		common_inputs.push_back(line6);
		QLineEdit *line7 = new QLineEdit;
		line7->setMaxLength(20);
		common_inputs.push_back(line7);
		QLabel *msg5 = new QLabel;
		msg5->setText(QString::fromWCharArray(L"折扣："));
		common_labels.push_back(msg5);
		QLabel *msg6 = new QLabel;
		msg6->setText(QString::fromWCharArray(L"品种："));
		common_labels.push_back(msg6);
		QLabel *msg7 = new QLabel;
		msg7->setText(QString::fromWCharArray(L"尺寸："));
		common_labels.push_back(msg7);
		for (int i = 0; i < common_labels.size(); i++)
			common_inputs[i]->setText(QString::fromStdString(search_result_1[row][i + 1]));
	}

	for (int i = NUM_ATTR_MERCH - 1; i < common_inputs.size(); i++) {
		grid->addWidget(common_labels[i], i + 1, 0, 1, 1);
		grid->addWidget(common_inputs[i], i + 1, 1, 1, 1);
	}

	QVBoxLayout *right = new QVBoxLayout;

	common_combo_3->setEnabled(false);
	common_inputs[0]->setEnabled(false);
	if (store.IfUser()) {
		QPushButton *favour = new QPushButton;
		favour->setText(QString::fromWCharArray(L"加入收藏夹"));
		QPushButton *order = new QPushButton;
		order->setText(QString::fromWCharArray(L"加入购物车"));
		right->addWidget(favour);
		right->addWidget(order);

		connect(favour, &QPushButton::clicked, this, [=] {Favour(); });
		connect(order, &QPushButton::clicked, this, [&] {Order(1); });
	}
	if (store.IfAdmin()) {
		QPushButton *edit = new QPushButton;
		edit->setText(QString::fromWCharArray(L"编辑"));
		right->addWidget(edit);
		
		connect(edit, &QPushButton::clicked, this, [=] {Edit(); });
	}
	if (!store.IfAdmin()) {
		for (int i = 1; i < common_inputs.size(); i++)
			common_inputs[i]->setEnabled(false);
	}

	QHBoxLayout *hori = new QHBoxLayout;
	hori->addLayout(grid);
	hori->addLayout(right);

	add_dlg->setLayout(hori);
	add_dlg->setModal(true);

	add_dlg->exec();
	ClearLayout(hori);
	delete hori;
	add_dlg->close();
	delete add_dlg;
	common_labels.clear();
	common_inputs.clear();
	return;
}

void StoreQT::Edit() {
	vector<string> item;
	item.push_back(common_combo_3->currentText().toStdString());
	for (int i = 0; i < common_inputs.size(); i++)
		item.push_back(common_inputs[i]->text().toStdString());

	int success = store.EditMerchan(item);
	if (success == SUCCESS) {
		QMessageBox::information(this, 
			QString::fromWCharArray(L"成功"), 
			QString::fromWCharArray(L"编辑成功")
		);
	}
	else {
		QMessageBox::information(this, 
			QString::fromWCharArray(L"失败"), 
			QString::fromWCharArray(L"似乎出现了一些问题")
		);
	}
	Search();
}

void StoreQT::Favour() {
	QModelIndexList selection = common_table_1->selectionModel()->selectedRows();
	if (selection.size() <= 0)
		return;
	int row = selection.at(0).row();

	string kind = this->search_result_1[row][0];
	string name = this->search_result_1[row][1];
	int success = store.Favour(kind, name);
	if (success == SUCCESS)
		QMessageBox::information(this, 
			QString::fromWCharArray(L"成功"), 
			QString::fromWCharArray(L"成功加入收藏夹")
		);
	else 
		QMessageBox::information(this, 
			QString::fromWCharArray(L"失败"), 
			QString::fromWCharArray(L"你已经收藏过此类商品了")
		);
}

void StoreQT::CheckFavour() {
	this->search_result_2.clear();
	this->search_result_2 = store.CheckFavour();

	common_table_2 = new QTableWidget;
	common_table_2->setColumnCount(NUM_ATTR_MERCH);
	common_table_2->setHorizontalHeaderLabels(*common_string);
	common_table_2->verticalHeader()->setVisible(false);
	common_table_2->setSelectionBehavior(QAbstractItemView::SelectRows);
	common_table_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
	common_table_2->setSelectionMode(QAbstractItemView::SingleSelection);
	for (int i = 0; i < this->search_result_2.size(); i++) {
		int temp = common_table_2->rowCount();
		common_table_2->insertRow(temp);
		for (int j = 0; j < NUM_ATTR_MERCH; j++) {
			common_table_2->setItem(i, 
				j, 
				new QTableWidgetItem(
					QString::fromStdString(this->search_result_2[i][j])
				)
			);
		}
	}

	QDialog *favour_dlg = new QDialog(this);
	favour_dlg->setWindowTitle(QString::fromWCharArray(L"收藏夹"));

	QPushButton *cancel = new QPushButton;
	cancel->setText(QString::fromWCharArray(L"移除"));
	QPushButton *order = new QPushButton;
	order->setText(QString::fromWCharArray(L"加入购物车"));

	QVBoxLayout *right = new QVBoxLayout;
	right->addWidget(order);
	right->addWidget(cancel);

	QHBoxLayout *hori = new QHBoxLayout;
	hori->addWidget(common_table_2);
	hori->addLayout(right);

	favour_dlg->setLayout(hori);
	favour_dlg->setModal(true);

	connect(order, &QPushButton::clicked, this, 
		[&] {
		Order(2);
		RefreshTable2();
	});
	connect(cancel, &QPushButton::clicked, this,
		[&] {
		QModelIndexList selection = common_table_2->selectionModel()->selectedRows();
		if (selection.size() <= 0)
			return;

		int row = selection.at(0).row();
		store.CancelFavour(search_result_2[row][0], search_result_2[row][1]);
		search_result_2.erase(search_result_2.begin() + row);
		RefreshTable2();
	});

	favour_dlg->exec();
	ClearLayout(hori);
	delete hori;
	favour_dlg->close();
	delete favour_dlg;
	return;
}

void StoreQT::Order(int index_result) {
	vector<vector<string>> *target_table;
	int row;

	if (index_result == 1) {
		target_table = &(this->search_result_1);
		QModelIndexList selection = common_table_1->selectionModel()->selectedRows();
		if (selection.size() <= 0)
			return;

		row = selection.at(0).row();
	}
	else if (index_result == 2) {
		target_table = &(this->search_result_2);
		QModelIndexList selection = common_table_2->selectionModel()->selectedRows();
		if (selection.size() <= 0)
			return;

		row = selection.at(0).row();
	}

	QDialog *set_amount = new QDialog;
	common_spin = new QSpinBox;
	common_spin->setRange(1, stoi((*target_table)[row][3]));
	common_spin->setSingleStep(1);

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"加入购物车"));

	QHBoxLayout *temp = new QHBoxLayout;
	temp->addWidget(common_spin);
	temp->addWidget(confirm);
	set_amount->setLayout(temp);
	set_amount->setModal(true);

	connect(confirm, &QPushButton::clicked, this, 
		[=, &target_table] {
		int success = 
			store.Order(
				(*target_table)[row][0], 
				(*target_table)[row][1], 
				this->common_spin->value()
			);

		if (success == SUCCESS) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"成功"), 
				QString::fromWCharArray(L"成功加入购物车")
			);

			(*target_table)[0][3] = 
				to_string(stoi((*target_table)[0][3]) 
				- this->common_spin->value());

			set_amount->close();
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"似乎出现了一些错误")
			);
		}
	});

	set_amount->exec();
	ClearLayout(temp);
	delete temp;
	set_amount->close();
	delete set_amount;
}

void StoreQT::CheckOrder() {
	this->search_result_2.clear();
	this->search_result_2 = store.CheckOrder();

	common_table_2 = new QTableWidget;
	common_table_2->setColumnCount(NUM_ATTR_MERCH + 2);
	QStringList *head = new QStringList;
	*head << QString::fromWCharArray(L"种类") 
		<< QString::fromWCharArray(L"商品名称") 
		<< QString::fromWCharArray(L"原价")
		<< QString::fromWCharArray(L"剩余数量") 
		<< QString::fromWCharArray(L"简介") 
		<< QString::fromWCharArray(L"折扣") 
		<< QString::fromWCharArray(L"总价");

	common_table_2->setHorizontalHeaderLabels(*head);
	common_table_2->verticalHeader()->setVisible(false);
	common_table_2->setSelectionBehavior(QAbstractItemView::SelectRows);
	common_table_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
	common_table_2->setSelectionMode(QAbstractItemView::SingleSelection);
	final_price.clear();
	final_price.push_back(0.0);

	for (int i = 0; i < this->search_result_2.size(); i++) {
		int temp = common_table_2->rowCount();
		common_table_2->insertRow(temp);
		for (int j = 0; j < NUM_ATTR_MERCH + 1; j++) {
			common_table_2->setItem(i, 
				j, 
				new QTableWidgetItem(
					QString::fromStdString(this->search_result_2[i][j])
				)
			);
		}
		if (this->search_result_2[i][0] == "book") {
			Book b(this->search_result_2[i], nullptr);
			float price = b.GetPrice();

			common_table_2->setItem(i, 
				NUM_ATTR_MERCH + 1, 
				new QTableWidgetItem(
					QString::fromStdString(to_string(price))
				)
			);

			final_price.push_back(price);
		}
		else if (this->search_result_2[i][0] == "food") {
			Food f(this->search_result_2[i], nullptr);
			float price = f.GetPrice();

			common_table_2->setItem(i, 
				NUM_ATTR_MERCH + 1, 
				new QTableWidgetItem(
					QString::fromStdString(to_string(price))
				)
			);
			
			final_price.push_back(price);
		}
		else if (this->search_result_2[i][0] == "clothes") {
			Clothes c(this->search_result_2[i], nullptr);
			float price = c.GetPrice();

			common_table_2->setItem(i, 
				NUM_ATTR_MERCH + 1, 
				new QTableWidgetItem(
					QString::fromStdString(to_string(price))
				)
			);
			
			final_price.push_back(price);
		}
	}

	QDialog *order_dlg = new QDialog(this);
	order_dlg->setWindowTitle(QString::fromWCharArray(L"设置数量"));

	QLabel *price = new QLabel;
	price->setText(QString::fromWCharArray(L"总价：") 
		+ QString::fromStdString(
			to_string(accumulate(final_price.begin(), final_price.end(), 0))
		)
	);
	QLabel *promotion = new QLabel;
	QLabel *total = new QLabel;

	if (accumulate(final_price.begin(), final_price.end(), 0) > this->promotion_bound) {
		promotion->setText(
			QString::fromWCharArray(L"满减：") 
			+ QString::fromStdString(to_string(this->promotion_amount))
		);
		total->setText(
			QString::fromWCharArray(L"最终价格：") 
			+ QString::fromStdString(to_string(
					accumulate(final_price.begin(), final_price.end(), 0) 
					- this->promotion_amount)
			)
		);
	}
	else {
		promotion->setText(QString::fromWCharArray(L"满减：0.00"));
		total->setText(QString::fromWCharArray(L"最终价格：") 
			+ QString::fromStdString(to_string(
				accumulate(final_price.begin(), final_price.end(), 0))
			)
		);
	}

	QPushButton *cancel = new QPushButton;
	cancel->setText(QString::fromWCharArray(L"移除"));
	QPushButton *pay = new QPushButton;
	pay->setText(QString::fromWCharArray(L"去支付"));

	QVBoxLayout *right = new QVBoxLayout;
	right->addWidget(price);
	right->addWidget(promotion);
	right->addWidget(total);
	
	right->addWidget(pay);
	right->addWidget(cancel);

	QHBoxLayout *hori = new QHBoxLayout;
	hori->addWidget(common_table_2);
	hori->addLayout(right);

	order_dlg->setLayout(hori);
	order_dlg->setModal(true);

	connect(pay, &QPushButton::clicked, this, [&] {CheckBankCard(); });
	connect(cancel, &QPushButton::clicked, this,
		[&] {
		QModelIndexList selection = common_table_2->selectionModel()->selectedRows();
		if (selection.size() <= 0)
			return;
		int row = selection.at(0).row();
		store.CancelOrder(search_result_2[row][0], search_result_2[row][1]);
		search_result_2.erase(search_result_2.begin() + row);
		final_price.erase(final_price.begin() + row);

		QMessageBox::information(this, 
			QString::fromWCharArray(L"成功"), 
			QString::fromWCharArray(L"成功移除商品")
		);
		order_dlg->close();
		CheckOrder();
	});

	order_dlg->exec();
	final_price.clear();
	ClearLayout(hori);
	delete hori;
	order_dlg->close();
	delete order_dlg;
	return;
}

void StoreQT::CheckBankCard() {
	this->search_result_3.clear();
	this->search_result_3 = store.CheckOrder();

	common_table_3 = new QTableWidget;
	common_table_3->setColumnCount(2);
	QStringList *head = new QStringList;
	*head << QString::fromWCharArray(L"银行名称") << QString::fromWCharArray(L"卡号");
	common_table_3->setHorizontalHeaderLabels(*head);
	common_table_3->verticalHeader()->setVisible(false);
	common_table_3->setSelectionBehavior(QAbstractItemView::SelectRows);
	common_table_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
	common_table_3->setSelectionMode(QAbstractItemView::SingleSelection);
	search_result_3 = store.CheckBankCard();

	for (int i = 0; i < this->search_result_3.size(); i++) {
		int temp = common_table_3->rowCount();
		common_table_3->insertRow(temp);
		for (int j = 0; j < 2; j++) {
			common_table_3->setItem(i, 
				j, 
				new QTableWidgetItem(
					QString::fromStdString(this->search_result_3[i][j])
				)
			);
		}
	}

	QDialog *card_dlg = new QDialog(this);
	card_dlg->setWindowTitle(QString::fromWCharArray(L"已绑定银行卡"));

	QPushButton *add = new QPushButton;
	add->setText(QString::fromWCharArray(L"添加"));
	QPushButton *cancel = new QPushButton;
	cancel->setText(QString::fromWCharArray(L"移除"));
	QPushButton *pay = new QPushButton;
	pay->setText(QString::fromWCharArray(L"去支付"));
	if (accumulate(final_price.begin(), final_price.end(), 0) <= 0)
		pay->setEnabled(false);

	QVBoxLayout *right = new QVBoxLayout;
	right->addWidget(add);
	right->addWidget(pay);
	right->addWidget(cancel);

	QHBoxLayout *hori = new QHBoxLayout;
	hori->addWidget(common_table_3);
	hori->addLayout(right);

	card_dlg->setLayout(hori);
	card_dlg->setModal(true);

	connect(add, &QPushButton::clicked, this,
		[=] {
		BindCard_1(); 
		card_dlg->close();
		CheckBankCard();
	});
	connect(pay, &QPushButton::clicked, this,
		[&] {
		Pay();
		card_dlg->close();
	});
	connect(cancel, &QPushButton::clicked, this,
		[&] {
		QModelIndexList selection = common_table_3->selectionModel()->selectedRows();
		if (selection.size() <= 0)
			return;
		int row = selection.at(0).row();
		store.CancelCard(search_result_3[row][0], stoll(search_result_3[row][1]));
		search_result_3.erase(search_result_3.begin() + row);
		QMessageBox::information(this, 
			QString::fromWCharArray(L"成功"), 
			QString::fromWCharArray(L"成功移除")
		);
		card_dlg->close();
		CheckBankCard();
	});

	card_dlg->exec();
	ClearLayout(hori);
	delete hori;
	card_dlg->close();
	delete card_dlg;
	return;
}

void StoreQT::Pay() {
	QModelIndexList selection = common_table_3->selectionModel()->selectedRows();
	if (selection.size() <= 0)
		return;
	QDialog *temp = new QDialog;
	temp->setWindowTitle(QString::fromWCharArray(L"支付"));

	QLabel *hint = new QLabel;
	hint->setText(QString::fromWCharArray(L"请输入银行卡密码："));
	temp_line_1 = new QLineEdit;
	temp_line_1->setEchoMode(QLineEdit::Password);
	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"确认"));

	QHBoxLayout *aaa = new QHBoxLayout;
	aaa->addWidget(hint);
	aaa->addWidget(temp_line_1);
	aaa->addWidget(confirm);

	temp->setLayout(aaa);

	connect(confirm, &QPushButton::clicked, this, 
		[=] {
		QModelIndexList selection = common_table_3->selectionModel()->selectedRows();
		if (selection.size() <= 0)
			return;

		int row = selection.at(0).row();
		this->tcp_state = 2;
		float total = accumulate(final_price.begin(), final_price.end(), 0);

		if (total > this->promotion_bound)
			total -= this->promotion_amount;

		PayCommuicate(search_result_3[row][0], 
			stoll(search_result_3[row][1]),
			temp_line_1->text().toStdString(), total);
		temp->close();
	});
	temp->exec();
	ClearLayout(aaa);
	delete aaa;
	temp->close();
	delete temp;
	return;
}

StoreQT::~StoreQT() {
	ClearLayout(main_layout);
	delete main_layout;
}

// ------------------------- Communication Part -------------------------

void StoreQT::ConnectUp() {
	QMessageBox::information(this, 
		QString::fromWCharArray(L"提示"), 
		QString::fromWCharArray(L"尝试连接银行...")
	);

	this->blockSize = 0;
	this->tcpSocket = new QTcpSocket(this);
	this->tcpSocket->connectToHost(QHostAddress::LocalHost, 6000);
	bool connected = this->tcpSocket->waitForConnected();

	QMessageBox::information(this, 
		QString::fromWCharArray(L"连接状态"), 
		QString::fromStdString(to_string(connected))
	);

	connect(this->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(ConnectionError(QAbstractSocket::SocketError)));
	connect(this->tcpSocket, SIGNAL(readyRead()), this, SLOT(ReceiveMsg()));
}

void StoreQT::ConnectionError(QAbstractSocket::SocketError) {
	QMessageBox::information(this, 
		QString::fromWCharArray(L"连接失败"), 
		this->tcpSocket->errorString()
	);
}

void StoreQT::BindCard_1() {
	if (this->tcpSocket->state() != QTcpSocket::ConnectedState) {
		QMessageBox::information(this, 
			QString::fromWCharArray(L"失败"), 
			QString::fromWCharArray(L"连接银行系统失败")
		);
		return;
	}
	tcp_dlg = new QDialog;
	tcp_dlg->setWindowTitle(QString::fromWCharArray(L"绑定银行卡"));
	
	QLabel *hint = new QLabel;
	hint->setText(QString::fromWCharArray(L"请选择一个银行："));

	tcp_h_layout = new QHBoxLayout;
	tcp_h_layout->addWidget(hint);

	this->tcp_state = 1;
	char *msg = "1";
	this->tcpSocket->write(msg);

	tcp_dlg->setLayout(tcp_h_layout);

	tcp_dlg->exec();
	ClearLayout(tcp_h_layout);
	delete tcp_h_layout;
	tcp_dlg->close();
	delete tcp_dlg;
}

void StoreQT::BindCard_2(string bank_name) {
	ClearLayout(tcp_h_layout);
	this->remember = bank_name;

	QLabel *account = new QLabel;
	account->setText(QString::fromWCharArray(L"银行账号："));
	temp_line_1 = new QLineEdit;
	QHBoxLayout *upper = new QHBoxLayout;
	upper->addWidget(account);
	upper->addWidget(temp_line_1);

	QLabel *pass = new QLabel;
	pass->setText(QString::fromWCharArray(L"银行密码："));
	temp_line_2 = new QLineEdit;
	temp_line_2->setEchoMode(QLineEdit::Password);
	QHBoxLayout *down = new QHBoxLayout;
	down->addWidget(pass);
	down->addWidget(temp_line_2);

	QPushButton *confirm = new QPushButton;
	confirm->setText(QString::fromWCharArray(L"绑定"));
	QPushButton *cancel = new QPushButton;
	cancel->setText(QString::fromWCharArray(L"取消"));
	QHBoxLayout *button = new QHBoxLayout;
	button->addWidget(confirm);
	button->addWidget(cancel);

	QVBoxLayout *all = new QVBoxLayout;
	all->addLayout(upper);
	all->addLayout(down);
	all->addLayout(button);

	tcp_h_layout->addLayout(all);

	connect(confirm, &QPushButton::clicked, this,
		[=] {
		if (!(this->tcpSocket->state() == QTcpSocket::ConnectedState)) {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"连接银行系统失败")
			);
			tcp_dlg->close();
		}
		else {
			this->tcp_state = 2;
			float amount = 0.0;

			string msg_s = "0";
			msg_s += "," + remember;
			msg_s += "," + temp_line_1->text().toStdString();
			msg_s += "," + temp_line_2->text().toStdString();
			msg_s += "," + to_string(amount);
			const char *msg = msg_s.data();
			this->tcpSocket->write(msg);
		}
	});

	connect(cancel, &QPushButton::clicked, this,
		[=] {
		tcp_dlg->close();
	});
}

void StoreQT::PayCommuicate(string bank, long long card_number, string password, float amount) {
	if (!(this->tcpSocket->state() == QTcpSocket::ConnectedState)) {
		QMessageBox::information(this, Q
			String::fromWCharArray(L"失败"), 
			QString::fromWCharArray(L"连接银行系统失败")
		);
		return;
	}

	string msg_s = "0";
	msg_s += "," + bank;
	msg_s += "," + to_string(card_number);
	msg_s += "," + password;
	msg_s += "," + to_string(amount);
	const char *msg = msg_s.data();
	this->tcpSocket->write(msg);
}

void StoreQT::ReceiveMsg() {
	QString temp;
	temp = this->tcpSocket->readAll();

	vector<string> result = StringParse(temp.toStdString(), ",");
	int answer = stoi(result[0]);

	switch (this->tcp_state) {
	case 1: {
		// answer for bank list
		if (answer <= 0)
			QMessageBox::information(this, 
				QString::fromWCharArray(L"提示"), Q
				String::fromWCharArray(L"银行都倒闭了！")
			);
		else {
			vector<string> bank_list;
			for (int i = 0; i < answer; i++) {
				bank_list.push_back(result[i + 1]);
			}

			QPushButton **bank_in = new QPushButton*[bank_list.size()];
			tcp_h_layout->addStretch();

			for (int i = 0; i < bank_list.size(); i++) {
				bank_in[i] = new QPushButton;
				bank_in[i]->setText(QString::fromStdString(bank_list[i]));
				tcp_h_layout->addWidget(bank_in[i]);
				tcp_h_layout->addStretch();

				connect(bank_in[i], &QPushButton::clicked, this, [=] {BindCard_2(bank_list[i]); });
			}
		}
		this->tcp_state = -1;
		break;
	}
	case 2: {
		// request bank list
		if (answer == SUCCESS) {
			if (accumulate(final_price.begin(), final_price.end(), 0.0) <= 0) {
				store.BindBankCard(remember, 
					stoll(temp_line_1->text().toStdString())
				);

				QMessageBox::information(this, 
					QString::fromWCharArray(L"成功"), 
					QString::fromWCharArray(L"绑定成功")
				);

				this->tcp_dlg->close();
			}
			else {
				QMessageBox::information(this, 
					QString::fromWCharArray(L"成功"), 
					QString::fromWCharArray(L"支付成功")
				);

				for (int i = 0; i < final_price.size() - 1; i++) {
					store.CancelOrder(search_result_2[i][0], search_result_2[i][1]);
					search_result_2.erase(search_result_2.begin());
					final_price.erase(final_price.begin() + 1);
				}
			}
		}
		else {
			QMessageBox::information(this, 
				QString::fromWCharArray(L"失败"), 
				QString::fromWCharArray(L"密码错误")
			);

			if (accumulate(final_price.begin(), final_price.end(), 0.0) <= 0)
				this->tcp_dlg->close();
		}
		this->tcp_state = -1;
	}
	default: {
		break;
	}
	}
}