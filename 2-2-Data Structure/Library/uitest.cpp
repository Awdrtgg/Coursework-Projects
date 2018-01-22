#include "uitest.h"

Library library;
Search_Result b;
Time *currentTime;

//---------------------Users----------------------------------

Infodlg::Infodlg()
{
	nameLabel = new QLabel;
	IDLabel = new QLabel;
	typeLabel = new QLabel;
	schoolLabel = new QLabel;
	honestyLabel = new QLabel;
	borrowlimitLabel = new QLabel;
	borrowednumLabel = new QLabel;
	reservelimLabel = new QLabel;
	reservesLabel = new QLabel;

	nameLabel->setStyleSheet("font-size:30px;color:black");
	IDLabel->setStyleSheet("font-size:30px;color:black");
	typeLabel->setStyleSheet("font-size:30px;color:black");
	schoolLabel->setStyleSheet("font-size:30px;color:black");
	honestyLabel->setStyleSheet("font-size:30px;color:black");
	borrowlimitLabel->setStyleSheet("font-size:30px;color:black");
	borrowednumLabel->setStyleSheet("font-size:30px;color:black");
	reservelimLabel->setStyleSheet("font-size:30px;color:black");
	reservesLabel->setStyleSheet("font-size:30px;color:black");

	changepasswordBtn = new QPushButton;
	changepasswordBtn->setText(QString::fromWCharArray(L"修改密码"));

	mainLayout = new QVBoxLayout;
	mainLayout->addSpacing(HEIGHT / 10);
	mainLayout->addWidget(nameLabel);
	mainLayout->addWidget(IDLabel);
	mainLayout->addWidget(typeLabel);
	mainLayout->addWidget(schoolLabel);
	mainLayout->addWidget(honestyLabel);
	mainLayout->addWidget(borrowlimitLabel);
	mainLayout->addWidget(borrowednumLabel);
	mainLayout->addWidget(reservelimLabel);
	mainLayout->addWidget(reservesLabel);
	mainLayout->addWidget(changepasswordBtn);
	mainLayout->addSpacing(HEIGHT / 10);
	// mainLayout->setSpacing(HEIGHT/50);

	QHBoxLayout *mainl = new QHBoxLayout(this);
	mainl->addSpacing(WIDTH / 3);
	mainl->addLayout(mainLayout);
	mainl->addSpacing(WIDTH / 2 - WIDTH / 10);

	connect(changepasswordBtn, SIGNAL(clicked()), this, SLOT(ChangePassWord()));
}
void Infodlg::UserInfo()
{
	nameLabel->setText(QString::fromWCharArray(L"姓名:") + QString::fromStdString(library.currentUser->real_name));
	IDLabel->setText(QString::fromWCharArray(L"学号:") + QString::fromStdString(library.currentUser->user_name));
	typeLabel->setText(QString::fromWCharArray(L"类型:") + (library.currentUser->type == STUDENT ? QString::fromWCharArray(L"学生") : QString::fromWCharArray(L"教师")));
	schoolLabel->setText(QString::fromWCharArray(L"学院:") + QString::fromStdString(library.currentUser->school));
	honestyLabel->setText(QString::fromWCharArray(L"诚信值:") + QString::number(library.currentUser->honesty));
	borrowlimitLabel->setText(QString::fromWCharArray(L"借书上限:") + QString::number(library.currentUser->type == STUDENT ? STU_LIMIT : TEACHER_LIMIT));
	borrowednumLabel->setText(QString::fromWCharArray(L"已借数量:") + QString::number(library.currentUser->borrowed_book.size(), 10));
	reservelimLabel->setText(QString::fromWCharArray(L"预约上限:") + QString::number(MAX_ORDER_NUM));
	reservesLabel->setText(QString::fromWCharArray(L"预约已到/已预约:") + QString::number(library.currentUser->v_reserved_book.size(), 10) + "/" + QString::number(library.currentUser->reserved_book.size(), 10));
}
void Infodlg::ChangePassWord()
{
	confirmdlg = new Confirmpassdlg;
	confirmdlg->show();
}
Infodlg::~Infodlg()
{

}

Confirmpassdlg::Confirmpassdlg()
{
	setWindowTitle(QString::fromWCharArray(L" "));
	passwordLabel = new QLabel;
	passwordLabel->setText(QString::fromWCharArray(L"请输入当前密码："));
	passwordLine = new QLineEdit;
	okBtn = new QPushButton;
	okBtn->setText(QString::fromWCharArray(L"确定"));
	cancelBtn = new QPushButton;
	cancelBtn->setText(QString::fromWCharArray(L"取消"));

	passwordLayout = new QHBoxLayout;
	passwordLayout->addWidget(passwordLabel);
	passwordLayout->addWidget(passwordLine);
	btnnLayout = new QHBoxLayout;
	btnnLayout->addWidget(okBtn);
	btnnLayout->addWidget(cancelBtn);
	mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(passwordLayout);
	mainLayout->addLayout(btnnLayout);

	this->show();

	connect(okBtn, SIGNAL(clicked()), this, SLOT(Confirm()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
}
void Confirmpassdlg::Confirm()
{
	string password = passwordLine->text().toStdString();
	if ((library.currentUser != NULL && password == library.currentUser->password) || (library.currentAdmin != NULL && password == library.currentAdmin->password))
	{
		this->close();
		changedlg = new Changepassdlg;
		changedlg->show();
	}
	else
	{
		QMessageBox::critical(this, QString::fromWCharArray(L"失败"), QString::fromWCharArray(L"密码错误！"));
		this->close();
	}
}
Confirmpassdlg::~Confirmpassdlg()
{

}

Changepassdlg::Changepassdlg()
{
	passwordLabel = new QLabel;
	passwordLabel->setText(QString::fromWCharArray(L"输入新密码"));
	repasswordLabel = new QLabel;
	repasswordLabel->setText(QString::fromWCharArray(L"确认新密码"));
	passwordLine = new QLineEdit;
	repasswordLine = new QLineEdit;
	okBtn = new QPushButton;
	okBtn->setText(QString::fromWCharArray(L"确定"));
	cancelBtn = new QPushButton;
	cancelBtn->setText(QString::fromWCharArray(L"取消"));

	passwordLayout = new QHBoxLayout;
	passwordLayout->addWidget(passwordLabel);
	passwordLayout->addWidget(passwordLine);
	repasswordLayout = new QHBoxLayout;
	repasswordLayout->addWidget(repasswordLabel);
	repasswordLayout->addWidget(repasswordLine);
	btnnLayout = new QHBoxLayout;
	btnnLayout->addWidget(okBtn);
	btnnLayout->addWidget(cancelBtn);
	mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(passwordLayout);
	mainLayout->addLayout(repasswordLayout);
	mainLayout->addLayout(btnnLayout);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(Change()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
}
void Changepassdlg::Change()
{
	string password1 = this->passwordLine->text().toStdString();
	string password2 = this->repasswordLine->text().toStdString();
	if (!(password1 == password2))   QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"两次输入的密码不一致"));
	else
	{
		int result;
		if (library.currentUser != NULL)
		{
			result = library.UserChangePassword(password1);
			if (result == FAIL)   QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"密码过长"));
			else
			{
				this->close();
				QMessageBox::information(this, QString::fromWCharArray(L"修改成功"), QString::fromWCharArray(L"修改成功"));
			}

		}
		else if (library.currentAdmin != NULL)
		{
			result = library.AdminChangePassword(password1);
			if (result == FAIL)   QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"密码过长"));
			else
			{
				QMessageBox::information(this, QString::fromWCharArray(L"修改成功"), QString::fromWCharArray(L"修改成功"));
				this->close();
			}
		}
	}
}
Changepassdlg::~Changepassdlg()
{

}

Searchdlg::Searchdlg()
{
	setWindowTitle(QString::fromWCharArray(L"搜索"));

	CreateResults();
	CreateSearch();
	CreateButtons();
	CreateChangeDlg();
	CreateCheckBox();

	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(searchWidget, 0, 0, 1, 2);
	mainLayout->addWidget(resultTable, 1, 0, 1, 2);
	mainLayout->addWidget(btnDlg, 2, 1);

	// mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->setSpacing(10);
}
void Searchdlg::CreateSearch()
{
	searchWidget = new QWidget;

	keywordLabel = new QLabel;
	keywordLabel->setText(QString::fromWCharArray(L"关键字:"));
	keywordLabel->setStyleSheet("color:black");

	typeCombo = new QComboBox;
	typeCombo->insertItem(0, QString::fromWCharArray(L"书籍名称"));
	typeCombo->insertItem(1, QString::fromWCharArray(L"出版社"));
	typeCombo->insertItem(2, QString::fromWCharArray(L"作者/译者"));
	typeCombo->insertItem(3, QString::fromWCharArray(L"ISBN编号"));
	typeCombo->insertItem(4, QString::fromWCharArray(L"标签"));

	valueLine = new QLineEdit;
	searchBtn = new QPushButton;
	searchBtn->setText(QString::fromWCharArray(L"搜索"));

	QHBoxLayout *mainLayout = new QHBoxLayout(searchWidget);
	mainLayout->addSpacing(WIDTH / 5);
	mainLayout->addWidget(keywordLabel);
	mainLayout->addWidget(typeCombo);
	mainLayout->addSpacing(WIDTH / 20);
	mainLayout->addWidget(valueLine);
	mainLayout->addSpacing(WIDTH / 20);
	mainLayout->addWidget(searchBtn);
	mainLayout->addSpacing(WIDTH / 5);
	//mainLayout->setSpacing(WIDTH/20);

	connect(searchBtn, SIGNAL(clicked()), this, SLOT(Search()));
	connect(valueLine, SIGNAL(returnPressed()), this, SLOT(Search()));
}
void Searchdlg::CreateResults()
{
	resultTable = new QTableWidget;
	resultTable->setRowCount(3);
	resultTable->setColumnCount(9);

	QStringList header;
	header << QString::fromWCharArray(L"书名") << QString::fromWCharArray(L"出版社") << QString::fromWCharArray(L"作者") << QString::fromWCharArray(L"译者") << QString::fromWCharArray(L"标签") << QString::fromWCharArray(L"ISBN") << QString::fromWCharArray(L"购入时间") << QString::fromWCharArray(L"剩余数量") << QString::fromWCharArray(L"总数");
	resultTable->setHorizontalHeaderLabels(header);

	resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	resultTable->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选择
	resultTable->hide();
}
void Searchdlg::CreateButtons()
{
	btnDlg = new QDialog;
	btnDlg->setAttribute(Qt::WA_TranslucentBackground, true);

	firstBtn = new QPushButton;
	firstBtn->setText(QString::fromWCharArray(L"首页"));
	preBtn = new QPushButton;
	preBtn->setText(QString::fromWCharArray(L"上一页"));
	nextBtn = new QPushButton;
	nextBtn->setText(QString::fromWCharArray(L"下一页"));
	lastBtn = new QPushButton;
	lastBtn->setText(QString::fromWCharArray(L"尾页"));
	pageLabel = new QLabel;
	pageLabel->setStyleSheet("font-size:20px;color:black");
	jumpLabel1 = new QLabel;
	jumpLabel1->setText(QString::fromWCharArray(L"跳转至"));
	jumpLabel1->setStyleSheet("font-size:20px;color:black");
	jumpLine = new QLineEdit;
	jumpLabel2 = new QLabel;
	jumpLabel2->setText(QString::fromWCharArray(L"页"));
	jumpLabel2->setStyleSheet("font-size:20px;color:black");
	jumpBtn = new QPushButton;
	jumpBtn->setText(QString::fromWCharArray(L"跳转"));
	borrowBtn = new QPushButton;
	borrowBtn->setText(QString::fromWCharArray(L"借阅"));
	detailBtn = new QPushButton;
	detailBtn->setText(QString::fromWCharArray(L"详情"));

	QHBoxLayout *btnBox = new QHBoxLayout(btnDlg);
	QHBoxLayout *jumpBox = new QHBoxLayout(btnDlg);
	btnBox->addSpacing(WIDTH / 10);
	btnBox->addWidget(firstBtn);
	btnBox->addWidget(preBtn);
	btnBox->addWidget(nextBtn);
	btnBox->addWidget(lastBtn);
	btnBox->addWidget(pageLabel);
	jumpBox->addSpacing(10);
	jumpBox->addWidget(jumpLabel1);
	jumpBox->addWidget(jumpLine);
	jumpBox->addWidget(jumpLabel2);
	jumpBox->addWidget(jumpBtn);
	jumpBox->addSpacing(10);
	jumpBox->setSpacing(10);
	btnBox->addLayout(jumpBox);
	btnBox->addWidget(borrowBtn);
	btnBox->addWidget(detailBtn);
	btnBox->setSpacing(30);

	connect(firstBtn, SIGNAL(clicked()), this, SLOT(ShowFirstPage()));
	connect(preBtn, SIGNAL(clicked()), this, SLOT(ShowPrePage()));
	connect(nextBtn, SIGNAL(clicked()), this, SLOT(ShowNextPage()));
	connect(lastBtn, SIGNAL(clicked()), this, SLOT(ShowLastPage()));
	connect(jumpBtn, SIGNAL(clicked()), this, SLOT(ShowJumpedPage()));
	connect(borrowBtn, SIGNAL(clicked()), this, SLOT(Borrow()));
	connect(detailBtn, SIGNAL(clicked()), this, SLOT(ShowDetail()));

	btnDlg->hide();
}
void Searchdlg::CreateChangeDlg()
{

	change = new QDialog;
	change->setWindowIcon(QIcon(".\\css\\book.gif"));
	change->setWindowTitle(QString::fromWCharArray(L"修改信息"));
	nameLabel = new QLabel;
	nameLabel->setText(QString::fromWCharArray(L"书名:"));
	nameLine = new QLineEdit;
	org_nameLabel = new QLabel;
	org_nameLabel->setText(QString::fromWCharArray(L"原名:"));
	org_nameLine = new QLineEdit;
	publisherLabel = new QLabel;
	publisherLabel->setText(QString::fromWCharArray(L"出版社:"));
	publisherLine = new QLineEdit;
	writerLabel = new QLabel;
	writerLabel->setText(QString::fromWCharArray(L"作者:"));
	writerLine = new QLineEdit;
	translatorLabel = new QLabel;
	translatorLabel->setText(QString::fromWCharArray(L"译者:"));
	translatorLine = new QLineEdit;
	publish_timeLabel = new QLabel;
	publish_timeLabel->setText(QString::fromWCharArray(L"出版时间:"));
	publish_timeLine = new QLineEdit;
	introLabel = new QLabel;
	introLabel->setText(QString::fromWCharArray(L"简介:"));
	introTxt = new QTextEdit;
	key_wordLabel = new QLabel;
	key_wordLabel->setText(QString::fromWCharArray(L"标签:"));
	QHBoxLayout *keywordLayout = new QHBoxLayout;
	key_wordLine = new QLineEdit;
	key_wordLine->setEnabled(false);
	change_keywBtn = new QPushButton;
	change_keywBtn->setText(QString::fromWCharArray(L"修改"));
	keywordLayout->addWidget(key_wordLine);
	keywordLayout->addWidget(change_keywBtn);
	ratingLabel = new QLabel;
	ratingLabel->setText(QString::fromWCharArray(L"豆瓣评分:"));
	ratingLine = new QLineEdit;
	page_numLabel = new QLabel;
	page_numLabel->setText(QString::fromWCharArray(L"总页数:"));
	page_numLine = new QLineEdit;
	purchase_timeLabel = new QLabel;
	purchase_timeLabel->setText(QString::fromWCharArray(L"购入时间:"));
	purchase_timeLine = new QLineEdit;
	purchase_timeLine->setEnabled(false);
	// purchase_timeDate = new QDateEdit;
	// purchase_timeDate->setCalendarPopup(true);
	// purchase_timeDate->setDisplayFormat("yyyy/MM/dd");

	ISBNLabel = new QLabel;
	ISBNLabel->setText(QString::fromWCharArray(L"ISBN编号:"));
	ISBNLine = new QLineEdit;

	pic = new QLabel;
	changepicBtn = new QPushButton;
	changepicBtn->setText(QString::fromWCharArray(L"修改图片"));

	okBtn = new QPushButton;
	okBtn->setText(QString::fromWCharArray(L"确定"));
	cancelBtn = new QPushButton;
	cancelBtn->setText(QString::fromWCharArray(L"取消"));

	InfoLayout1 = new QVBoxLayout;
	InfoLayout1->addWidget(nameLabel);
	InfoLayout1->addWidget(writerLabel);
	InfoLayout1->addWidget(org_nameLabel);
	InfoLayout1->addWidget(translatorLabel);
	InfoLayout1->addWidget(key_wordLabel);
	InfoLayout1->addWidget(publish_timeLabel);
	InfoLayout1->addWidget(ratingLabel);
	InfoLayout1->addWidget(page_numLabel);
	InfoLayout1->addWidget(purchase_timeLabel);
	InfoLayout1->addWidget(ISBNLabel);


	InfoLayout2 = new QVBoxLayout;
	InfoLayout2->addWidget(nameLine);
	InfoLayout2->addWidget(writerLine);
	InfoLayout2->addWidget(org_nameLine);
	InfoLayout2->addWidget(translatorLine);
	InfoLayout2->addLayout(keywordLayout);
	InfoLayout2->addWidget(publish_timeLine);
	InfoLayout2->addWidget(ratingLine);
	InfoLayout2->addWidget(page_numLine);
	InfoLayout2->addWidget(purchase_timeLine);
	InfoLayout2->addWidget(ISBNLine);

	InfoLayout3 = new QHBoxLayout;
	InfoLayout3->addLayout(InfoLayout1);
	InfoLayout3->addLayout(InfoLayout2);

	InfoLayout = new QVBoxLayout;
	InfoLayout->addLayout(InfoLayout3);
	InfoLayout->addWidget(introLabel);
	InfoLayout->addWidget(introTxt);

	PicLayout = new QVBoxLayout;
	PicLayout->addWidget(pic);
	PicLayout->addWidget(changepicBtn);

	btnBox = new QHBoxLayout;
	btnBox->addWidget(okBtn);
	btnBox->addWidget(cancelBtn);

	QHBoxLayout *topLayout = new   QHBoxLayout;
	topLayout->addLayout(InfoLayout);
	topLayout->addLayout(PicLayout);

	QVBoxLayout *totalLayout = new QVBoxLayout(change);
	totalLayout->addLayout(topLayout);
	totalLayout->addLayout(btnBox);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(ChangeInfo()));
	connect(cancelBtn, SIGNAL(clicked()), change, SLOT(close()));
	connect(changepicBtn, SIGNAL(clicked()), this, SLOT(ChangePic()));
	connect(change_keywBtn, SIGNAL(clicked()), this, SLOT(ShowCheckBox()));

	change->hide();
}
void Searchdlg::CreateCheckBox()
{
	int i;
	CheboxDlg = new QDialog;
	check_mainLayout = new QGridLayout(CheboxDlg);
	for (i = 0; i < keywords.size(); i++)
	{
		QCheckBox *temp = new QCheckBox(CheboxDlg);
		temp->setText(QString::fromStdString(keywords[i]));
		checkboxs.push_back(temp);
		check_mainLayout->addWidget(checkboxs[i], i / 10, i % 10);
	}
	check_okBtn = new QPushButton;
	check_okBtn->setText(QString::fromWCharArray(L"确定"));
	check_cancelBtn = new QPushButton;
	check_cancelBtn->setText(QString::fromWCharArray(L"取消"));
	check_mainLayout->addWidget(check_okBtn, ++i / 10 + 1, 8);
	check_mainLayout->addWidget(check_cancelBtn, i / 10 + 1, 9);

	connect(check_okBtn, SIGNAL(clicked()), this, SLOT(ChangeKey()));
	connect(check_cancelBtn, SIGNAL(clicked()), CheboxDlg, SLOT(close()));

	CheboxDlg->hide();
}
void Searchdlg::Search()
{
	string type = this->typeCombo->currentText().toStdString();

	if (type == QString::fromWCharArray(L"ISBN编号").toStdString())
	{
		long long value = this->valueLine->text().toLongLong();
		b = library.Search(type, value);
	}
	else
	{
		string value = this->valueLine->text().toStdString();
		b = library.Search(type, value);
	}
	currentpage = 1;
	ShowSearchResult();
}
void Searchdlg::ShowSearchResult()
{
	resultTable->hide();
	int row;
	row = resultTable->rowCount();
	for (int i = 0; i<row; i++)
	{
		resultTable->removeRow(i);
	}
	resultTable->setRowCount(b.current_book.size());
	if (b == FAIL)
	{
		resultTable->hide();
		btnDlg->hide();
		QMessageBox::critical(this, QString::fromWCharArray(L"查询失败"), QString::fromWCharArray(L"找不到书籍"));
	}
	else
	{
		pages = b.book.size() / 20 + (b.book.size() % 20 > 0 ? 1 : 0);
		pageLabel->setText(QString::fromWCharArray(L"共") + QString::number(currentpage, 10) + " / " + QString::number(pages, 10) + QString::fromWCharArray(L"页"));
		for (row = 0; row<b.current_book.size(); row++)
		{
			resultTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(b.current_book[row]->name)));
			resultTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(b.current_book[row]->publisher)));
			resultTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(b.current_book[row]->writer)));
			resultTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(b.current_book[row]->translator)));
			resultTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString((string)(b.current_book[row]->key_word))));
			resultTable->setItem(row, 5, new QTableWidgetItem(QString::number(b.current_book[row]->ISBN, 10)));
			resultTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(b.current_book[row]->purchase_time.date())));
			resultTable->setItem(row, 7, new QTableWidgetItem(QString::number(b.current_book[row]->get_v1_num(), 10)));
			resultTable->setItem(row, 8, new QTableWidgetItem(QString::number(b.current_book[row]->total_num, 10)));
		}
		resultTable->show();
		btnDlg->show();
	}
}
void Searchdlg::ShowFirstPage()
{
	int result = b.FirstPage();
	if (result == SUCCESS)
	{
		currentpage = 1;
		ShowSearchResult();
	}

}
void Searchdlg::ShowPrePage()
{
	int result = b.PreviousPage();
	if (result == SUCCESS)
	{
		currentpage--;
		ShowSearchResult();
	}
	else    QMessageBox::critical(this, QString::fromWCharArray(L"跳转失败"), QString::fromWCharArray(L"已经是第一页"));
}
void Searchdlg::ShowNextPage()
{
	int result = b.NextPage();
	if (result == SUCCESS)
	{
		currentpage++;
		ShowSearchResult();
	}
	else    QMessageBox::critical(this, QString::fromWCharArray(L"跳转失败"), QString::fromWCharArray(L"已经是最后一页"));
}
void Searchdlg::ShowLastPage()
{
	int result = b.LastPage();
	if (result == SUCCESS)
	{
		currentpage = pages;
		ShowSearchResult();
	}
	else    QMessageBox::critical(this, QString::fromWCharArray(L"跳转失败"), QString::fromWCharArray(L"已经是最后一页"));
}
void Searchdlg::ShowJumpedPage()
{
	int page = jumpLine->text().toInt();
	int result = b.JumpPage(page);
	if (result == SUCCESS)
	{
		currentpage = page;
		ShowSearchResult();
	}
	else    QMessageBox::critical(this, QString::fromWCharArray(L"跳转失败"), QString::fromWCharArray(L"无该页，跳转失败！"));
}
void Searchdlg::Borrow()
{
	int row;
	if (borrowBtn->text() == QString::fromWCharArray(L"借阅"))
	{
		if (resultTable->currentItem() == NULL)
			QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"请先选择要借阅的书籍！"));
		else
		{
			row = resultTable->currentItem()->row();
			int result = library.Borrow(*b.current_book[row], DEFAULT_KEEP_TIME);
			switch (result) {
			case NO_LOGIN:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"请先登录！"));
				break;
			case DISHONESTY:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"你的诚信度不够，请找管理员解决"));
				break;
			case NO_ACCESS:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"借书数量已达上限"));
				break;
			case BOOK_NOT_LEFT:
				switch (QMessageBox::question(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"该书籍全部借出，是否预约?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
				{
				case QMessageBox::Yes:
					Reserve();
					break;
				default:
					break;
				}
				break;
			case SUCCESS:
				QMessageBox::information(this, QString::fromWCharArray(L"借阅成功"), QString::fromWCharArray(L"借阅成功"));
				b.Fresh();
				resultTable->setItem(row, 7, new QTableWidgetItem(QString::number(b.current_book[row]->left_num, 10)));
				break;
			case BOOK_ALREADY_BORROWED:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"你已经借阅该书"));
				break;
			default:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"未知错误，返回值为") + QString::number(result, 10));
				break;
			}
		}
	}
	else
	{
		if (resultTable->currentItem() == NULL)
			QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"请先选择要修改的书籍！"));
		else
		{
			row = resultTable->currentItem()->row();
			nameLine->setText(QString::fromStdString(b.current_book[row]->name));
			org_nameLine->setText(QString::fromStdString(b.current_book[row]->org_name));
			publisherLine->setText(QString::fromStdString(b.current_book[row]->publisher));
			writerLine->setText(QString::fromStdString(b.current_book[row]->writer));
			translatorLine->setText(QString::fromStdString(b.current_book[row]->translator));
			publish_timeLine->setText(QString::fromStdString(b.current_book[row]->publish_time));
			introTxt->setText(QString::fromStdString(b.current_book[row]->intro));
			key_wordLine->setText(QString::fromStdString(b.current_book[row]->key_word));
			ratingLine->setText(QString("%1").arg(b.current_book[row]->rating));
			page_numLine->setText(QString::number(b.current_book[row]->page_num, 10));
			//purchase_timeDate->setDate(QDate::fromString(QString::fromStdString(b.current_book[row]->purchase_time.date()),"yyyy/MM/dd"));
			purchase_timeLine->setText(QString::fromStdString(b.current_book[row]->purchase_time.date()));
			ISBNLine->setText(QString::number(b.current_book[row]->ISBN, 10));

			url = QString::fromStdString(b.current_book[row]->img_url);
			pict = new QPixmap(".\\img\\" + url);
			pic->setPixmap(*pict);

			change->show();
		}
	}
}
void Searchdlg::ChangeInfo()
{
	int row = resultTable->currentItem()->row();
	string name = nameLine->text().toStdString();
	string org_name = org_nameLine->text().toStdString();
	string publishe = publisherLine->text().toStdString();
	string writer = writerLine->text().toStdString();
	string translator = translatorLine->text().toStdString();
	string publish_time = publish_timeLine->text().toStdString();
	string intro = introTxt->toPlainText().toStdString();
	string key_word = key_wordLine->text().toStdString();
	string rating = ratingLine->text().toStdString();
	string page_num = page_numLine->text().toStdString();
	string ISBN = ISBNLine->text().toStdString();

	url = QString::fromStdString(ISBN) + ".jpg";
	pict->save(".\\img\\" + url);

	string line = name + "," + org_name + "," + publishe + "," + writer + "," + translator + "," + rating + "," + key_word + "," + ISBN + "," + page_num + "," + publish_time + "," + url.toStdString() + "," + intro;
	int result = library.BookEdit(*(b.current_book[row]), line);
	switch (result) {
	case NO_ACCESS:
		QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"需要管理员权限"));
		break;
	case FAIL:
		QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"FAIL"));
		break;
	case SUCCESS:
		change->close();
		ShowSearchResult();
		QMessageBox::information(this, QString::fromWCharArray(L"修改成功"), QString::fromWCharArray(L"修改成功"));
		break;
	case SYN_ERROR:
		QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"SYN_ERROR"));
		break;
	default:
		QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"未知错误"));
		break;
	}
}
void Searchdlg::ChangePic()
{
	QString file = QFileDialog::getOpenFileName(0, QString::fromWCharArray(L"打开图片"), ".", "Image Files(*.jpg)");
	if (file == NULL)    return;
	url = file;
	pict = new QPixmap(url);
	pic->setPixmap(*pict);
}
void Searchdlg::ShowCheckBox()
{
	int row = resultTable->currentItem()->row();
	for (int i = 0; i < keywords.size(); i++)
	{
		if (((string)b.current_book[row]->key_word).find(keywords[i]) != ((string)b.current_book[row]->key_word).npos)
			checkboxs[i]->setChecked(true);
		else
			checkboxs[i]->setChecked(false);
	}
	CheboxDlg->show();
}
void Searchdlg::ChangeKey()
{
	int first = 1;
	string key;
	for (int i = 0; i < checkboxs.size(); i++)
	{
		if (checkboxs[i]->checkState() == Qt::Checked)
		{
			if (first)
			{
				key = keywords[i];
				first = 0;
			}
			else
				key = key + ";" + keywords[i];
		}
	}
	key_wordLine->setText(QString::fromStdString(key));
	CheboxDlg->close();
}
void Searchdlg::Reserve()
{
	int row = resultTable->currentItem()->row();
	int result = library.Reserve(*b.current_book[row]);
	switch (result) {
	case NO_LOGIN:
		QMessageBox::critical(this, QString::fromWCharArray(L"预约失败"), QString::fromWCharArray(L"请先登录！"));
		break;
	case DISHONESTY:
		QMessageBox::critical(this, QString::fromWCharArray(L"预约失败"), QString::fromWCharArray(L"你的诚信度不够，请找管理员解决"));
		break;
	case BOOK_ALREADY_BORROWED:
		QMessageBox::critical(this, QString::fromWCharArray(L"预约失败"), QString::fromWCharArray(L"你已经借阅该书"));
		break;
	case BOOK_ALREADY_RESERVED:
		QMessageBox::critical(this, QString::fromWCharArray(L"预约失败"), QString::fromWCharArray(L"你已经预约该书"));
		break;
	case SUCCESS:
		QMessageBox::information(this, QString::fromWCharArray(L"预约成功"), QString::fromWCharArray(L"预约成功，请经常查看预约信息以便及时借阅"));
		b.Fresh();
		break;
	case SYN_ERROR:
		QMessageBox::critical(this, QString::fromWCharArray(L"预约失败"), QString::fromWCharArray(L"SYN_ERROR"));
		break;
	default:
		QMessageBox::critical(this, QString::fromWCharArray(L"预约失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
		break;
	}
}
void Searchdlg::ShowDetail()
{
	int row;
	if (resultTable->currentItem() == NULL)
		QMessageBox::critical(this, QString::fromWCharArray(L"查看失败"), QString::fromWCharArray(L"请先选择要查看的书籍！"));
	else
	{
		row = resultTable->currentItem()->row();
		QDialog *details = new QDialog;
		details->setWindowIcon(QIcon(".\\css\\book.gif"));
		details->setWindowTitle(QString::fromWCharArray(L"书籍详情"));
		QLabel *nameLabel = new QLabel;
		nameLabel->setText(QString::fromWCharArray(L"书名:") + QString::fromStdString(b.current_book[row]->name));
		QLabel *org_nameLabel = new QLabel;
		org_nameLabel->setText(QString::fromWCharArray(L"原名:") + QString::fromStdString(b.current_book[row]->org_name));
		QLabel *publisherLabel = new QLabel;
		publisherLabel->setText(QString::fromWCharArray(L"出版社:") + QString::fromStdString(b.current_book[row]->publisher));
		QLabel *writerLabel = new QLabel;
		writerLabel->setText(QString::fromWCharArray(L"作者:") + QString::fromStdString(b.current_book[row]->writer));
		QLabel *translatorLabel = new QLabel;
		translatorLabel->setText(QString::fromWCharArray(L"译者:") + QString::fromStdString(b.current_book[row]->translator));
		QLabel *publish_timeLabel = new QLabel;
		publish_timeLabel->setText(QString::fromWCharArray(L"出版时间:") + QString::fromStdString(b.current_book[row]->publish_time));
		QLabel *introLabel = new QLabel;
		introLabel->setText(QString::fromWCharArray(L"简介:"));
		QTextEdit *introTxt = new QTextEdit;
		introTxt->setText(QString::fromStdString(b.current_book[row]->intro));
		introTxt->setReadOnly(true);
		QLabel *key_wordLabel = new QLabel;
		key_wordLabel->setText(QString::fromWCharArray(L"标签:") + QString::fromStdString(b.current_book[row]->key_word));
		QLabel *ratingLabel = new QLabel;
		ratingLabel->setText(QString::fromWCharArray(L"豆瓣评分:") + QString("%1").arg(b.current_book[row]->rating));
		QLabel *page_numLabel = new QLabel;
		page_numLabel->setText(QString::fromWCharArray(L"总页数:") + QString::number(b.current_book[row]->page_num, 10));
		QLabel *left_numLabel = new QLabel;
		left_numLabel->setText(QString::fromWCharArray(L"剩余量:") + QString::number(b.current_book[row]->left_num, 10));
		QLabel *purchase_timeLabel = new QLabel;
		purchase_timeLabel->setText(QString::fromWCharArray(L"购入时间:") + QString::fromStdString(b.current_book[row]->purchase_time.date()));
		QLabel *total_numLabel = new QLabel;
		total_numLabel->setText(QString::fromWCharArray(L"总数量:") + QString::number(b.current_book[row]->total_num, 10));
		QLabel *ISBNLabel = new QLabel;
		ISBNLabel->setText(QString::fromWCharArray(L"ISBN:") + QString::number(b.current_book[row]->ISBN, 10));

		QLabel *pic = new QLabel;
		QString url = ".\\img\\" + QString::fromStdString(b.current_book[row]->img_url);
		QPixmap *pict = new QPixmap(url);
		pic->setPixmap(*pict);

		detailLayout = new QVBoxLayout;
		detailLayout->addWidget(nameLabel);
		detailLayout->addWidget(writerLabel);
		detailLayout->addWidget(org_nameLabel);
		detailLayout->addWidget(translatorLabel);
		detailLayout->addWidget(key_wordLabel);
		detailLayout->addWidget(publish_timeLabel);
		detailLayout->addWidget(ratingLabel);
		detailLayout->addWidget(page_numLabel);
		detailLayout->addWidget(purchase_timeLabel);
		detailLayout->addWidget(total_numLabel);
		detailLayout->addWidget(left_numLabel);
		detailLayout->addWidget(ISBNLabel);
		detailLayout->addWidget(introLabel);
		detailLayout->addWidget(introTxt);

		QHBoxLayout *totalLayout = new   QHBoxLayout(details);
		totalLayout->addLayout(detailLayout);
		totalLayout->addWidget(pic);

		details->show();
	}
}
Searchdlg::~Searchdlg()
{

}

Borroweddlg::Borroweddlg()
{
	this->setRowCount(1);
	this->setColumnCount(7);

	QStringList header;
	header << QString::fromWCharArray(L"书籍名称") << QString::fromWCharArray(L"出版社") << QString::fromWCharArray(L"作者") << QString::fromWCharArray(L"标签") << "ISBN" << QString::fromWCharArray(L"借阅时间") << QString::fromWCharArray(L"应还日期");
	this->setHorizontalHeaderLabels(header);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
}
void Borroweddlg::ShowBorrowed(User *u)
{
	int row = this->rowCount();
	for (int i = 0; i<row; i++)
	{
		this->removeRow(i);
	}

	borrowed_book = u->borrowed_book;
	this->setRowCount(borrowed_book.size());
	Time t;

	for (row = 0; row<borrowed_book.size(); row++)
	{
		this->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(borrowed_book[row].book->name)));
		this->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(borrowed_book[row].book->publisher)));
		this->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(borrowed_book[row].book->writer)));
		this->setItem(row, 3, new QTableWidgetItem(QString::fromStdString((string)(borrowed_book[row].book->key_word))));
		this->setItem(row, 4, new QTableWidgetItem(QString::number(borrowed_book[row].book->ISBN, 10)));
		this->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(borrowed_book[row].start_time.date())));
		t.time = borrowed_book[row].start_time.time + borrowed_book[row].keep_time;
		this->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(t.date())));
	}
}
Borroweddlg::~Borroweddlg()
{
}

ReservedTable::ReservedTable()
{
	this->setRowCount(1);
	this->setColumnCount(6);

	QStringList header;
	header << QString::fromWCharArray(L"书籍名称") << QString::fromWCharArray(L"出版社") << QString::fromWCharArray(L"作者") << QString::fromWCharArray(L"标签") << "ISBN" << QString::fromWCharArray(L"状态");
	this->setHorizontalHeaderLabels(header);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
}
void ReservedTable::ShowReserved(User *u)
{
	int row = this->rowCount(), i;
	for (int i = 0; i<row; i++)
	{
		this->removeRow(i);
	}

	reserved_book = u->reserved_book;
	this->setRowCount(reserved_book.size());

	for (row = 0; row < reserved_book.size(); row++)
	{
		this->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(Book(reserved_book[row]).name)));
		this->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(Book(reserved_book[row]).publisher)));
		this->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(Book(reserved_book[row]).writer)));
		this->setItem(row, 3, new QTableWidgetItem(QString::fromStdString((string)(Book(reserved_book[row]).key_word))));
		this->setItem(row, 4, new QTableWidgetItem(QString::number(Book(reserved_book[row]).ISBN, 10)));
		for (i = 0; i < u->v_reserved_book.size(); i++)
			if (u->v_reserved_book[i] == reserved_book[row])   break;
		if (i == u->v_reserved_book.size())
			this->setItem(row, 5, new QTableWidgetItem(QString::fromWCharArray(L"预约中")));
		else if (i < u->v_reserved_book.size())
			this->setItem(row, 5, new QTableWidgetItem(QString::fromWCharArray(L"可借阅")));
		else
			this->setItem(row, 5, new QTableWidgetItem(QString::fromWCharArray(L"未知错误")));
	}
}
ReservedTable::~ReservedTable()
{

}

Reserveddlg::Reserveddlg()
{
	setWindowTitle(QString::fromWCharArray(L"预约信息"));

	CreateResults();
	CreateButtons();

	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(reservedTable, 0, 0, 1, 2);
	mainLayout->addWidget(btnDlg, 1, 1);
}
void Reserveddlg::CreateResults()
{
	reservedTable = new ReservedTable;
}
void Reserveddlg::CreateButtons()
{
	btnDlg = new QDialog;

	borrowBtn = new QPushButton;
	cancelresBtn = new QPushButton;

	borrowBtn->setText(QString::fromWCharArray(L"借阅"));
	cancelresBtn->setText(QString::fromWCharArray(L"取消预约"));

	QHBoxLayout *btnBox = new QHBoxLayout(btnDlg);
	btnBox->addWidget(borrowBtn);
	btnBox->addWidget(cancelresBtn);

	connect(borrowBtn, SIGNAL(clicked()), this, SLOT(Borrow()));
	connect(cancelresBtn, SIGNAL(clicked()), this, SLOT(CancelReserve()));
}
void Reserveddlg::Borrow()
{
	int row;
	if (reservedTable->currentItem() == NULL)
		QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"请先选择要借阅的书籍！"));
	else
	{
		row = reservedTable->currentItem()->row();
		if (reservedTable->item(row, 5)->text() != QString::fromWCharArray(L"可借阅"))
			QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"尚在预约之中，无可借阅书籍"));
		else
		{
			int result = library.Borrow(library.currentUser->reserved_book[row], DEFAULT_KEEP_TIME);
			switch (result) {
			case NO_LOGIN:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"请先登录！"));
				break;
			case DISHONESTY:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"你的诚信度不够，请找管理员解决"));
				break;
			case NO_ACCESS:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"借书数量已达上限"));
				break;
			case BOOK_NOT_LEFT:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"该书籍全部借出，请重新预约"));
				break;
			case SUCCESS:
				b.Fresh();
				reservedTable->ShowReserved(library.currentUser);
				QMessageBox::information(this, QString::fromWCharArray(L"借阅成功"), QString::fromWCharArray(L"借阅成功"));
				break;
			case BOOK_ALREADY_BORROWED:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"你已经借阅该书"));
				break;
			default:
				QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
				break;
			}
		}
	}
}
void Reserveddlg::CancelReserve()
{
	int row;
	if (reservedTable->currentItem() == NULL)
		QMessageBox::critical(this, QString::fromWCharArray(L"取消失败"), QString::fromWCharArray(L"请先选择要取消的书籍！"));
	else
	{
		row = reservedTable->currentItem()->row();
		int result = library.ReserveCancel(library.currentUser->reserved_book[row]);
		switch (result) {
		case NO_LOGIN:
			QMessageBox::critical(this, QString::fromWCharArray(L"取消失败"), QString::fromWCharArray(L"请先登录！"));
			break;
		case SUCCESS:
			reservedTable->ShowReserved(library.currentUser);
			QMessageBox::information(this, QString::fromWCharArray(L"取消成功"), QString::fromWCharArray(L"取消成功"));
			b.Fresh();
			break;
		default:
			QMessageBox::critical(this, QString::fromWCharArray(L"取消失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
			break;
		}
	}

}
Reserveddlg::~Reserveddlg()
{

}

Returndlg::Returndlg()
{
	borrowedDlg = new Borroweddlg;
	returnBtn = new QPushButton;
	returnBtn->setText(QString::fromWCharArray(L"还书"));
	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(borrowedDlg, 0, 0);
	mainLayout->addWidget(returnBtn, 1, 3);

	connect(returnBtn, SIGNAL(clicked()), this, SLOT(Return()));
}
void Returndlg::Return()
{
	int row;
	if (borrowedDlg->currentItem() == NULL)
		QMessageBox::critical(this, QString::fromWCharArray(L"还书失败"), QString::fromWCharArray(L"请先选择要归还的书籍！"));
	else
	{
		row = borrowedDlg->currentItem()->row();
		int result = library.Return(*(borrowedDlg->borrowed_book[row].book));
		switch (result) {
		case NO_LOGIN:
			QMessageBox::critical(this, QString::fromWCharArray(L"还书失败"), QString::fromWCharArray(L"请先登录！"));
			break;
		case FAIL:
			QMessageBox::critical(this, QString::fromWCharArray(L"还书失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
			break;
		default:
			b.Fresh();
			borrowedDlg->ShowBorrowed(library.currentUser);
			QMessageBox::information(this, QString::fromWCharArray(L"还书成功"), QString::fromWCharArray(L"还书成功"));
			break;
		}
	}
}
Returndlg::~Returndlg()
{

}

Extenddlg::Extenddlg()
{
	borrowedDlg = new Borroweddlg;
	extendLabel1 = new QLabel;
	extendLabel1->setText(QString::fromWCharArray(L"续借"));
	extendLabel1->setStyleSheet("color:black");
	daysCombo = new QComboBox;
	daysCombo->addItem("5");
	daysCombo->addItem("10");
	daysCombo->addItem("15");
	daysCombo->addItem("20");
	daysCombo->addItem("25");
	daysCombo->addItem("30");
	extendLabel2 = new QLabel;
	extendLabel2->setText(QString::fromWCharArray(L"天"));
	extendLabel2->setStyleSheet("color:black");
	extendBtn = new QPushButton;
	extendBtn->setText(QString::fromWCharArray(L"续借"));
	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(borrowedDlg, 0, 0);
	mainLayout->addWidget(extendLabel1, 1, 1);
	mainLayout->addWidget(daysCombo, 1, 2);
	mainLayout->addWidget(extendLabel2, 1, 3);
	mainLayout->addWidget(extendBtn, 1, 4);
	connect(extendBtn, SIGNAL(clicked()), this, SLOT(Extend()));
}
void Extenddlg::Extend()
{
	int row, days;
	if (borrowedDlg->currentItem() == NULL)
		QMessageBox::critical(this, QString::fromWCharArray(L"续借失败"), QString::fromWCharArray(L"请先选择要续借的书籍！"));
	else
	{
		row = borrowedDlg->currentItem()->row();
		days = daysCombo->currentText().toInt();
		int result = library.Extend(*(borrowedDlg->borrowed_book[row].book), days);
		switch (result) {
		case SUCCESS:
			b.Fresh();
			borrowedDlg->ShowBorrowed(library.currentUser);
			QMessageBox::information(this, QString::fromWCharArray(L"续借成功"), QString::fromWCharArray(L"续借成功"));
			break;
		case NO_ACCESS:
			QMessageBox::critical(this, QString::fromWCharArray(L"续借失败"), QString::fromWCharArray(L"你的诚信度不过，无法续借，请联系管理员"));
			break;
		case FAIL:
			QMessageBox::critical(this, QString::fromWCharArray(L"续借失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
			break;
		default:
			break;
		}
	}

}
Extenddlg::~Extenddlg()
{

}


//---------------------------Administer--------------------------------------

Registerdlg::Registerdlg()
{
	setWindowTitle(QString::fromWCharArray(L"注册"));
	setWindowIcon(QIcon(".\\css\\register.png"));

	typeLabel = new QLabel;
	typeLabel->setText(QString::fromWCharArray(L"用户类型："));
	nameLabel = new QLabel;
	nameLabel->setText(QString::fromWCharArray(L"姓名："));
	collegeLabel = new QLabel;
	collegeLabel->setText(QString::fromWCharArray(L"学院："));
	idLabel = new QLabel;
	idLabel->setText(QString::fromWCharArray(L"学号："));
	passwordLabel = new QLabel;
	passwordLabel->setText(QString::fromWCharArray(L"密码："));
	repasswordLabel = new QLabel;
	repasswordLabel->setText(QString::fromWCharArray(L"确认密码："));
	okBtn = new QPushButton;
	okBtn->setText(QString::fromWCharArray(L"确定"));
	cancelBtn = new QPushButton;
	cancelBtn->setText(QString::fromWCharArray(L"取消"));
	byfileBtn = new QPushButton;
	byfileBtn->setText(QString::fromWCharArray(L"从文件导入"));

	typeCombo = new QComboBox;
	typeCombo->insertItem(0, QString::fromWCharArray(L"学生"));
	typeCombo->insertItem(1, QString::fromWCharArray(L"教师"));
	typeCombo->insertItem(2, QString::fromWCharArray(L"管理员"));
	nameLine = new QLineEdit;
	collegeCombo = new QComboBox;
	collegeCombo->insertItem(0, QString::fromWCharArray(L"信息与通信工程学院"));
	collegeCombo->insertItem(1, QString::fromWCharArray(L"计算机学院"));
	collegeCombo->insertItem(2, QString::fromWCharArray(L"电子工程学院"));
	collegeCombo->insertItem(3, QString::fromWCharArray(L"数字媒体与设计艺术学院"));
	collegeCombo->insertItem(4, QString::fromWCharArray(L"理学院"));
	collegeCombo->insertItem(5, QString::fromWCharArray(L"经济管理学院"));
	collegeCombo->insertItem(6, QString::fromWCharArray(L"人文学院"));
	idLine = new QLineEdit;
	passwordLine = new QLineEdit;
	repasswordLine = new QLineEdit;

	buttonBox = new QHBoxLayout;
	buttonBox->addWidget(okBtn);
	buttonBox->addWidget(cancelBtn);
	buttonBox->addWidget(byfileBtn);
	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(typeLabel, 0, 0);
	mainLayout->addWidget(typeCombo, 0, 1);
	mainLayout->addWidget(nameLabel, 1, 0);
	mainLayout->addWidget(nameLine, 1, 1);
	mainLayout->addWidget(collegeLabel, 2, 0);
	mainLayout->addWidget(collegeCombo, 2, 1);
	mainLayout->addWidget(idLabel, 3, 0);
	mainLayout->addWidget(idLine, 3, 1);
	mainLayout->addWidget(passwordLabel, 4, 0);
	mainLayout->addWidget(passwordLine, 4, 1);
	mainLayout->addWidget(repasswordLabel, 5, 0);
	mainLayout->addWidget(repasswordLine, 5, 1);
	mainLayout->addLayout(buttonBox, 6, 0, 1, 2);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(Register()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
	connect(byfileBtn, SIGNAL(clicked()), this, SLOT(RegisterByFile()));
}
void Registerdlg::Register()
{
	string type = this->typeCombo->currentText().toStdString();
	string name = this->nameLine->text().toStdString();
	string college = this->collegeCombo->currentText().toStdString();
	string id = this->idLine->text().toStdString();
	string password1 = this->passwordLine->text().toStdString();
	string password2 = this->repasswordLine->text().toStdString();
	int result;
	string line;
	if (!(password1 == password2))   QMessageBox::critical(this, QString::fromWCharArray(L"注册失败"), QString::fromWCharArray(L"两次输入的密码不一致"));
	else
	{
		if (type == QString::fromWCharArray(L"管理员").toStdString())
		{
			line = id + "," + password1;
			result = library.AdminRegister(line);
		}
		else
		{
			string typeId = ((type == QString::fromWCharArray(L"学生").toStdString()) ? "0" : "1");
			line = id + "," + password1 + "," + typeId + "," + name + "," + college;
			result = library.UserRegister(line);
		}
		switch (result) {
		case ACCOUNT_EXIST:
			QMessageBox::critical(this, QString::fromWCharArray(L"注册失败"), QString::fromWCharArray(L"该用户已存在"));
			break;
		case SUCCESS:
			close();
			QMessageBox::information(this, QString::fromWCharArray(L"注册成功"), QString::fromWCharArray(L"注册成功"));
			break;
		default:
			QMessageBox::critical(this, QString::fromWCharArray(L"注册失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
			break;
		}
	}
}
void Registerdlg::RegisterByFile()
{
	QString file = QFileDialog::getOpenFileName();
	if (file == NULL)    return;
	int result = library.UserRegister(file.toStdString());
	switch (result) {
	case ACCOUNT_EXIST:
		QMessageBox::critical(this, QString::fromWCharArray(L"注册失败"), QString::fromWCharArray(L"该用户已存在"));
		break;
	case SUCCESS:
		this->close();
		QMessageBox::information(this, QString::fromWCharArray(L"注册成功"), QString::fromWCharArray(L"注册成功"));
		break;
	default:
		QMessageBox::critical(this, QString::fromWCharArray(L"注册失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
		break;
	}
}
Registerdlg::~Registerdlg()
{

}

UserInfoTabel::UserInfoTabel()
{
	this->setRowCount(2);
	this->setColumnCount(8);

	QStringList header;
	header << QString::fromWCharArray(L"姓名") << QString::fromWCharArray(L"学号") << QString::fromWCharArray(L"学院") << QString::fromWCharArray(L"类型") << QString::fromWCharArray(L"诚信度") << QString::fromWCharArray(L"借阅数量") << QString::fromWCharArray(L"预约数量") << QString::fromWCharArray(L"密码");
	this->setHorizontalHeaderLabels(header);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
}
void UserInfoTabel::ShowUsers()
{
	int row = this->rowCount();
	for (int i = 0; i<row; i++)
	{
		this->removeRow(i);
	}

	users = library.CurrentState();
	this->setRowCount(users.size());

	for (row = 0; row<users.size(); row++)
	{
		this->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(users[row].real_name)));
		this->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(users[row].user_name)));
		this->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(users[row].school)));
		this->setItem(row, 3, new QTableWidgetItem(users[row].type == STUDENT ? QString::fromWCharArray(L"学生") : QString::fromWCharArray(L"教师")));
		this->setItem(row, 4, new QTableWidgetItem(QString::number(users[row].honesty, 10)));
		this->setItem(row, 5, new QTableWidgetItem(QString::number(users[row].borrowed_book.size(), 10)));
		this->setItem(row, 6, new QTableWidgetItem(QString::number(users[row].reserved_book.size(), 10)));
		this->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(users[row].password)));
	}
}
UserInfoTabel::~UserInfoTabel()
{

}

UserInfodlg::UserInfodlg()
{
	infoTabel = new UserInfoTabel;
	infoTabel->ShowUsers();

	btnDlg = new QDialog;
	borrowedBtn = new QPushButton;
	borrowedBtn->setText(QString::fromWCharArray(L"借阅详情"));
	reservedBtn = new QPushButton;
	reservedBtn->setText(QString::fromWCharArray(L"预约详情"));
	changeBtn = new QPushButton;
	changeBtn->setText(QString::fromWCharArray(L"更改信息"));

	QHBoxLayout *buttonBox = new QHBoxLayout(btnDlg);
	buttonBox->addWidget(borrowedBtn);
	buttonBox->addWidget(reservedBtn);
	buttonBox->addWidget(changeBtn);

	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(infoTabel, 0, 0, 1, 2);
	mainLayout->addWidget(btnDlg, 1, 1);

	connect(borrowedBtn, SIGNAL(clicked()), this, SLOT(ShowBorrowed()));
	connect(reservedBtn, SIGNAL(clicked()), this, SLOT(ShowReserved()));
	connect(changeBtn, SIGNAL(clicked()), this, SLOT(ShowChanges()));
}
void UserInfodlg::ShowBorrowed()
{
	int row;
	if (infoTabel->currentItem() == NULL)
		QMessageBox::critical(this, QString::fromWCharArray(L"查看失败"), QString::fromWCharArray(L"请先选择要查看的用户！"));
	else
	{
		row = infoTabel->currentItem()->row();
		Borroweddlg *borrowed = new Borroweddlg;
		borrowed->ShowBorrowed(&(infoTabel->users[row]));
		borrowed->setWindowTitle(QString::fromStdString(infoTabel->users[row].real_name) + QString::fromWCharArray(L"借阅详情"));
		borrowed->show();
	}
}
void UserInfodlg::ShowReserved()
{
	int row;
	if (infoTabel->currentItem() == NULL)
		QMessageBox::critical(this, QString::fromWCharArray(L"查看失败"), QString::fromWCharArray(L"请先选择要查看的用户！"));
	else
	{
		row = infoTabel->currentItem()->row();
		ReservedTable *reserved = new ReservedTable;
		reserved->ShowReserved(&(infoTabel->users[row]));
		reserved->setWindowTitle(QString::fromStdString(infoTabel->users[row].real_name) + QString::fromWCharArray(L"预约详情"));
		reserved->show();
	}
}
void UserInfodlg::ShowChanges()
{
	if (infoTabel->currentItem() == NULL)
		QMessageBox::critical(this, QString::fromWCharArray(L"借阅失败"), QString::fromWCharArray(L"请先选择要借阅的书籍！"));
	else
	{
		int row = infoTabel->currentItem()->row();
		user = infoTabel->users[row];

		changeDlg = new QDialog;
		changeDlg->setWindowTitle(QString::fromWCharArray(L"修改用户信息"));
		changeDlg->setWindowIcon(QIcon(".\\css\\register.png"));

		typeLabel = new QLabel;
		typeLabel->setText(QString::fromWCharArray(L"用户类型："));
		nameLabel = new QLabel;
		nameLabel->setText(QString::fromWCharArray(L"姓名："));
		collegeLabel = new QLabel;
		collegeLabel->setText(QString::fromWCharArray(L"学院："));
		idLabel = new QLabel;
		idLabel->setText(QString::fromWCharArray(L"学号："));
		passwordLabel = new QLabel;
		passwordLabel->setText(QString::fromWCharArray(L"密码："));
		repasswordLabel = new QLabel;
		repasswordLabel->setText(QString::fromWCharArray(L"确认密码："));
		honestyLabel = new QLabel;
		honestyLabel->setText(QString::fromWCharArray(L"诚信度："));
		okBtn = new QPushButton;
		okBtn->setText(QString::fromWCharArray(L"确定"));
		cancelBtn = new QPushButton;
		cancelBtn->setText(QString::fromWCharArray(L"取消"));

		typeCombo = new QComboBox;
		typeCombo->insertItem(0, QString::fromWCharArray(L"学生"));
		typeCombo->insertItem(1, QString::fromWCharArray(L"教师"));
		if (user.type == STUDENT)    typeCombo->setCurrentIndex(0);
		else if (user.type == TEACHER)    typeCombo->setCurrentIndex(1);
		nameLine = new QLineEdit;
		nameLine->setText(QString::fromStdString(user.real_name));
		collegeCombo = new QComboBox;
		collegeCombo->insertItem(0, QString::fromWCharArray(L"信息与通信工程学院"));
		collegeCombo->insertItem(1, QString::fromWCharArray(L"计算机学院"));
		collegeCombo->insertItem(2, QString::fromWCharArray(L"电子工程学院"));
		collegeCombo->insertItem(3, QString::fromWCharArray(L"数字媒体与设计艺术学院"));
		collegeCombo->insertItem(4, QString::fromWCharArray(L"理学院"));
		collegeCombo->insertItem(5, QString::fromWCharArray(L"经济管理学院"));
		collegeCombo->insertItem(6, QString::fromWCharArray(L"人文学院"));
		collegeCombo->setCurrentText(QString::fromStdString(user.school));
		idLine = new QLineEdit;
		idLine->setText(QString::fromStdString(user.user_name));
		passwordLine = new QLineEdit;
		passwordLine->setText(QString::fromStdString(user.password));
		repasswordLine = new QLineEdit;
		repasswordLine->setText(QString::fromStdString(user.password));
		honestyLine = new QLineEdit;
		honestyLine->setText(QString::number(user.honesty, 10));

		buttonBox = new QHBoxLayout;
		buttonBox->addWidget(okBtn);
		buttonBox->addWidget(cancelBtn);
		changeLayout = new QGridLayout(changeDlg);
		changeLayout->addWidget(typeLabel, 0, 0);
		changeLayout->addWidget(typeCombo, 0, 1);
		changeLayout->addWidget(nameLabel, 1, 0);
		changeLayout->addWidget(nameLine, 1, 1);
		changeLayout->addWidget(collegeLabel, 2, 0);
		changeLayout->addWidget(collegeCombo, 2, 1);
		changeLayout->addWidget(idLabel, 3, 0);
		changeLayout->addWidget(idLine, 3, 1);
		changeLayout->addWidget(passwordLabel, 4, 0);
		changeLayout->addWidget(passwordLine, 4, 1);
		changeLayout->addWidget(repasswordLabel, 5, 0);
		changeLayout->addWidget(repasswordLine, 5, 1);
		changeLayout->addWidget(honestyLabel, 6, 0);
		changeLayout->addWidget(honestyLine, 6, 1);
		changeLayout->addLayout(buttonBox, 7, 0, 1, 2);

		changeDlg->show();

		connect(okBtn, SIGNAL(clicked()), this, SLOT(Change()));
		connect(cancelBtn, SIGNAL(clicked()), changeDlg, SLOT(close()));
	}
}
void UserInfodlg::Change()
{
	string type = this->typeCombo->currentText().toStdString();
	string name = this->nameLine->text().toStdString();
	string college = this->collegeCombo->currentText().toStdString();
	string id = this->idLine->text().toStdString();
	string honesty = this->honestyLine->text().toStdString();
	string password1 = this->passwordLine->text().toStdString();
	string password2 = this->repasswordLine->text().toStdString();
	int result;
	string line;

	if (!(password1 == password2))   QMessageBox::critical(this, QString::fromWCharArray(L"修改失败"), QString::fromWCharArray(L"两次输入的密码不一致"));
	else
	{
		string typeId = ((type == QString::fromWCharArray(L"学生").toStdString()) ? "0" : "1");
		line = id + "," + password1 + "," + honesty + "," + typeId + "," + name + "," + college;
		result = library.UserEdit(user, line);
		switch (result) {
		case SUCCESS:
			changeDlg->close();
			infoTabel->ShowUsers();
			QMessageBox::information(this, QString::fromWCharArray(L"修改成功"), QString::fromWCharArray(L"修改成功"));
			break;
		default:
			QMessageBox::critical(this, QString::fromWCharArray(L"注册失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
			break;
		}
	}
}
UserInfodlg::~UserInfodlg()
{

}

Booksdlg::Booksdlg()
{
	CreateTable();
	//ShowResults();

	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(booksTabel, 0, 0);
}
void Booksdlg::CreateTable()
{
	booksTabel = new QTableWidget;
	booksTabel->setRowCount(1);
	booksTabel->setColumnCount(6);

	QStringList header;
	header << QString::fromWCharArray(L"书名") << "ISBN" << QString::fromWCharArray(L"借出日期") << QString::fromWCharArray(L"应还日期") << QString::fromWCharArray(L"借者姓名") << QString::fromWCharArray(L"借者学号");
	booksTabel->setHorizontalHeaderLabels(header);

	booksTabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
	booksTabel->setSelectionBehavior(QAbstractItemView::SelectRows);
}
void Booksdlg::ShowResults()
{
	int row;
	row = booksTabel->rowCount();
	for (int i = 0; i<row; i++)
	{
		booksTabel->removeRow(i);
	}

	int i, j;
	Time t;
	users = library.CurrentState();
	for (i = 0, row = 0; i<users.size(); i++)
	{
		for (j = 0; j<users[i].borrowed_book.size(); j++, row++)
		{
			booksTabel->setRowCount(row + 1);
			booksTabel->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(users[i].borrowed_book[j].book->name)));
			booksTabel->setItem(row, 1, new QTableWidgetItem(QString::number(users[i].borrowed_book[j].book->ISBN, 10)));
			booksTabel->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(users[i].borrowed_book[j].start_time.date())));
			t.time += users[i].borrowed_book[j].keep_time;
			booksTabel->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(t.date())));
			booksTabel->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(users[i].real_name)));
			booksTabel->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(users[i].user_name)));
		}
	}
}
Booksdlg::~Booksdlg()
{

}

Purchasedlg::Purchasedlg()
{
	setWindowTitle(QString::fromWCharArray(L"购买"));
	CreateCheckbox();

	CHnameLabel = new QLabel;
	CHnameLabel->setText(QString::fromWCharArray(L"书籍名称"));
	CHnameLabel->setStyleSheet("color:black");
	NEnameLabel = new QLabel;
	NEnameLabel->setText(QString::fromWCharArray(L"书籍原名"));
	NEnameLabel->setStyleSheet("color:black");
	publisherLabel = new QLabel;
	publisherLabel->setText(QString::fromWCharArray(L"出版社"));
	publisherLabel->setStyleSheet("color:black");
	writerLabel = new QLabel;
	writerLabel->setText(QString::fromWCharArray(L"作者"));
	writerLabel->setStyleSheet("color:black");
	translatorLabel = new QLabel;
	translatorLabel->setText(QString::fromWCharArray(L"译者"));
	translatorLabel->setStyleSheet("color:black");
	scoreLabel = new QLabel;
	scoreLabel->setText(QString::fromWCharArray(L"豆瓣评分"));
	scoreLabel->setStyleSheet("color:black");
	classLabel = new QLabel;
	classLabel->setText(QString::fromWCharArray(L"标签"));
	classLabel->setStyleSheet("color:black");
	ISBNLabel = new QLabel;
	ISBNLabel->setText(QString::fromWCharArray(L"ISBN"));
	ISBNLabel->setStyleSheet("color:black");
	pagenumLabel = new QLabel;
	pagenumLabel->setText(QString::fromWCharArray(L"页数"));
	pagenumLabel->setStyleSheet("color:black");
	dateLabel = new QLabel;
	dateLabel->setText(QString::fromWCharArray(L"出版日期"));
	dateLabel->setStyleSheet("color:black");
	introLabel = new QLabel;
	introLabel->setText(QString::fromWCharArray(L"简介"));
	introLabel->setStyleSheet("color:black");
	numLabel = new QLabel;
	numLabel->setText(QString::fromWCharArray(L"购入数量(1-25)"));
	numLabel->setStyleSheet("color:black");

	CHnameLine = new QLineEdit;
	NEnameLine = new QLineEdit;
	publisherLine = new QLineEdit;
	writerLine = new QLineEdit;
	translatorLine = new QLineEdit;
	scoreLine = new QLineEdit;
	classLine = new QLineEdit;
	classLine->setEnabled(false);
	changclassBtn = new QPushButton;
	changclassBtn->setText(QString::fromWCharArray(L"修改"));
	ISBNLine = new QLineEdit;
	pagenumLine = new QLineEdit;
	dateEdit = new QDateEdit(QDate::currentDate(), this);
	dateEdit->setCalendarPopup(true);
	dateEdit->setDate(QDate::fromString(QString::fromStdString(currentTime->date()), "yyyy/MM/dd"));
	introTxt = new QTextEdit;
	numLine = new QLineEdit;
	numLine->setText("1");
	numLine->setValidator(new QIntValidator(1, 25, numLine));
	numSlider = new QSlider(Qt::Horizontal);
	numSlider->setMinimum(1);
	numSlider->setMaximum(25);
	numSlider->setValue(1);
	numSlider->setSingleStep(1);
	numSlider->setTickInterval(1);
	numSlider->setTickPosition(QSlider::TicksBothSides);

	picLabel = new QLabel;
	url = "";
	addpicBtn = new QPushButton;
	addpicBtn->setText(QString::fromWCharArray(L"上传图片"));

	okBtn = new QPushButton;
	okBtn->setText(QString::fromWCharArray(L"确定"));
	cancelBtn = new QPushButton;
	cancelBtn->setText(QString::fromWCharArray(L"取消"));
	byfileBtn = new QPushButton;
	byfileBtn->setText(QString::fromWCharArray(L"通过文件导入"));

	buttonBox = new QHBoxLayout;
	buttonBox->addWidget(okBtn);
	buttonBox->addWidget(cancelBtn);
	buttonBox->addWidget(addpicBtn);
	buttonBox->addWidget(byfileBtn);
	QHBoxLayout *space1 = new QHBoxLayout;
	space1->addSpacing(WIDTH / 5);
	QHBoxLayout *space2 = new QHBoxLayout;
	space2->addSpacing(WIDTH / 5);
	mainLayout = new QGridLayout(this);
	mainLayout->addLayout(space1, 0, 0, 13, 1);
	mainLayout->addWidget(CHnameLabel, 0, 1);
	mainLayout->addWidget(CHnameLine, 0, 2, 1, 2);
	mainLayout->addWidget(picLabel, 0, 4, 11, 1);
	mainLayout->addWidget(NEnameLabel, 1, 1);
	mainLayout->addWidget(NEnameLine, 1, 2, 1, 2);
	mainLayout->addWidget(publisherLabel, 2, 1);
	mainLayout->addWidget(publisherLine, 2, 2, 1, 2);
	mainLayout->addWidget(writerLabel, 3, 1);
	mainLayout->addWidget(writerLine, 3, 2, 1, 2);
	mainLayout->addWidget(translatorLabel, 4, 1);
	mainLayout->addWidget(translatorLine, 4, 2, 1, 2);
	mainLayout->addWidget(scoreLabel, 5, 1);
	mainLayout->addWidget(scoreLine, 5, 2, 1, 2);
	mainLayout->addWidget(classLabel, 6, 1);
	mainLayout->addWidget(classLine, 6, 2);
	mainLayout->addWidget(changclassBtn, 6, 3);
	mainLayout->addWidget(ISBNLabel, 7, 1);
	mainLayout->addWidget(ISBNLine, 7, 2, 1, 2);
	mainLayout->addWidget(pagenumLabel, 8, 1);
	mainLayout->addWidget(pagenumLine, 8, 2, 1, 2);
	mainLayout->addWidget(dateLabel, 9, 1);
	mainLayout->addWidget(dateEdit, 9, 2, 1, 2);
	mainLayout->addWidget(introLabel, 10, 1);
	mainLayout->addWidget(introTxt, 10, 2, 1, 2);
	mainLayout->addWidget(numLabel, 11, 1);
	mainLayout->addWidget(numLine, 11, 2, 1, 2);
	mainLayout->addWidget(numSlider, 12, 2, 1, 2);
	mainLayout->addLayout(buttonBox, 13, 3);
	mainLayout->addLayout(space2, 0, 3, 14, 1);

	connect(numSlider, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue()));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(Purchase()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
	connect(addpicBtn, SIGNAL(clicked()), this, SLOT(AddPicture()));
	connect(byfileBtn, SIGNAL(clicked()), this, SLOT(PurchaseByFile()));
	connect(changclassBtn, SIGNAL(clicked()), CheboxDlg, SLOT(show()));
}
void Purchasedlg::CreateCheckbox()
{
	int i;
	CheboxDlg = new QDialog;
	check_mainLayout = new QGridLayout(CheboxDlg);
	for (i = 0; i < keywords.size(); i++)
	{
		QCheckBox *temp = new QCheckBox(CheboxDlg);
		temp->setText(QString::fromStdString(keywords[i]));
		checkboxs.push_back(temp);
		check_mainLayout->addWidget(checkboxs[i], i / 10, i % 10);
	}
	check_okBtn = new QPushButton;
	check_okBtn->setText(QString::fromWCharArray(L"确定"));
	check_cancelBtn = new QPushButton;
	check_cancelBtn->setText(QString::fromWCharArray(L"取消"));
	check_mainLayout->addWidget(check_okBtn, ++i / 10 + 1, 8);
	check_mainLayout->addWidget(check_cancelBtn, i / 10 + 1, 9);

	connect(check_okBtn, SIGNAL(clicked()), this, SLOT(ChangeKey()));
	connect(check_cancelBtn, SIGNAL(clicked()), CheboxDlg, SLOT(close()));

	CheboxDlg->hide();
}
void Purchasedlg::ChangeKey()
{
	int first = 1;
	string key;
	for (int i = 0; i < checkboxs.size(); i++)
	{
		if (checkboxs[i]->checkState() == Qt::Checked)
		{
			if (first)
			{
				key = keywords[i];
				first = 0;
			}
			else
				key = key + ";" + keywords[i];
		}
	}
	classLine->setText(QString::fromStdString(key));
	CheboxDlg->close();
}
void Purchasedlg::AddPicture()
{
	url = QFileDialog::getOpenFileName(0, QString::fromWCharArray(L"打开图片"), ".", "Image Files(*.jpg)");
	if (url == NULL)    return;
	pic = new QPixmap(url);
	picLabel->setPixmap(*pic);
}
void Purchasedlg::setLineEditValue()
{
	int num = numSlider->value();
	QString str = QString("%1").arg(num);
	numLine->setText(str);
}
void Purchasedlg::Purchase()
{
	string CHname = "";
	if (!CHnameLine->text().isEmpty())    CHname = this->CHnameLine->text().toStdString();
	string NEname = "";
	if (!NEnameLine->text().isEmpty())    NEname = this->NEnameLine->text().toStdString();
	string publisher = "";
	if (!publisherLine->text().isEmpty())    publisher = this->publisherLine->text().toStdString();
	string writer = "";
	if (!writerLine->text().isEmpty())    writer = this->writerLine->text().toStdString();
	string translator = "";
	if (!translatorLine->text().isEmpty())    translator = this->translatorLine->text().toStdString();
	string score = this->scoreLine->text().toStdString();
	string book_class = "";
	if (!classLine->text().isEmpty())    book_class = this->classLine->text().toStdString();
	string ISBN = "";
	if (!ISBNLine->text().isEmpty())    ISBN = this->ISBNLine->text().toStdString();
	string pages = "";
	if (!pagenumLine->text().isEmpty())    pages = this->pagenumLine->text().toStdString();
	string num = this->numLine->text().toStdString();
	string pubdate = this->dateEdit->text().toStdString();
	string intro = "";
	if (!introTxt->toPlainText().isEmpty())    intro = this->introTxt->toPlainText().toStdString();
	if (pic != NULL)
	{
		url = QString::fromStdString(ISBN) + ".jpg";
		pic->save(".\\img\\" + url);
	}
	string line = CHname + "," + NEname + "," + publisher + "," + writer + "," + translator + "," + score + "," + book_class + "," + ISBN + "," + pages + "," + num + "," + pubdate + "," + url.toStdString() + "," + intro;
	int result = library.Purchase(line);
	switch (result) {
	case NO_ACCESS:
		QMessageBox::critical(this, QString::fromWCharArray(L"购入失败"), QString::fromWCharArray(L"非管理员不能进行购入操作"));
		break;
	case PURCHASE_NEW:
	case PURCHASE_OLD:
		QMessageBox::information(this, QString::fromWCharArray(L"购入成功"), QString::fromWCharArray(L"购入成功"));
		break;
	default:
		QMessageBox::critical(this, QString::fromWCharArray(L"购入失败"), QString::fromWCharArray(L"未知失败"));
		break;
	}
}
void Purchasedlg::PurchaseByFile()
{
	QString file = QFileDialog::getOpenFileName();
	QMessageBox::information(this, QString::fromWCharArray(L"购入中"), QString::fromWCharArray(L"点击确定开始购入，请耐心等待，不要进行其他操作"));
	int result = library.Purchase(file.toStdString());
	switch (result) {
	case NO_ACCESS:
		QMessageBox::critical(this, QString::fromWCharArray(L"购入失败"), QString::fromWCharArray(L"非管理员不能进行购入操作"));
		break;
	case PURCHASE_NEW:
	case PURCHASE_OLD:
		QMessageBox::information(this, QString::fromWCharArray(L"购入成功"), QString::fromWCharArray(L"购入成功"));
		break;
	default:
		QMessageBox::critical(this, QString::fromWCharArray(L"购入失败"), QString::fromWCharArray(L"未知失败"));
		break;
	}
}
Purchasedlg::~Purchasedlg()
{

}

Titledlg::Titledlg()
{
	welcomeLabel = new QLabel;
	welcomepic = new QPixmap("title.png");
	welcomeLabel->setPixmap(*welcomepic);

	mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(welcomeLabel, 0, Qt::AlignCenter);
}
Titledlg::~Titledlg()
{

}

RankDlg::RankDlg()
{
	titleLabel = new QLabel;
	titleLabel->setPixmap(*(new QPixmap("RankTitle.png")));
	mainLayout = new QVBoxLayout;
	mainLayout->addSpacing(HEIGHT / 20);
	mainLayout->addWidget(titleLabel);
	mainLayout->addSpacing(HEIGHT / 30);
	int i;
	QString url;
	for (i = 0; i < BORROW_RANK; i++)
	{
		ranks[i] = new QLabel;
		books[i] = new QLabel;
		books[i]->setStyleSheet("font-size:35px;color:black;text-align:start");
		url = "rank" + QString::number(i + 1, 10) + ".png";
		ranks[i]->setPixmap(*(new QPixmap(url)));
		rankLayout[i] = new QHBoxLayout;
		rankLayout[i]->addWidget(ranks[i]);
		rankLayout[i]->addWidget(books[i]);
		mainLayout->addLayout(rankLayout[i]);
	}
	mainLayout->addSpacing(HEIGHT / 6);
	totalLayout = new QHBoxLayout(this);
	totalLayout->addSpacing(WIDTH * 10 / 11 / 3);
	totalLayout->addLayout(mainLayout, Qt::AlignCenter);
	totalLayout->addSpacing(WIDTH * 10 / 11 / 3);
}
void RankDlg::ShowRank()
{
	int i;
	vector<Book> Rank_Books = library.BorrowRank();
	for (i = 0; i < Rank_Books.size(); i++)
	{
		books[i]->setText(QString::fromStdString(Rank_Books[i].name));
	}
	for (; i < BORROW_RANK; i++)
	{
		books[i]->setText(QString::fromWCharArray(L"无"));
	}
}
RankDlg::~RankDlg()
{

}

MainWindow::MainWindow()
{
	//library.UserLogin("2015212160","zq");
	//library.AdminLogin("awdrtg","111");
	// library.Purchase(".\\backup\\doubanUTF8.txt");

	resize(WIDTH, HEIGHT - HEIGHT / 10);
	setWindowIcon(QIcon(".\\css\\book.gif"));
	setWindowTitle(QString::fromWCharArray(L"我的图书馆"));

	CreateUserMenuDlg();
	CreateAdminMenuDlg();
	CreateCurrentDlg();

	infodlg = new Infodlg;
	CreateLoginDlg();
	purchasedlg = new Purchasedlg;
	returndlg = new Returndlg;
	searchdlg = new Searchdlg;
	extenddlg = new Extenddlg;
	reserveddlg = new Reserveddlg;
	userinfodlg = new UserInfodlg;
	booksdlg = new Booksdlg;
	titledlg = new Titledlg;
	currentDlg = rankdlg = new RankDlg;
	rankdlg->ShowRank();

	titledlg->setAttribute(Qt::WA_TranslucentBackground, true);
	UsermenuDlg->setAttribute(Qt::WA_TranslucentBackground, true);
	AdminmenuDlg->setAttribute(Qt::WA_TranslucentBackground, true);
	currentDlg->setAttribute(Qt::WA_TranslucentBackground, 0.5);
	searchdlg->setAttribute(Qt::WA_TranslucentBackground, true);
	returndlg->setAttribute(Qt::WA_TranslucentBackground, true);
	extenddlg->setAttribute(Qt::WA_TranslucentBackground, true);
	infodlg->setAttribute(Qt::WA_TranslucentBackground, true);
	purchasedlg->setAttribute(Qt::WA_TranslucentBackground, true);

	menuDlg = UsermenuDlg;
	titledlg->resize(WIDTH, HEIGHT / 4);
	topLayout = new QHBoxLayout;
	topLayout->addWidget(titledlg);

	bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(UsermenuDlg);
	bottomLayout->addWidget(currentDlg);
	bottomLayout->setStretchFactor(UsermenuDlg, 1);
	bottomLayout->setStretchFactor(currentDlg, 10);

	mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	mainLayout->setStretchFactor(topLayout, 1);
	mainLayout->setStretchFactor(bottomLayout, 4);

	setLayout(mainLayout);
	timer = new QTimer;
	timer->start(MS_PER_DAY);
	connect(timer, SIGNAL(timeout()), this, SLOT(ChangeDate()));
}
void MainWindow::CreateLoginDlg()
{
	logindlg = new QDialog;
	logindlg->setWindowTitle(QString::fromWCharArray(L"登录"));
	logindlg->setWindowIcon(QIcon(".\\css\\login.gif"));

	typeLabel = new QLabel;
	typeLabel->setText(QString::fromWCharArray(L"用户类型："));
	nameLabel = new QLabel;
	nameLabel->setText(QString::fromWCharArray(L"学号："));
	passwordLabel = new QLabel;
	passwordLabel->setText(QString::fromWCharArray(L"密码："));

	typeCombo = new QComboBox;
	typeCombo->insertItem(0, QString::fromWCharArray(L"用户"));
	typeCombo->insertItem(1, QString::fromWCharArray(L"管理员"));
	nameLine = new QLineEdit;
	passwordLine = new QLineEdit;
	passwordLine->setEchoMode(QLineEdit::Password);

	loginBtn = new QPushButton;
	loginBtn->setText(QString::fromWCharArray(L"登录"));
	cancelBtn = new QPushButton;
	cancelBtn->setText(QString::fromWCharArray(L"取消"));
	buttonBox = new QHBoxLayout;
	buttonBox->addWidget(loginBtn);
	buttonBox->addWidget(cancelBtn);

	loginLayout = new QGridLayout(logindlg);
	loginLayout->addWidget(typeLabel, 0, 0);
	loginLayout->addWidget(typeCombo, 0, 1);
	loginLayout->addWidget(nameLabel, 1, 0);
	loginLayout->addWidget(nameLine, 1, 1);
	loginLayout->addWidget(passwordLabel, 2, 0);
	loginLayout->addWidget(passwordLine, 2, 1);
	loginLayout->addLayout(buttonBox, 3, 0, 1, 2);

	connect(loginBtn, SIGNAL(clicked()), this, SLOT(Login()));
	connect(cancelBtn, SIGNAL(clicked()), logindlg, SLOT(close()));

}
void MainWindow::CreateUserMenuDlg()
{
	UsermenuDlg = new QDialog;

	ShowUserInfoBtn = new QPushButton;
	LogioBtn1 = new QPushButton;
	SearchBtn = new QPushButton;
	BorrowBtn = new QPushButton;
	ReturnBtn = new QPushButton;
	ExtendBtn = new QPushButton;
	OrderedBtn = new QPushButton;
	NextdayBtn1 = new QPushButton;
	ShowUserInfoBtn->setText(QString::fromWCharArray(L"用户信息"));
	LogioBtn1->setText(QString::fromWCharArray(L"登录/登出"));
	SearchBtn->setText(QString::fromWCharArray(L"搜索"));
	BorrowBtn->setText(QString::fromWCharArray(L"借书"));
	ReturnBtn->setText(QString::fromWCharArray(L"还书"));
	ExtendBtn->setText(QString::fromWCharArray(L"续借"));
	OrderedBtn->setText(QString::fromWCharArray(L"预约情况"));
	NextdayBtn1->setText(QString::fromWCharArray(L"下一天"));

	ShowUserInfoBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	LogioBtn1->setStyleSheet("color: white;background-image:url(btn.png);");
	SearchBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	BorrowBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	ReturnBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	ExtendBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	OrderedBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	NextdayBtn1->setStyleSheet("color: white;background-image:url(btn.png);");

	time1 = new QLabel;
	time1->setText(QString::fromStdString(currentTime->date()));
	time1->setStyleSheet("color:black");

	UserleftBtnBox = new QVBoxLayout(UsermenuDlg);
	UserleftBtnBox->addSpacing(HEIGHT / 10);
	UserleftBtnBox->addWidget(ShowUserInfoBtn);
	UserleftBtnBox->addWidget(SearchBtn);
	UserleftBtnBox->addWidget(BorrowBtn);
	UserleftBtnBox->addWidget(ReturnBtn);
	UserleftBtnBox->addWidget(ExtendBtn);
	UserleftBtnBox->addWidget(OrderedBtn);
	UserleftBtnBox->addWidget(NextdayBtn1);
	UserleftBtnBox->addWidget(LogioBtn1);
	UserleftBtnBox->setSpacing(HEIGHT / 50);
	UserleftBtnBox->addWidget(time1);

	connect(ShowUserInfoBtn, SIGNAL(clicked()), this, SLOT(ShowUserInfo()));
	connect(SearchBtn, SIGNAL(clicked()), this, SLOT(ShowSearch()));
	connect(BorrowBtn, SIGNAL(clicked()), this, SLOT(ShowBorrow()));
	connect(ReturnBtn, SIGNAL(clicked()), this, SLOT(ShowReturn()));
	connect(ExtendBtn, SIGNAL(clicked()), this, SLOT(ShowExtend()));
	connect(OrderedBtn, SIGNAL(clicked()), this, SLOT(ShowReserved()));
	connect(NextdayBtn1, SIGNAL(clicked()), this, SLOT(ChangeDate()));
	connect(LogioBtn1, SIGNAL(clicked()), this, SLOT(LoginOrOut()));
}
void MainWindow::CreateAdminMenuDlg()
{
	AdminmenuDlg = new QDialog;
	PurchaseBtn = new QPushButton;
	ShowBorrowedBtn = new QPushButton;
	UsersInfoBtn = new QPushButton;
	NextdayBtn2 = new QPushButton;
	LogioBtn2 = new QPushButton;
	registBtn = new QPushButton;
	ShowLogBtn = new QPushButton;
	ChangeBookBtn = new QPushButton;


	PurchaseBtn->setText(QString::fromWCharArray(L"购入书籍"));
	ShowBorrowedBtn->setText(QString::fromWCharArray(L"图书借阅情况"));
	ChangeBookBtn->setText(QString::fromWCharArray(L"修改图书信息"));
	UsersInfoBtn->setText(QString::fromWCharArray(L"所有用户信息"));
	registBtn->setText(QString::fromWCharArray(L"注册新用户"));
	ShowLogBtn->setText(QString::fromWCharArray(L"查询访问日志"));
	LogioBtn2->setText(QString::fromWCharArray(L"登录/登出"));
	NextdayBtn2->setText(QString::fromWCharArray(L"下一天"));

	PurchaseBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	ShowBorrowedBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	ChangeBookBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	UsersInfoBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	registBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	ShowLogBtn->setStyleSheet("color: white;background-image:url(btn.png);");
	LogioBtn2->setStyleSheet("color: white;background-image:url(btn.png);");
	NextdayBtn2->setStyleSheet("color: white;background-image:url(btn.png);");

	time2 = new QLabel;
	time2->setText(QString::fromStdString(currentTime->date()));
	time2->setStyleSheet("color:black");

	AdminleftBtnBox = new QVBoxLayout(AdminmenuDlg);
	AdminleftBtnBox->addSpacing(HEIGHT / 10);
	AdminleftBtnBox->addWidget(PurchaseBtn);
	AdminleftBtnBox->addWidget(ShowBorrowedBtn);
	AdminleftBtnBox->addWidget(ChangeBookBtn);
	AdminleftBtnBox->addWidget(UsersInfoBtn);
	AdminleftBtnBox->addWidget(registBtn);
	AdminleftBtnBox->addWidget(ShowLogBtn);
	AdminleftBtnBox->addWidget(NextdayBtn2);
	AdminleftBtnBox->addWidget(LogioBtn2);
	AdminleftBtnBox->setSpacing(HEIGHT / 50);
	AdminleftBtnBox->addWidget(time2);

	connect(PurchaseBtn, SIGNAL(clicked()), this, SLOT(ShowPurchase()));
	connect(ShowBorrowedBtn, SIGNAL(clicked()), this, SLOT(ShowBorrowed()));
	connect(ChangeBookBtn, SIGNAL(clicked()), this, SLOT(ShowSearch()));
	connect(UsersInfoBtn, SIGNAL(clicked()), this, SLOT(ShowUsersInfo()));
	connect(LogioBtn2, SIGNAL(clicked()), this, SLOT(LoginOrOut()));
	connect(NextdayBtn2, SIGNAL(clicked()), this, SLOT(ChangeDate()));
	connect(registBtn, SIGNAL(clicked()), this, SLOT(ShowRegister()));
	connect(ShowLogBtn, SIGNAL(clicked()), this, SLOT(ShowLog()));
}
void MainWindow::CreateCurrentDlg()
{
	currentDlg = new QDialog;
	menuDlg = new QDialog;
}
void MainWindow::Login()
{
	string username = this->nameLine->text().toStdString();
	string password = this->passwordLine->text().toStdString();
	string type = this->typeCombo->currentText().toStdString();

	int result;
	if (type == QString::fromWCharArray(L"用户").toStdString()) result = library.UserLogin(username, password);
	else if (type == QString::fromWCharArray(L"管理员").toStdString())result = library.AdminLogin(username, password);
	else QMessageBox::critical(this, QString::fromWCharArray(L"登录失败"), QString::fromWCharArray(L"未知错误"));

	switch (result) {
	case SUCCESS:
		logindlg->close();
		if (type == QString::fromWCharArray(L"用户").toStdString())
		{
			QMessageBox::information(this, QString::fromWCharArray(L"成功"), QString::fromWCharArray(L"欢迎你，亲爱的") + QString::fromStdString(library.currentUser->real_name));
			if (library.currentUser->v_reserved_book.size() > 0)
				QMessageBox::information(this, QString::fromWCharArray(L"预约信息"), QString::fromWCharArray(L"你有预约的书可以借阅，请前往“预约情况进行查看”"));
		}
		else QMessageBox::information(this, QString::fromWCharArray(L"成功"), QString::fromWCharArray(L"欢迎你,亲爱的的管理员"));
		ChangemenuDlg();
		this->nameLine->clear();
		this->passwordLine->clear();
		break;
	case FALSE_ACCOUNT:
		QMessageBox::critical(this, QString::fromWCharArray(L"登陆失败"), QString::fromWCharArray(L"用户名错误"));
		break;
	case FALSE_PASSWORD:
		QMessageBox::critical(this, QString::fromWCharArray(L"登陆失败"), QString::fromWCharArray(L"密码错误"));
		break;
	default:
		QMessageBox::critical(this, QString::fromWCharArray(L"登陆失败"), QString::fromWCharArray(L"未知错误,返回值为") + QString::number(result, 10));
		break;
	}
}
void MainWindow::ShowRegister()
{
	Registerdlg *registerdlg = new Registerdlg;
	registerdlg->show();
}
void MainWindow::ShowUserInfo()
{
	timer->stop();
	if (library.currentUser == NULL && library.currentAdmin == NULL)
		logindlg->show();
	else
	{
		if (currentDlg != infodlg)
			bottomLayout->replaceWidget(currentDlg, infodlg);
		currentDlg->hide();
		currentDlg = infodlg;
		infodlg->UserInfo();
		currentDlg->show();
	}
}
void MainWindow::ShowBorrowed()
{
	timer->stop();
	if (library.currentUser == NULL && library.currentAdmin == NULL)
		logindlg->show();
	else
	{
		currentDlg->hide();
		if (currentDlg != booksdlg)
			bottomLayout->replaceWidget(currentDlg, booksdlg);
		currentDlg = booksdlg;
		booksdlg->ShowResults();
		currentDlg->show();
	}
}
void MainWindow::ShowUsersInfo()
{
	timer->stop();
	if (library.currentUser == NULL && library.currentAdmin == NULL)
		logindlg->show();
	else
	{
		currentDlg->hide();
		if (currentDlg != userinfodlg)
			bottomLayout->replaceWidget(currentDlg, userinfodlg);
		currentDlg = userinfodlg;
		userinfodlg->infoTabel->ShowUsers();
		currentDlg->show();
	}
}
void MainWindow::ShowSearch()
{
	timer->stop();
	if (currentDlg != searchdlg)
		bottomLayout->replaceWidget(currentDlg, searchdlg);
	currentDlg->hide();
	currentDlg = searchdlg;
	searchdlg->resultTable->hide();
	searchdlg->btnDlg->hide();
	currentDlg->show();
}
void MainWindow::ShowBorrow()
{
	timer->stop();
	if (library.currentUser == NULL)
	{
		logindlg->show();
	}
	else
	{
		ShowSearch();
	}
}
void MainWindow::ShowReturn()
{
	timer->stop();
	if (library.currentUser == NULL)
	{
		logindlg->show();
	}
	else
	{
		currentDlg->hide();
		if (currentDlg != returndlg)
			bottomLayout->replaceWidget(currentDlg, returndlg);
		currentDlg = returndlg;
		returndlg->borrowedDlg->ShowBorrowed(library.currentUser);
		currentDlg->show();
	}
}
void MainWindow::ShowExtend()
{
	timer->stop();
	if (library.currentUser == NULL)
	{
		logindlg->show();
	}
	else
	{
		currentDlg->hide();
		if (currentDlg != extenddlg)
			bottomLayout->replaceWidget(currentDlg, extenddlg);
		currentDlg = extenddlg;
		extenddlg->borrowedDlg->ShowBorrowed(library.currentUser);
		currentDlg->show();
	}
}
void MainWindow::ShowReserved()
{
	timer->stop();
	if (library.currentUser == NULL)
	{
		logindlg->show();
	}
	else
	{
		currentDlg->hide();
		if (currentDlg != reserveddlg)
			bottomLayout->replaceWidget(currentDlg, reserveddlg);
		currentDlg = reserveddlg;
		reserveddlg->reservedTable->ShowReserved(library.currentUser);
		currentDlg->show();
	}
}
void MainWindow::ShowPurchase()
{
	timer->stop();
	if (library.currentUser == NULL && library.currentAdmin == NULL)
		logindlg->show();
	else if (library.currentAdmin == NULL)
		QMessageBox::critical(this, tr("Purchase Failed"), tr("You Are Not The Administrator!"));
	else
	{
		currentDlg->hide();
		if (currentDlg != purchasedlg)
			bottomLayout->replaceWidget(currentDlg, purchasedlg);
		currentDlg = purchasedlg;
		currentDlg->show();

	}
}
void MainWindow::ShowLog()
{
	QProcess* process = new QProcess();
	QString notepadPath = "notepad.exe  .\\data\\Log.txt";
	process->start(notepadPath);
}
void MainWindow::ChangeDate()
{
	currentTime->NextDay();
	time1->setText(QString::fromWCharArray(L"当前时间：") + QString::fromStdString(currentTime->date()));
	time2->setText(QString::fromWCharArray(L"当前时间：") + QString::fromStdString(currentTime->date()));
	timer->start(MS_PER_DAY);
}
void MainWindow::LoginOrOut()
{
	timer->stop();
	if (library.currentUser != NULL)
	{
		library.UserLogout();
		ChangemenuDlg();
		QMessageBox::information(this, QString::fromWCharArray(L"登出成功"), QString::fromWCharArray(L"登出成功"));
	}
	else if (library.currentAdmin != NULL)
	{
		library.AdminLogout();
		ChangemenuDlg();
		QMessageBox::information(this, QString::fromWCharArray(L"登出成功"), QString::fromWCharArray(L"登出成功"));
	}
	else
	{
		logindlg->show();
	}
}
void MainWindow::ChangemenuDlg()
{
	if (library.currentAdmin != NULL)
	{
		searchdlg->borrowBtn->setText(QString::fromWCharArray(L"修改信息"));
		if (menuDlg != AdminmenuDlg)
			bottomLayout->replaceWidget(menuDlg, AdminmenuDlg);
		menuDlg->hide();
		menuDlg = AdminmenuDlg;
		menuDlg->show();
	}
	else
	{
		searchdlg->borrowBtn->setText(QString::fromWCharArray(L"借阅"));
		if (menuDlg != UsermenuDlg)
			bottomLayout->replaceWidget(menuDlg, UsermenuDlg);
		menuDlg->hide();
		menuDlg = UsermenuDlg;
		menuDlg->show();
	}
	currentDlg->hide();
	if (currentDlg != rankdlg)
		bottomLayout->replaceWidget(currentDlg, rankdlg);
	rankdlg->ShowRank();
	currentDlg = rankdlg;
	currentDlg->show();
}
MainWindow::~MainWindow()
{

}
