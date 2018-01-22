#include "library.h"
#include <QStringMatcher>
vector<string>keywords;

Library::Library()
{
    string line; //对应文件中的某一行信息
    ifstream fb(".\\data\\Books.txt"), fs(".\\data\\Users.txt"), fa(".\\data\\Administrator.txt"), fk(".\\data\\KeyWord.txt"), fr(".\\data\\BorrowRank.txt");
    if (fb.fail() || fs.fail() || fa.fail()) {
        cout << "File not found" << fb.fail() << fs.fail() << fa.fail() << endl;
        return;
    }

    this->book_size = 0; // the internal number
    fb.seekg(3, ios::beg);
    while (getline(fb, line) && fb.good()) {
        StringParse s(line, ",", 5, 10, 10, 10, 15, 15);

        //将该书插入依各种信息查找的键树
        this->name.insert(s.str[0], this->book_size);
        if (s.str[1].length() > 0)
            this->name.insert(s.str[1], this->book_size);
        this->publisher.insert(s.str[2], this->book_size);
		if (s.str[3].length() > 0) {
			int find_pos = s.str[3].find("]") + 1;
			this->writer.insert(s.str[3].substr(find_pos, s.str[3].length() - find_pos), this->book_size);
		}

		if (s.str[4].length() > 0) {
			int find_pos = s.str[4].find("]") + 1;
			this->writer.insert(s.str[4].substr(find_pos, s.str[4].length() - find_pos), this->book_size);
		}
        if (s.str[6].length() > 0)
            this->keyword[KeyWord(s.str[6], HEX_MODE).ToInt()].push_back(book_size);
        if (s.str[7].length() > 0)
            this->isbn.insert(map<long long, int>::value_type(std::stoll(s.str[7]), this->book_size));
        this->book_size++;
    }

    this->user_size = 0;
    fs.seekg(3, ios::beg);
    while (getline(fs, line) && fs.good()) {
        StringParse user_stat(line, ",");
        //将用户信息加入map<用户名，用户信息>中
        this->user.insert(map<string, int>::value_type(user_stat.str[0], this->user_size));
        this->user_size++;
    }

    fa.seekg(3, ios::beg);
    while (getline(fa, line) && fa.good()) {
        StringParse admin_stat(line, ",");
        //将管理员信息加入map<管理员用户名，管理员信息>中
        this->admin.insert(map<string, Administrator>::value_type(admin_stat.str[0], Administrator(line)));
    }

    fk.seekg(3, ios::beg);
    while (getline(fk, line) && fk.good()) {
        keywords.push_back(line);
    }

	int book_index, borrowed_num;
	fr >> book_index;
	fr >> borrowed_num;
	while (fr.good()) {
		fr >> book_index;
		fr >> borrowed_num;
		this->borrow_stat.insert(map<int, int>::value_type(book_index, borrowed_num));
	}

    fb.close();
    fs.close();
    fa.close();
    fk.close();

    //设置为未登录状态
    currentAdmin = NULL;
    currentUser = NULL;

    currentTime = new Time;
    currentTime->l = this;
}

Library::~Library()
{
    //this->book.erase(this->book.begin());
    this->user.erase(this->user.begin());
}

Library::Library(const Library &l)
{
    this->admin = l.admin;
    this->admin_size = l.admin_size;
    this->book_size = l.book_size;
    this->currentAdmin = l.currentAdmin;
    this->currentUser = l.currentUser;
    this->isbn = l.isbn;
    for (int i = 0; i < 141; i++)
    {
        this->keyword[i] = l.keyword[i];
    }
    this->name = l.name;
    this->publisher = l.publisher;
    this->user = l.user;
    this->user_size = l.user_size;
    this->writer = l.writer;
}

int Library::Purchase(string line)
{
    if (currentAdmin == NULL)
        return NO_ACCESS;
    else {
        StringParse ss(line, ",");
        if (ss.str.size() < 2) {
			// 从文件读入
            ifstream fp(line);
            if (fp.fail()) {
                cout << "File Error" << endl;
                return FAIL;
            }
            fp.seekg(3, ios::beg);
            while (getline(fp, line) && fp.good()) {
                Book book(line, this->book_size);	// 如果信息出现问题则ISBN设为0或1
				if (book.get_ISBN() == 0 || book.get_ISBN() == 1)
					return book.get_ISBN();
                Search_Result r = Search(QString::fromWCharArray(L"ISBN编号").toStdString(), book.get_ISBN());	// 查看这本书是否在图书馆里
                if (r == FAIL) {
					// 是新书
                    StringParse s(line, ",", 5, 10, 10, 10, 15, 15);
					// 添加进各类索引
                    this->name.insert(s.str[0], this->book_size);
                    if (s.str[1].length() > 0)
                        this->name.insert(s.str[1], this->book_size);
                    if (s.str[2].length() > 0)
                        this->publisher.insert(s.str[2], this->book_size);
					if (s.str[3].length() > 0) {
						int find_pos = s.str[3].find("]") + 1;
						this->writer.insert(s.str[3].substr(find_pos, s.str[3].length() - find_pos), this->book_size);
					}
					if (s.str[4].length() > 0) {
						int find_pos = s.str[4].find("]") + 1;
						this->writer.insert(s.str[4].substr(find_pos, s.str[4].length() - find_pos), this->book_size);
					}
                    this->isbn.insert(map<long long, int>::value_type(book.get_ISBN(), this->book_size));
                    this->keyword[book.get_keyword().ToInt()].push_back(this->book_size);
                    this->book_size += 1;	// 图书总数+1
                    book.Save();
                    fstream fl(".\\data\\Log.txt", ios::app);
                    fl << currentTime->date() << QString::fromWCharArray(L",管理员 ").toStdString() << currentAdmin->user_name << QString::fromWCharArray(L" 购入新书籍《").toStdString();
                    fl << book.get_name() << QString::fromWCharArray(L"》(ISBN: ").toStdString() << book.get_ISBN() << QString::fromWCharArray(L") ").toStdString() << book.get_left_num() << QString::fromWCharArray(L"本").toStdString() << endl;
                    fl.close();
                }
                else {
					// 此书已经存在在图书馆中
                    this->currentAdmin->Purchase(r, book);	// 合并
                    fstream fl(".\\data\\Log.txt", ios::app);
                    fl << currentTime->date() << QString::fromWCharArray(L",管理员 ").toStdString() << currentAdmin->user_name << QString::fromWCharArray(L" 购入库存书籍《").toStdString();
                    fl << book.get_name() << QString::fromWCharArray(L"》(ISBN: ").toStdString() << book.get_ISBN() << QString::fromWCharArray(L") ").toStdString() << book.get_left_num() << QString::fromWCharArray(L"本").toStdString() << endl;
                    fl.close();
                }
            }
            return PURCHASE_NEW;
        }

        else {
			// 输入信息购入
            Book book(line, this->book_size);	// 如果信息出现问题则ISBN设为0或1
			if (book.get_ISBN() == 0 || book.get_ISBN() == 1)
				return book.get_ISBN();
            Search_Result r = Search(QString::fromWCharArray(L"ISBN编号").toStdString(), book.get_ISBN());	// 查看这本书是否在图书馆里
            if (r == FAIL) {
				// 是新书
                this->name.insert(book.get_name(), this->book_size);
                if (book.get_org_name().length() > 0)
                    this->name.insert(book.get_org_name(), this->book_size);
                this->publisher.insert(book.get_publisher(), this->book_size);
				if (book.get_writer().length() > 0) {
					int find_pos = book.get_writer().find("]") + 1;
					this->writer.insert(book.get_writer().substr(find_pos, book.get_writer().length() - find_pos), this->book_size);
				}
				if (book.get_translator().length() > 0) {
					int find_pos = book.get_translator().find("]") + 1;
					this->writer.insert(book.get_translator().substr(find_pos, book.get_translator().length() - find_pos), this->book_size);
				}
                //this->writer.insert(book.get_writer(), this->book_size);
                //if (book.get_translator().length() > 0)
                //    this->name.insert(book.get_translator(), this->book_size);
                this->isbn.insert(map<long long, int>::value_type(book.get_ISBN(), this->book_size));
                this->keyword[book.key_word.ToInt()].push_back(this->book_size);
                this->book_size += 1;
				book.Save();
                fstream fl(".\\data\\Log.txt", ios::in|ios::out | ios::app);
                fl << currentTime->date() << QString::fromWCharArray(L",管理员 ").toStdString() << currentAdmin->user_name << QString::fromWCharArray(L" 购入新书籍《").toStdString();
                fl << book.get_name() << QString::fromWCharArray(L"》(ISBN: ").toStdString() << book.get_ISBN() << QString::fromWCharArray(L") ").toStdString() << book.get_left_num() << QString::fromWCharArray(L"本").toStdString() << endl;
                fl.close();
                return PURCHASE_NEW;
            }
            else {
				// 此书已经存在在图书馆中
                this->currentAdmin->Purchase(r, book);
                fstream fl(".\\data\\Log.txt", ios::app);
                fl << currentTime->date() << QString::fromWCharArray(L",管理员 ").toStdString() << currentAdmin->user_name << QString::fromWCharArray(L" 购入库存书籍《").toStdString();
                fl << book.get_name() << QString::fromWCharArray(L"》(ISBN: ").toStdString() << book.get_ISBN() << QString::fromWCharArray(L") ").toStdString() << book.get_left_num() << QString::fromWCharArray(L"本").toStdString() << endl;
                fl.close();
                return PURCHASE_OLD;
            }
        }
    }
}

Search_Result Library::Search(string type, string value)
{
	if (type == QString::fromWCharArray(L"书籍名称").toStdString())
		return this->name.find(value);
	else if (type == QString::fromWCharArray(L"出版社").toStdString())
		return this->publisher.find(value);
	else if (type == QString::fromWCharArray(L"作者/译者").toStdString())
		return this->writer.find(value);
	else if (type == QString::fromWCharArray(L"标签").toStdString())
		return Search_Result(this->keyword[KeyWord(value, STRING_MODE).ToInt()]);
	else
		return Search_Result();
}

Search_Result Library::Search(string type, long long value)
{
	if (type == QString::fromWCharArray(L"ISBN编号").toStdString()) {
		map<long long, int>::iterator it = this->isbn.find(value);
		if (it == isbn.end())
			return Search_Result();
		else {
			return Search_Result(it->second);
		}
	}
	else
		return Search_Result();
}

int Library::Borrow(Book &b, int keep_time)
{
    if (this->currentUser == NULL) {
        return NO_LOGIN;
    }
    else {
        int state = currentUser->Borrow(b, keep_time);
		if (state == SUCCESS) {
			auto it = this->borrow_stat.find(b.inter_num);
			if (it == this->borrow_stat.end())
				this->borrow_stat.insert(pair<int, int>(b.inter_num, 1));
			else
				it->second += 1;

			ofstream fr(".\\data\\BorrowRank.txt");
			it = this->borrow_stat.begin();
			while (it != this->borrow_stat.end()) {
				fr << it->first << " " << it->second << endl;
				it++;
			}
			fr.close();
		}
		return state;
    }
}

int Library::Borrow(Reserved_Book &b, int keep_time)
{
    if (this->currentUser == NULL) {
        return NO_LOGIN;
    }
    else {
		int state = currentUser->Borrow(b, keep_time);
		if (state == SUCCESS) {
			auto it = this->borrow_stat.find(b.book);
			if (it == this->borrow_stat.end())
				this->borrow_stat.insert(pair<int, int>(b.book, 1));
			else
				it->second += 1;

			ofstream fr(".\\data\\BorrowRank.txt");
			it = this->borrow_stat.begin();
			while (it != this->borrow_stat.end()) {
				fr << it->first << " " << it->second << endl;
				it++;
			}
			fr.close();
		}
		return state;
    }
}

int Library::Return(Book &b)
{
    if (currentUser == NULL) {
        return NO_LOGIN;
    }
    else {
        int s = currentUser->Return(b);
        this->CheckReserve();
        return s;
    }
}

int Library::Reserve(Book &b)
{
    if (currentUser == NULL) {
        return NO_LOGIN;
    }
    else {
        int state = currentUser->Reserve(b);
        this->CheckReserve();
        return state;
    }
}

int Library::ReserveCancel(Reserved_Book &b)
{
    if (currentUser == NULL) {
        return NO_LOGIN;
    }
    else {
        int state = currentUser->ReserveCancel(b);
        this->CheckReserve();
        return state;
    }
}

int Library::Extend(Book &b, int extend_time)
{
    if (currentUser == NULL) {
        return NO_LOGIN;
    }
    else {
        return currentUser->Extend(b, extend_time);
    }
}

void Library::UserLogout()
{
    if (this->currentUser != NULL) {
        fstream fl(".\\data\\Log.txt", ios::app);
        fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << currentUser->user_name << QString::fromWCharArray(L" 登出").toStdString() << endl;
        fl.close();
        free(this->currentUser);
    }
    this->currentUser = NULL;
}

void Library::AdminLogout()
{
    if (this->currentAdmin != NULL) {
        fstream fl(".\\data\\Log.txt", ios::app);
        fl << currentTime->date() << QString::fromWCharArray(L",管理员 ").toStdString() << currentAdmin->user_name << QString::fromWCharArray(L" 登出").toStdString() << endl;
        fl.close();
        //free(this->currentAdmin);
    }

    this->currentAdmin = NULL;
}

int Library::UserLogin(string username, string password)
{
    //寻找用户名
    map<string, int>::iterator it = user.find(username);
    if (it == user.end())
    {
        return FALSE_ACCOUNT;
    }
    //验证密码
    else {
        User* u = new User(it->second);
        if (u->Check(password) == SUCCESS) {
            this->currentUser = u;
            fstream fl(".\\data\\Log.txt", ios::app);
            fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << currentUser->user_name << QString::fromWCharArray(L" 登入").toStdString() << endl;
            fl.close();
            return SUCCESS;
        }
        else {
            return FALSE_PASSWORD;
        }
    }
}

int Library::AdminLogin(string username, string password)
{
    //寻找用户名
    map<string, Administrator>::iterator it = admin.find(username);
    if (it == admin.end())
    {
        return FALSE_ACCOUNT;
    }
    //验证密码
    else {
        if (admin[username].Check(password) == SUCCESS) {
            this->currentAdmin = &admin[username];
            fstream fl(".\\data\\Log.txt", ios::app);
            fl << currentTime->date() << QString::fromWCharArray(L",管理员 ").toStdString() << currentAdmin->user_name << QString::fromWCharArray(L" 登入").toStdString() << endl;
            fl.close();
            return SUCCESS;
        }
        else {
            return FALSE_PASSWORD;
        }
    }
}

int Library::UserRegister(string line)
{
    if (currentAdmin == NULL)
        return NO_ACCESS;
    else {
        StringParse user_stat(line, ",");
        if (user_stat.str.size() < 2) {
			// 从文件注册
            ifstream fp(line);
            if (fp.fail()) {
                cout << "File Error" << endl;
                return FAIL;
            }

            fp.seekg(3, ios::beg);
            while (getline(fp, line) && fp.good()) {
                StringParse user_stat(line, ",");

				// 查看是否已经存在该用户
                if (this->user.find(user_stat.str[0]) != this->user.end())
                    continue;
                else {
                    User* u = new User(line, this->user_size);	// 信息有误则将诚信度置为0
					if (u->honesty == 0)
						return FAIL;
                    this->user.insert(map<string, int>::value_type(user_stat.str[0], this->user_size));	//添加进索引
                    u->Save();
                    this->user_size++;
                    fstream fl(".\\data\\Log.txt", ios::app);
                    fl << currentTime->date() << QString::fromWCharArray(L",用户新账号 ").toStdString() << u->user_name << QString::fromWCharArray(L" 注册").toStdString() << endl;
                    fl.close();
                }
            }
            return SUCCESS;
        }
        else {
			// 查看是否已经存在该用户
            if (this->user.find(user_stat.str[0]) != this->user.end())
                return ACCOUNT_EXIST;
            else {
                User* u = new User(line, this->user_size);	// 信息有误则将诚信度置为0
				if (u->honesty == 0)
					return FAIL;
                this->user.insert(map<string, int>::value_type(user_stat.str[0], this->user_size));	//添加进索引
                u->Save();
                this->user_size++;
                fstream fl(".\\data\\Log.txt", ios::app);
                fl << currentTime->date() << QString::fromWCharArray(L",用户新账号 ").toStdString() << u->user_name << QString::fromWCharArray(L" 注册").toStdString() << endl;
                fl.close();
                return SUCCESS;
            }
        }
    }
}

int Library::AdminRegister(string line)
{
    if (currentAdmin == NULL)
        return NO_ACCESS;
    else {
        Administrator a(line);
        if (this->admin.find(a.user_name) != this->admin.end())
            return ACCOUNT_EXIST;
        else {
            this->admin.insert(map<string, Administrator>::value_type(a.user_name, a));
            a.Save();
            fstream fl(".\\data\\Log.txt", ios::app);
            fl << currentTime->date() << QString::fromWCharArray(L",管理员新账号 ").toStdString() << currentUser->user_name << QString::fromWCharArray(L" 注册").toStdString() << endl;
            fl.close();
            return SUCCESS;
        }
    }
}

int Library::UserChangePassword(string line)
{
    if (this->currentUser == NULL || line.length() > LENGTH_STAFF_PASSWORD)
        return FAIL;
    else {
        this->currentUser->ChangePassWord(line);
        this->currentUser->Save();
        fstream fl(".\\data\\Log.txt", ios::app);
        fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << currentUser->user_name << QString::fromWCharArray(L" 修改密码").toStdString() << endl;
        fl.close();
        return SUCCESS;
    }
}

int Library::AdminChangePassword(string line)
{
    if (this->currentAdmin == NULL || line.length() > LENGTH_STAFF_PASSWORD)
        return FAIL;
    else {
        this->currentAdmin->ChangePassWord(line);
        fstream fp(".\\data\\Users.txt", ios::app);
        fp << setfill(' ') << setw(LENGTH_STAFF_NAME) << currentAdmin->user_name << ',';
        fp << setfill(' ') << setw(LENGTH_STAFF_PASSWORD) << currentAdmin->password << '\n';
        fp.close();
        fstream fl(".\\data\\Log.txt", ios::app);
        fl << currentTime->date() << QString::fromWCharArray(L",管理员 ").toStdString() << currentUser->user_name << QString::fromWCharArray(L" 修改密码").toStdString() << endl;
        fl.close();
        return SUCCESS;
    }
}

int Library::UserEdit(User &u, string line)
{
	if (currentAdmin == NULL)
		return NULL;
	User temp = u;
	int state = u.Edit(line);
	if (state == SUCCESS) {
		auto it = user.find(temp.user_name);
		this->user.erase(it);
		this->user.insert(map<string, int>::value_type(u.user_name, u.index));
		return SUCCESS;
	}
	else {
		u = temp;
		return FAIL;
	}
}

int Library::BookEdit(Book &b, string line)
{
	if (currentAdmin == NULL)
		return NO_ACCESS;
	Book temp = b;

	int state = b.Edit(line);
	if (state == FAIL) {
		b = temp;
		return FAIL;
	}
	else if (state == SUCCESS) {
		auto it = isbn.find(temp.ISBN);
		this->isbn.erase(it);
		this->writer.erase(temp.writer, temp.inter_num);
		this->writer.erase(temp.translator, temp.inter_num);
		this->publisher.erase(temp.publisher, temp.inter_num);
		this->name.erase(temp.name, temp.inter_num);
		this->name.erase(temp.org_name, temp.inter_num);

		StringParse s(line, ",", 5, 10, 10, 10, 15, 15);
		if (s.str[0].length() > 0)
			this->name.insert(s.str[0], b.inter_num);
		if (s.str[1].length() > 0)
			this->name.insert(s.str[1], b.inter_num);
		if (s.str[2].length() > 0)
			this->publisher.insert(s.str[2], b.inter_num);
		if (s.str[3].length() > 0) {
			int find_pos = s.str[3].find("]") + 1;
			this->writer.insert(s.str[3].substr(find_pos, s.str[3].length() - find_pos), b.inter_num);
		}
		if (s.str[4].length() > 0) {
			int find_pos = s.str[4].find("]") + 1;
			this->writer.insert(s.str[4].substr(find_pos, s.str[4].length() - find_pos), b.inter_num);
		}
		if (b.key_word.ToInt() != FAIL)
			this->keyword[b.key_word.ToInt()].push_back(b.inter_num);

		this->isbn.insert(map<long long, int>::value_type(b.ISBN, b.inter_num));
		return SUCCESS;
	}
	return SYN_ERROR;
}

vector<Book> Library::BorrowRank()
{
	vector<pair<int, int>> inverse(this->borrow_stat.begin(), this->borrow_stat.end());
	function<bool(pair<int, int>, pair<int, int>)>cmp;
	cmp = [](pair<int, int> a, pair<int, int> b) {
		return a.second < b.second;
	};
	sort(inverse.begin(), inverse.end(), cmp);
	vector<Book> result;
	auto it = inverse.begin();
	int i = 0;
	while (it != inverse.end() && i < BORROW_RANK) {
		result.push_back(Book(it->first));
		it++;
		i++;
	}
	return result;
}

void Library::TimeMachine()
{
	// 方便测试用的函数，回归初始状态
    if (currentAdmin == NULL)
        return;

    map<string, int>::iterator it = this->user.begin();
    while (it != this->user.end()) {
		// 依次读取所有用户
        User u(it->second);

        vector<Borrowed_Book>::iterator itt = u.borrowed_book.begin();
        while (itt != u.borrowed_book.end()) {
			// 将所有借阅书籍归还
            itt->Return();
            itt++;
        }
        u.borrowed_book.clear();
        vector<Reserved_Book>::iterator ittt = u.reserved_book.begin();
        while (ittt != u.reserved_book.end()) {
			// 取消预定所有预约书籍
            u.ReserveCancel(*(ittt++));
        }
        u.honesty = 100;
        u.Save();
        it++;
    }

    string old = currentTime->date() + "Log.txt", temp;
    ofstream fo(old);
    ifstream fi("Log.txt");
    while (fi.good()) {
        fi >> temp;
        fo << temp;
    }
    fo.close();
    fi.close();
    ofstream fn("Log.txt");
    fn.close();

    currentTime = new Time(0);	// 重设时间为0
    currentTime->Save();
}

void Library::CheckReserve()
{
	// 自动检查预约状态
    vector<Reserved_Book*> ob;
    User **user = new User*[this->user_size];
    for (int i = 0; i < this->user_size; i++) {
        if (this->currentUser != NULL && i == this->currentUser->index)
            user[i] = this->currentUser;
        else
            user[i] = new User(i);

        if (user[i]->reserved_book.size() != 0) {
            vector<Reserved_Book>::iterator it = user[i]->reserved_book.begin();
            while (it != user[i]->reserved_book.end()) {
                ob.push_back(&(*it));
                it++;
            }
        }
    }
    auto Compare = [](Reserved_Book *l, Reserved_Book *r) {
        return l->reserve_time < r->reserve_time;
    };
    sort(ob.begin(), ob.end(), Compare); // sort by order time
	// 以上为读取所有被预约的书籍并按预约时间排序

    vector<Reserved_Book*>::iterator it = ob.begin();
    while (it != ob.end() && *currentTime - (*it)->reserve_time >= RESERVE_TIME) {
        Book bb((*it)->book);
        // 删掉可借阅过期未操作的书籍
        if ((*it)->v_time != FAIL && *currentTime - (*it)->v_time > RESERVE_TIME) {
            bb.ReservingCancel();
            vector<Reserved_Book>::iterator ii = find((*it)->user->reserved_book.begin(), (*it)->user->reserved_book.end(), **it);
            vector<Reserved_Book>::iterator iii = find((*it)->user->v_reserved_book.begin(), (*it)->user->v_reserved_book.end(), **it);
            (*it)->user->v_reserved_book.erase(iii);
            (*it)->user->reserved_book.erase(ii);
            (*it)->user->Save();
        }
        it++;
    }

    it = ob.begin();
    while (it != ob.end()) {
        Book bb((*it)->book);
        // 将可以借阅的书籍为用户保留
        if ((*it)->v_time == FAIL && bb.get_v1_num() > 0) {
            bb.Reserving();
            (*it)->v_time = *currentTime;
            (*it)->user->v_reserved_book.push_back(**it);
            (*it)->user->Save();
        }
        it++;
    }
}

vector<User> Library::CurrentState()
{
	// 返回用户的信息
    vector<User> u;
    if (currentAdmin == NULL)
        return u;

    int i = 0;
    while (i < this->user_size) {
        u.push_back(User(i));
        i++;
    }
    return u;
}
