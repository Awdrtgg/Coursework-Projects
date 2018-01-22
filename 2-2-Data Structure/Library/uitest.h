#include "library.h"
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

#define WIDTH QApplication::desktop()->width() * 3 / 4
#define HEIGHT QApplication::desktop()->height() * 9 / 10

//-------------------------User-----------------------------------

class Changepassdlg : public QDialog
{
	Q_OBJECT
private:
	QLabel *passwordLabel;
	QLabel *repasswordLabel;
	QLineEdit *passwordLine;
	QLineEdit *repasswordLine;
	QPushButton *okBtn;
	QPushButton *cancelBtn;
	QHBoxLayout *passwordLayout;
	QHBoxLayout *repasswordLayout;
	QHBoxLayout *btnnLayout;
	QVBoxLayout *mainLayout;

public:
	Changepassdlg();
	~Changepassdlg();
	private slots:
	void Change();
};
class Confirmpassdlg : public QDialog
{
	Q_OBJECT
private:
	QLabel *passwordLabel;
	QPushButton *okBtn;
	QPushButton *cancelBtn;
	QHBoxLayout *passwordLayout;
	QHBoxLayout *btnnLayout;
	QVBoxLayout *mainLayout;
	QLineEdit *passwordLine;

	Changepassdlg *changedlg;
public:
	Confirmpassdlg();
	~Confirmpassdlg();
	private slots:
	void Confirm();
};
class Infodlg : public QDialog
{

	Q_OBJECT
public:
	Infodlg();
	~Infodlg();
	void UserInfo();
private:
	QLabel *nameLabel;
	QLabel *IDLabel;
	QLabel *typeLabel;
	QLabel *schoolLabel;
	QLabel *honestyLabel;
	QLabel *borrowlimitLabel;
	QLabel *borrowednumLabel;
	QLabel *reservelimLabel;
	QLabel *reservesLabel;
	QPushButton *changepasswordBtn;
	QVBoxLayout *mainLayout;

	Confirmpassdlg *confirmdlg;
	private slots:
	void ChangePassWord();
};
class Searchdlg : public QDialog
{

	Q_OBJECT

public:
	Searchdlg();
	~Searchdlg();
	void CreateSearch();
	void CreateResults();
	void CreateButtons();
	void ShowSearchResult();
	void Reserve();
	QDialog *btnDlg;
	QPushButton *firstBtn;
	QPushButton *preBtn;
	QPushButton *nextBtn;
	QPushButton *lastBtn;
	QLineEdit *jumpLine;
	QPushButton *jumpBtn;
	QPushButton *borrowBtn;
	QPushButton *detailBtn;
	QWidget *searchWidget;
	QTableWidget *resultTable;
	QVBoxLayout *detailLayout;
private:
	int pages;
	int currentpage;

	QLabel *pageLabel;
	QLabel *jumpLabel1;
	QLabel *jumpLabel2;

	QComboBox *typeCombo;
	QLabel *keywordLabel;
	QLineEdit *valueLine;
	QPushButton *searchBtn;

	QGridLayout *mainLayout;

	//for ChangeInfo
	QPushButton *changeLabel;
	QDialog *change;
	QLabel *nameLabel;
	QLineEdit *nameLine;
	QLabel *org_nameLabel;
	QLineEdit *org_nameLine;
	QLabel *publisherLabel;
	QLineEdit *publisherLine;
	QLabel *writerLabel;
	QLineEdit *writerLine;
	QLabel *translatorLabel;
	QLineEdit *translatorLine;
	QLabel *publish_timeLabel;
	QLineEdit *publish_timeLine;
	QLabel *introLabel;
	QTextEdit *introTxt;
	QLabel *key_wordLabel;
	QLineEdit *key_wordLine;
	QPushButton *change_keywBtn;
	QLabel *ratingLabel;
	QLineEdit *ratingLine;
	QLabel *page_numLabel;
	QLineEdit *page_numLine;
	QLabel *purchase_timeLabel;
	// QDateEdit *purchase_timeDate;
	QLineEdit *purchase_timeLine;
	QLabel *ISBNLabel;
	QLineEdit *ISBNLine;
	QLabel *pic;
	QPixmap *pict;
	QString url;
	QPushButton *changepicBtn;
	QPushButton *okBtn;
	QPushButton *cancelBtn;
	QHBoxLayout *btnBox;
	QVBoxLayout *InfoLayout1;
	QVBoxLayout *InfoLayout2;
	QHBoxLayout *InfoLayout3;
	QVBoxLayout *InfoLayout;
	QVBoxLayout *PicLayout;

	//for CheckBox
	QDialog *CheboxDlg;
	vector<QCheckBox*> checkboxs;
	QPushButton *check_okBtn;
	QPushButton *check_cancelBtn;
	QGridLayout *check_mainLayout;

	private slots:
	void Search();
	void ShowFirstPage();
	void ShowPrePage();
	void ShowNextPage();
	void ShowLastPage();
	void ShowJumpedPage();
	void Borrow();
	void ShowDetail();

	//for Admin
	void CreateChangeDlg();
	void ChangeInfo();
	void ChangePic();
	void CreateCheckBox();
	void ShowCheckBox();
	void ChangeKey();
};
class Borroweddlg :public QTableWidget
{
	Q_OBJECT

public:
	Borroweddlg();
	~Borroweddlg();
	void ShowBorrowed(User *u);
	vector<Borrowed_Book> borrowed_book;
};
class ReservedTable :public QTableWidget
{
	Q_OBJECT
public:
	ReservedTable();
	~ReservedTable();
	void ShowReserved(User *u);
	vector<Reserved_Book> reserved_book;
};
class Reserveddlg :public QDialog
{
	Q_OBJECT
private:
	QGridLayout *mainLayout;
	QDialog *btnDlg;
	QPushButton *borrowBtn;
	QPushButton *cancelresBtn;
public:
	ReservedTable *reservedTable;
	void CreateResults();
	void CreateButtons();
	Reserveddlg();
	~Reserveddlg();
	private slots:
	void Borrow();
	void CancelReserve();
};
class Returndlg : public QDialog
{
	Q_OBJECT

public:
	Returndlg();
	~Returndlg();
	Borroweddlg *borrowedDlg;
private:
	QGridLayout *mainLayout;
	QPushButton *returnBtn;
	private slots:
	void Return();
};
class Extenddlg : public QDialog
{
	Q_OBJECT

public:
	Extenddlg();
	~Extenddlg();
	Borroweddlg *borrowedDlg;
private:
	QGridLayout *mainLayout;
	QLabel *extendLabel1;
	QLabel *extendLabel2;
	QComboBox *daysCombo;
	QPushButton *extendBtn;
	private slots:
	void Extend();
};

//----------------------Administer-----------------------------

class Registerdlg : public QDialog
{
	Q_OBJECT

public:
	Registerdlg();
	~Registerdlg();

private:
	QLabel *typeLabel;          //鐢ㄦ埛绫诲瀷锛堝鐢燂紝鏁欏笀锛岀鐞嗗憳锛?
	QLabel *nameLabel;          //濮撳悕
	QLabel *collegeLabel;       //瀛﹂櫌
	QLabel *idLabel;            //璇佷欢鍙?
	QLabel *passwordLabel;      //杈撳叆瀵嗙爜
	QLabel *repasswordLabel;    //纭瀵嗙爜

	QComboBox *typeCombo;
	QLineEdit *nameLine;
	QComboBox *collegeCombo;
	QLineEdit *idLine;
	QLineEdit *passwordLine;
	QLineEdit *repasswordLine;

	QPushButton *okBtn;
	QPushButton *cancelBtn;
	QPushButton *byfileBtn;

	QHBoxLayout *buttonBox;
	QGridLayout * mainLayout;

	private slots:
	void Register();
	void RegisterByFile();
};
class Purchasedlg : public QDialog
{
	Q_OBJECT

public:
	Purchasedlg();
	~Purchasedlg();
	void CreateCheckbox();
private:
	QLabel *CHnameLabel;
	QLabel *NEnameLabel;
	QLabel *publisherLabel;
	QLabel *writerLabel;
	QLabel *translatorLabel;
	QLabel *scoreLabel;
	QLabel *classLabel;
	QLabel *ISBNLabel;
	QLabel *dateLabel;
	QLabel *introLabel;
	QLabel *numLabel;
	QLabel *pagenumLabel;

	QLineEdit *CHnameLine;
	QLineEdit *NEnameLine;
	QLineEdit *publisherLine;
	QLineEdit *writerLine;
	QLineEdit *translatorLine;
	QLineEdit *scoreLine;
	QLineEdit *classLine;
	QLineEdit *ISBNLine;
	QDateEdit *dateEdit;
	QTextEdit *introTxt;
	QLineEdit *numLine;
	QSlider *numSlider;
	QLineEdit *pagenumLine;

	QLabel *picLabel;
	QPixmap *pic;
	QPushButton *addpicBtn;
	QString url;

	QPushButton *okBtn;
	QPushButton *cancelBtn;
	QPushButton *byfileBtn;

	QHBoxLayout *buttonBox;
	QGridLayout *mainLayout;

	//for checkbox
	QDialog *CheboxDlg;
	vector<QCheckBox*> checkboxs;
	QPushButton *check_okBtn;
	QPushButton *check_cancelBtn;
	QGridLayout *check_mainLayout;
	QPushButton *changclassBtn;
	private slots:
	void ChangeKey();
	void Purchase();
	void PurchaseByFile();
	void AddPicture();
	void setLineEditValue();
};
class UserInfoTabel : public QTableWidget
{
	Q_OBJECT

public:
	vector<User> users;
	UserInfoTabel();
	~UserInfoTabel();
	void ShowUsers();
};
class UserInfodlg : public QDialog
{
	Q_OBJECT
private:
	QPushButton *borrowedBtn;
	QPushButton *reservedBtn;
	QPushButton *changeBtn;
	QDialog *btnDlg;
	QDialog *changeDlg;
	QGridLayout *mainLayout;

	//for change
	QLabel *typeLabel;
	QLabel *nameLabel;
	QLabel *collegeLabel;
	QLabel *idLabel;
	QLabel *passwordLabel;
	QLabel *repasswordLabel;
	QLabel *honestyLabel;
	QComboBox *typeCombo;
	QLineEdit *nameLine;
	QComboBox *collegeCombo;
	QLineEdit *idLine;
	QLineEdit *passwordLine;
	QLineEdit *repasswordLine;
	QLineEdit *honestyLine;


	QPushButton *okBtn;
	QPushButton *cancelBtn;

	QHBoxLayout *buttonBox;
	QGridLayout *changeLayout;
	User user;
public:
	UserInfodlg();
	~UserInfodlg();
	UserInfoTabel *infoTabel;
	private slots:
	void ShowBorrowed();
	void ShowReserved();
	void ShowChanges();
	void Change();
};
class Booksdlg : public QDialog
{
	Q_OBJECT
private:
	vector<User> users;
	QTableWidget *booksTabel;
	QGridLayout *mainLayout;
public:
	void CreateTable();
	void ShowResults();
	Booksdlg();
	~Booksdlg();
	private slots:

};

//---------------------MainWindow------------------------------

class RankDlg : public QDialog
{
	Q_OBJECT
private:
	QLabel *titleLabel;
	QLabel *ranks[BORROW_RANK];
	QLabel *books[BORROW_RANK];
	QHBoxLayout *rankLayout[BORROW_RANK];
	QVBoxLayout *mainLayout;
	QHBoxLayout *totalLayout;
public:
	RankDlg();
	~RankDlg();
	void ShowRank();
};
class Titledlg : public QDialog
{
	Q_OBJECT
private:
	QLabel *welcomeLabel;
	QHBoxLayout *mainLayout;
	QPixmap *welcomepic;
public:
	Titledlg();
	~Titledlg();
};
class MainWindow : public QDialog
{
	Q_OBJECT

private:
	Infodlg *infodlg;
	Searchdlg *searchdlg;
	QDialog *logindlg;
	Registerdlg *registerdlg;
	Purchasedlg *purchasedlg;
	Returndlg *returndlg;
	Extenddlg *extenddlg;
	Reserveddlg *reserveddlg;
	UserInfodlg *userinfodlg;
	Booksdlg *booksdlg;
	Titledlg *titledlg;
	RankDlg *rankdlg;

	//UsermenuDlg
	QPushButton *LogioBtn1;
	QPushButton *SearchBtn;
	QPushButton *BorrowBtn;
	QPushButton *ReturnBtn;
	QPushButton *ShowUserInfoBtn;
	QPushButton *OrderedBtn;
	QPushButton *ExtendBtn;
	QPushButton *NextdayBtn1;
	QLabel *time1;

	//AdminmenuDlg
	QPushButton *LogioBtn2;
	QPushButton *PurchaseBtn;
	QPushButton *ShowBorrowedBtn;
	QPushButton *UsersInfoBtn;
	QPushButton *registBtn;
	QPushButton *NextdayBtn2;
	QPushButton *ShowLogBtn;
	QPushButton *ChangeBookBtn;
	QLabel *time2;

	//For Logindlg
	QLabel *typeLabel;
	QLabel *nameLabel;
	QLabel *passwordLabel;
	QComboBox *typeCombo;
	QLineEdit *nameLine;
	QLineEdit *passwordLine;
	QPushButton *loginBtn;
	QPushButton *cancelBtn;

	QGridLayout *loginLayout;
	QHBoxLayout *buttonBox;

	//Layout
	QDialog *menuDlg;
	QDialog *currentDlg;
	QDialog *UsermenuDlg;
	QDialog *AdminmenuDlg;
	QVBoxLayout *mainLayout;
	QHBoxLayout *topLayout;
	QHBoxLayout *bottomLayout;
	QVBoxLayout *UserleftBtnBox;
	QVBoxLayout *AdminleftBtnBox;

	QTimer *timer;
public:
	MainWindow();
	~MainWindow();
	void CreateUserMenuDlg();
	void CreateAdminMenuDlg();
	void CreateLoginDlg();
	void CreateCurrentDlg();
	void ChangemenuDlg();
	private slots:
	void Login();
	void ShowRegister();
	void ShowReturn();
	void ShowSearch();
	void ShowBorrow();
	void ShowExtend();
	void ShowUserInfo();
	void ShowReserved();
	void ChangeDate();
	void LoginOrOut();

	void ShowPurchase();
	void ShowBorrowed();
	void ShowUsersInfo();
	void ShowLog();
};
