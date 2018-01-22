#include "library.h"
#include <QStringMatcher>
//----------------------- Staff --------------------------------

Staff::Staff(string line)
{
    StringParse s(line, ",");
	if (s.str[0].length() <= LENGTH_STAFF_NAME)
		this->user_name = s.str[0];
	else
		this->user_name = s.str[0].substr(0, LENGTH_STAFF_NAME);

	if (s.str[1].length() <= LENGTH_STAFF_PASSWORD)
		this->password = s.str[1];
	else
		this->password = s.str[1].substr(0, LENGTH_STAFF_PASSWORD);
    this->honesty = 100;
}

Staff::~Staff()
{
    // nothing?
}

Staff::Staff(const Staff &s)
{
    this->honesty = s.honesty;
    this->index = s.index;
    this->password = s.password;
    this->user_name = s.user_name;
}

int Staff::Check(string password)
{
    if (this->password == password)
        return SUCCESS;
    else
        return FALSE_PASSWORD;
}

void Staff::ChangePassWord(string line)
{
    this->password = line;
}

int Staff::Edit(string line)
{
	try {
		StringParse s(line, ",");
		if (s.str[0].length() <= LENGTH_STAFF_NAME)
			this->user_name = s.str[0];
		else
			this->user_name = s.str[0].substr(0, LENGTH_STAFF_NAME);

		if (s.str[1].length() <= LENGTH_STAFF_PASSWORD)
			this->password = s.str[1];
		else
			this->password = s.str[1].substr(0, LENGTH_STAFF_PASSWORD);

		try {
			this->honesty = stoi(s.str[2]);
		}
		catch (invalid_argument &a) {
			return FAIL;
		}
		return SUCCESS;
	}
	catch (out_of_range &o) {
		return FAIL;
	}
}

//----------------------- User --------------------------------

User::User(string line, int index) :Staff(line)
{
	try {
		StringParse s(line, ",");
		try {
			this->type = std::stoi(s.str[2]);
		}
		catch (invalid_argument& a) {
			this->type = 0;
		}

		if (s.str[3].length() <= LENGTH_REAL_NAME)
			this->real_name = s.str[3];
		else
			this->real_name = s.str[3].substr(0, LENGTH_REAL_NAME);

		if (s.str[4].length() <= LENGTH_USER_SCHOOL)
			this->school = s.str[4];
		else
			this->school = s.str[4].substr(0, LENGTH_USER_SCHOOL);

		this->honesty = 100;
		this->index = index;
	}
	catch (std::out_of_range &o) {
		this->honesty = 0;
	}
    
}

User::User(int index)
{
    this->index = index;

    fstream fp(".\\data\\Users.txt", ios::in | ios::out);
    fp.seekg(index * LENGTH_USER_LINE + 3, ios::beg);
    string line;
    //fp >> setw(LENGTH_BOOK_LINE) >> line;
    getline(fp, line);

    StringParse s(line, ",");
    int j = -1;
    this->user_name = s.str[++j];
    this->password = s.str[++j];
    this->type = std::stoi(s.str[++j]);
    this->real_name = s.str[++j];
    this->school = s.str[++j];
    this->honesty = std::stoi(s.str[++j]);

    int i = 0;
    while (i < MAX_ORDER_NUM) {
        int book_index = std::stoi(s.str[++j]);
        if (book_index != NONE) {
            Reserved_Book o(book_index, std::stoi(s.str[j+1]), std::stoi(s.str[j+2]), *this);
            this->reserved_book.push_back(o);

            if (o.v_time != NONE) {
                this->v_reserved_book.push_back(o);
            }
        }
        j += 2;
        i++;
    }

    i = 0;
    while (i < TEACHER_LIMIT)
    {
        int book_index = std::stoi(s.str[++j]);
        if (book_index != NONE) {
            Borrowed_Book b(book_index, std::stoi(s.str[j+1]), std::stoi(s.str[j+2]), std::stoi(s.str[j+3]));
            this->borrowed_book.push_back(b);
        }
        j += 3;
        i++;
    }

    fp.close();
}

User::~User()
{
    this->borrowed_book.clear();
    this->reserved_book.clear();
    this->v_reserved_book.clear();
}

User::User(const User &u)
{
    this->borrowed_book = u.borrowed_book;
    this->reserved_book = u.reserved_book;
    this->v_reserved_book = u.v_reserved_book;
    this->honesty = u.honesty;
    this->index = u.index;
    this->password = u.password;
    this->type = u.type;
    this->school = u.school;
    this->user_name = u.user_name;
	this->real_name = u.real_name;
}

void User::Save()
{
    fstream fp(".\\data\\Users.txt", ios::in | ios::out);
    fp.seekg(this->index * LENGTH_USER_LINE + 3, ios::beg);

    fp << setfill(' ') << setw(LENGTH_STAFF_NAME) << this->user_name << ',';
    fp << setfill(' ') << setw(LENGTH_STAFF_PASSWORD) << this->password << ',';
    fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->type) << ',';
    fp << setfill(' ') << setw(LENGTH_REAL_NAME) << this->real_name << ',';
    fp << setfill(' ') << setw(LENGTH_USER_SCHOOL) << this->school << ',';
    fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->honesty) << ',';

    int i = 0;
    while (i < MAX_ORDER_NUM) {
        if (i >= this->reserved_book.size()) {
            fp << setfill(' ') << setw(LENGTH_NUM + 1) << "-1,";
            fp << setfill(' ') << setw(LENGTH_NUM + 1) << "-1,";
            fp << setfill(' ') << setw(LENGTH_NUM + 1) << "-1,";
        }
        else {
            fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->reserved_book[i].book) << ',';
            fp << setfill(' ') << setw(LENGTH_NUM) << to_string((int)this->reserved_book[i].reserve_time) << ',';
            fp << setfill(' ') << setw(LENGTH_NUM) << to_string((int)this->reserved_book[i].v_time) << ',';
        }

        i++;
    }

    i = 0;
    while (i < TEACHER_LIMIT) {
        if (i >= this->borrowed_book.size()) {
            fp << setfill(' ') << setw(LENGTH_NUM + 1) << "-1,";
            fp << setfill(' ') << setw(LENGTH_NUM + 1) << "-1,";
            fp << setfill(' ') << setw(LENGTH_NUM + 1) << "-1,";
            if (i == TEACHER_LIMIT - 1)
                fp << setfill(' ') << setw(LENGTH_NUM + 1) << "-1\n";
            else
                fp << setfill(' ') << setw(LENGTH_NUM + 1) << "-1,";
        }
        else {
            fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->borrowed_book[i].book->get_index()) << ',';
            fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->borrowed_book[i].book_num) << ',';
            fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->borrowed_book[i].start_time) << ',';
            if (i == TEACHER_LIMIT - 1)
                fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->borrowed_book[i].keep_time) << '\n';
            else
                fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->borrowed_book[i].keep_time) << ',';
        }

        i++;
    }

    fp.close();
}

int User::Borrow(Book &b, int keep_time)
{
    //检测诚信是否达到要求
    if (this->honesty < BORROW_LIMIT)
        return DISHONESTY;

    //检测借书数是否达到上限
    else if (this->type == STUDENT && this->borrowed_book.size() >= STU_LIMIT)
        return NO_ACCESS;
    else if (this->type == TEACHER && this->borrowed_book.size() >= TEACHER_LIMIT)
        return NO_ACCESS;

	/*
    vector<Borrowed_Book>::iterator it = borrowed_book.begin();
    while (it != borrowed_book.end()) {
        if (*(it->book) == b)
            break;
        it++;
    }
    if (it != borrowed_book.end())
        return FAIL;
	*/
	if (b.get_v2_num() <= 0)
		return BOOK_NOT_LEFT;
	else {
		this->borrowed_book.push_back(Borrowed_Book(b, keep_time));
		this->Save();
		fstream fl(".\\data\\Log.txt", ios::app);
		fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << this->user_name << QString::fromWCharArray(L" 借阅书籍《").toStdString();
		fl << b.get_name() << QString::fromWCharArray(L"》(流水号: ").toStdString() << b.get_ISBN();
		fl << setfill('0') << setw(2) << borrowed_book.back().book_num;
		fl << QString::fromWCharArray(L") ").toStdString() << keep_time << QString::fromWCharArray(L"天").toStdString() << endl;
		fl.close();
		return SUCCESS;
	}
}

int User::Borrow(Reserved_Book &b, int keep_time)
{
    //检测诚信是否达到要求
    if (this->honesty < BORROW_LIMIT)
        return DISHONESTY;

    //检测借书数是否达到上限
    else if (this->type == STUDENT && this->borrowed_book.size() >= STU_LIMIT)
        return NO_ACCESS;
    else if (this->type == TEACHER && this->borrowed_book.size() >= TEACHER_LIMIT)
        return NO_ACCESS;

    vector<Reserved_Book>::iterator it = find(reserved_book.begin(), reserved_book.end(), b);
    vector<Reserved_Book>::iterator itt = find(v_reserved_book.begin(), v_reserved_book.end(), *it);
    if (it == reserved_book.end() || itt == v_reserved_book.end())
        return SYN_ERROR;

    reserved_book.erase(it);
    v_reserved_book.erase(itt);
    Book bb = (Book)b;
    bb.ReservingCancel();
    this->borrowed_book.push_back(Borrowed_Book(bb, keep_time));
    this->Save();

    fstream fl(".\\data\\Log.txt", ios::app);
    fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << this->user_name << QString::fromWCharArray(L" 借阅书籍《").toStdString();
    fl << this->borrowed_book.back().book->get_name() << QString::fromWCharArray(L"》(流水号: ").toStdString() << this->borrowed_book.back().book->get_ISBN();
    fl << setfill('0') << setw(2) << borrowed_book.front().book_num;
    fl << ") " << keep_time << QString::fromWCharArray(L"天").toStdString() << endl;
    fl.close();
    return SUCCESS;
}

int User::Return(Book &b)
{
    vector<Borrowed_Book>::iterator it = borrowed_book.begin();
    while (it != borrowed_book.end()) {
        if (*(it->book) == b)
            break;
        it++;
    }
    if (it == borrowed_book.end())
        return FAIL;

    int od = it->OverDue();
    if (od > 0) {
        this->honesty -= od;
        int bn = it->book_num;
        it->Return();
        this->borrowed_book.erase(it);
        this->Save();
        fstream fl(".\\data\\Log.txt", ios::app);
        fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << this->user_name << QString::fromWCharArray(L" 归还书籍《").toStdString();
        fl << b.get_name() << QString::fromWCharArray(L"》(流水号: ").toStdString() << b.get_ISBN();
        fl << setfill('0') << setw(2) << bn;
        fl << QString::fromWCharArray(L") ").toStdString() << QString::fromWCharArray(L"逾期").toStdString() << od << QString::fromWCharArray(L"天").toStdString() << endl;
        fl.close();
        return od;
    }
    else {
        int bn = it->book_num;
        it->Return();
        this->borrowed_book.erase(it);
        this->Save();
        fstream fl(".\\data\\Log.txt", ios::app);
        fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << this->user_name << QString::fromWCharArray(L" 归还书籍《").toStdString();
        fl << b.get_name() << QString::fromWCharArray(L"》(流水号: ").toStdString() << b.get_ISBN();
        fl << setfill('0') << setw(2) << bn;
        fl << QString::fromWCharArray(L") ").toStdString() << QString::fromWCharArray(L"未逾期").toStdString() << endl;
        fl.close();
        return SUCCESS;
    }
}

int User::Reserve(Book &b)
{
    //检测诚信是否达到要求
    if (this->honesty < ORDER_LIMIT)
        return NO_ACCESS;

    vector<Borrowed_Book>::iterator it = borrowed_book.begin();
    while (it != borrowed_book.end()) {
        if (*(it->book) == b)
            break;
        it++;
    }
    if (it != borrowed_book.end())
        return FAIL;

    if (this->reserved_book.size() <= MAX_ORDER_NUM && b.Reserve() == SUCCESS) {
        Reserved_Book o(b, *this);
        this->reserved_book.push_back(o);
        this->Save();

        fstream fl(".\\data\\Log.txt", ios::app);
        fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << this->user_name << QString::fromWCharArray(L" 预约书籍《").toStdString();
        fl << b.get_name() << QString::fromWCharArray(L"》,(ISBN: ").toStdString() << b.get_ISBN() << QString::fromWCharArray(L")").toStdString() << endl;
        fl.close();
        return SUCCESS;
    }
    else
        return FAIL;
}

int User::ReserveCancel(Reserved_Book &b)
{
    vector<Reserved_Book>::iterator it = find(reserved_book.begin(), reserved_book.end(), b);
    if (it == this->reserved_book.end())
        return FAIL;
    vector<Reserved_Book>::iterator itt = find(v_reserved_book.begin(), v_reserved_book.end(), *it);

    Book bb(b.book);
    if (bb.ReserveCancel() != SUCCESS)
        return FAIL;
    if (itt != v_reserved_book.end()) {
        this->v_reserved_book.erase(itt);
        bb.Reserve();
        bb.ReservingCancel();
    }
    this->reserved_book.erase(it);
    this->Save();

    fstream fl(".\\data\\Log.txt", ios::app);
    fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << this->user_name << QString::fromWCharArray(L" 取消预约书籍《").toStdString();
    fl << bb.get_name() << QString::fromWCharArray(L"》,(ISBN: ").toStdString() << bb.get_ISBN() << QString::fromWCharArray(L")").toStdString() << endl;
    fl.close();
    return SUCCESS;
}

int User::Extend(Book &b, int extend_time)
{
    if (this->honesty < ORDER_LIMIT)
        return NO_ACCESS;

    vector<Borrowed_Book>::iterator it = borrowed_book.begin();
    while (it != borrowed_book.end()) {
        if (*(it->book) == b)
            break;
        it++;
    }
    if (it == borrowed_book.end())
        return FAIL;

    int bn = it->book_num;
    it->Extend(extend_time);
    this->Save();
    fstream fl(".\\data\\Log.txt", ios::app);
    fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << this->user_name << QString::fromWCharArray(L" 续借书籍《").toStdString();
    fl << b.get_name() << QString::fromWCharArray(L"》(流水号: ").toStdString() << b.get_ISBN();
    fl << setfill('0') << setw(2) << bn;
    fl << QString::fromWCharArray(L") ").toStdString() << extend_time << QString::fromWCharArray(L"天").toStdString() << endl;
    fl.close();
    return SUCCESS;
}

int User::Edit(string line)
{
	try {
		StringParse s(line, ",");
		try {
			this->type = std::stoi(s.str[3]);
		}
		catch (invalid_argument& a) {
			this->type = 0;
		}

		if (s.str[4].length() <= LENGTH_REAL_NAME)
			this->real_name = s.str[4];
		else
			this->real_name = s.str[4].substr(0, LENGTH_REAL_NAME);

		if (s.str[5].length() <= LENGTH_USER_SCHOOL)
			this->school = s.str[5];
		else
			this->school = s.str[5].substr(0, LENGTH_USER_SCHOOL);

		this->index = index;
		int state = Staff::Edit(line);
		if (state == SUCCESS)
			this->Save();
		return state;
	}
	catch (std::out_of_range &o) {
		return FAIL;
	}
}

//----------------------- Administrator --------------------------------

Administrator::Administrator(string line) :Staff(line)
{
    // seems nothing here...
}

Administrator::~Administrator()
{
    // well...
}

Administrator::Administrator(const Administrator &a)
{
    this->honesty = a.honesty;
    this->index = a.index;
    this->password = a.password;
    this->user_name = a.user_name;
}

int Administrator::Purchase(Search_Result &s, Book &b)
{
    Book ori_book(s.book[0]);
    ori_book += b;
    ori_book.Save();
    return SUCCESS;
}

void Administrator::Save()
{
    fstream fp(".\\data\\Users.txt", ios::app);
    fp << setfill(' ') << setw(LENGTH_STAFF_NAME) << this->user_name << ',';
    fp << setfill(' ') << setw(LENGTH_STAFF_PASSWORD) << this->password << '\n';
    fp.close();
}
