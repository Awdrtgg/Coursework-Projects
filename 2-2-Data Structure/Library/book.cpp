#include "library.h"
#include <QStringMatcher>
//----------------------- Book --------------------------------

Book::Book(int index)
{

    fstream fp(".\\data\\Books.txt", ios::in | ios::out);
    fp.seekg(index * LENGTH_BOOK_LINE + 3, ios::beg);
    string line;
    //fp >> setw(LENGTH_BOOK_LINE) >> line;
    getline(fp, line);

    StringParse s(line, ",");
    int j = -1;
    this->name = s.str[++j];
    this->org_name = s.str[++j];
    this->publisher = s.str[++j];
    this->writer = s.str[++j];
    this->translator = s.str[++j];
    this->rating = std::stof(s.str[++j]);
    this->key_word = KeyWord(s.str[++j], HEX_MODE);
    this->ISBN = std::stoull(s.str[++j]);
    this->page_num = std::stoi(s.str[++j]);
    this->reserve_num = std::stoi(s.str[++j]);
    this->reserving_num = std::stoi(s.str[++j]);
    this->left_num = std::stoi(s.str[++j]);
    this->total_num = std::stoi(s.str[++j]);
    this->book_left = std::stoull(s.str[++j], nullptr, 16);
    this->publish_time = s.str[++j];
    this->purchase_time = std::stoi(s.str[++j]);
    this->img_url = s.str[++j];
    this->intro = s.str[++j];
    vector<string>::iterator it = s.str.begin() + (++j);
    while (it != s.str.end()) {
        this->intro = this->intro + ',' + *it;
        it++;
    }
    this->inter_num = index;

    fp.close();
}

Book::Book(string line, int index)
{
	try {
		StringParse s(line, ",");
		if (s.str[0].length() <= LENGTH_BOOK_NAME)
			this->name = s.str[0];
		else
			this->name = s.str[0].substr(0, LENGTH_BOOK_NAME);
		if (s.str[1].length() <= LENGTH_BOOK_NAME)
			this->org_name = s.str[1];
		else
			this->org_name = s.str[1].substr(0, LENGTH_BOOK_NAME);
		if (s.str[2].length() <= LENGTH_BOOK_PUBLISHER)
			this->publisher = s.str[2];
		else
			this->publisher = s.str[2].substr(0, LENGTH_BOOK_PUBLISHER);
		if (s.str[3].length() <= LENGTH_BOOK_WRITER)
			this->writer = s.str[3];
		else
			this->writer = s.str[3].substr(0, LENGTH_BOOK_WRITER);
		if (s.str[4].length() <= LENGTH_BOOK_WRITER)
			this->translator = s.str[4];
		else
			this->translator = s.str[4].substr(0, LENGTH_BOOK_WRITER);
		if (s.str[5].length() == 0)
			this->rating = 0.0;
		else {
			this->rating = std::stof(s.str[5]);
		}

		StringParse k(s.str[6], ";");
		this->key_word = KeyWord();
		auto kk = k.str.begin();
		while (kk != k.str.end()) {
			this->key_word += KeyWord(*kk, STRING_MODE);
			kk++;
		}

		//this->key_word = KeyWord(s.str[6], STRING_MODE);
		this->ISBN = std::stoull(s.str[7]);

		if (s.str[8].length() == 0)
			this->page_num = 0;
		else
			this->page_num = std::stoi(s.str[8]);

		this->reserve_num = 0;
		this->reserving_num = 0;
		if (s.str[9].length() <= LENGTH_NUM) {
			this->left_num = std::stoi(s.str[9]);
			this->total_num = std::stoi(s.str[9]);
			if (this->left_num > MAX_BOOK_NUM) {
				this->total_num = MAX_BOOK_NUM;
				this->left_num = MAX_BOOK_NUM;
			}
		}
		else {
			this->left_num = 0;
			this->total_num = 0;
		}

		this->book_left = 0x0;
		for (int i = 0; i < this->total_num; i++) {
			this->book_left += (0x1 << i);
		}

		if (s.str[10].length() <= LENGTH_BOOK_TIME)
			this->publish_time = s.str[10];
		else
			this->publish_time = s.str[10].substr(0, LENGTH_BOOK_TIME);

		this->purchase_time = *currentTime;
		if (s.str[11].length() <= LENGTH_BOOK_IMG)
			this->img_url = s.str[11];
		else if (s.str[11].find(".jpg") == string::npos)
			this->img_url = "";
		else
			this->img_url = "";

		this->intro = s.str[12];
		vector<string>::iterator it = s.str.begin() + 13;
		while (it != s.str.end()) {
			this->intro = this->intro + ',' + *it;
			it++;
		}

		if (this->intro.length() > LENGTH_BOOK_INTRO) {
			this->intro = this->intro.substr(0, LENGTH_BOOK_INTRO);
		}


		this->inter_num = index;
	}
	catch (std::out_of_range &o) {
		this->ISBN = 1;
	}
	catch (invalid_argument &a) {
		this->ISBN = 0;
	}
    
}

Book::~Book()
{
    // nothing?
}

Book::Book(const Book &b)
{
    this->img_url = b.img_url;
    this->inter_num = b.inter_num;
    this->intro = b.intro;
    this->ISBN = b.ISBN;
    this->key_word = b.key_word;
    this->left_num = b.left_num;
    this->name = b.name;
    this->reserve_num = b.reserve_num;
    this->reserving_num = b.reserving_num;
    this->org_name = b.org_name;
    this->page_num = b.page_num;
    this->publisher = b.publisher;
    this->publish_time = b.publish_time;
    this->purchase_time = b.purchase_time;
    this->rating = b.rating;
    this->total_num = b.total_num;
    this->book_left = b.book_left;
    this->translator = b.translator;
    this->writer = b.writer;
}

bool Book::operator==(Book &b)
{
    return ISBN == b.ISBN;
}

Book Book::operator+=(Book &b)
{
    if (this->name.length() == 0)
        this->name = b.name;
    if (this->org_name.length() == 0)
        this->org_name = b.org_name;
    if (this->publisher.length() == 0)
        this->publisher = b.publisher;
    if (this->writer.length() == 0)
        this->writer = b.writer;
    if (this->translator.length() == 0)
        this->translator = b.translator;
    if (this->rating == 0.0)
        this->rating = b.rating;
    this->key_word += b.key_word;
    if (this->page_num == 0)
        this->page_num = b.page_num;

    if (this->total_num + b.total_num > 64) {
        for (int i = this->total_num; i < 64; i++)
            this->book_left += (0x1 << i);
        this->left_num += (64 - this->total_num);
        this->total_num = 64;
    }
    else {
        for (int i = this->total_num; i < this->total_num + b.total_num; i++)
            this->book_left += (0x1 << i);
        this->left_num += b.left_num;
        this->total_num += b.total_num;
    }

    if (this->publish_time.length() == 0)
        this->publish_time = b.publish_time;
    if (this->img_url.length() == 0)
        this->img_url = b.img_url;
    if (this->intro.length() == 0)
        this->intro = b.intro;
    return *this;
}

void Book::Save()
{
    fstream fp(".\\data\\Books.txt", ios::in | ios::out);
    fp.seekg(this->inter_num * LENGTH_BOOK_LINE + 3, ios::beg);

    cout << fp.tellg() << endl;

    fp << setfill(' ') << setw(LENGTH_BOOK_NAME) << this->name << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_NAME) << this->org_name << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_PUBLISHER) << this->publisher << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_WRITER) << this->writer << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_WRITER) << this->translator << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_RATING) << to_string(this->rating).substr(0, 3) << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_KEY_WORD) << this->key_word.ToFile() << ',';
    fp << setfill('0') << setw(LENGTH_BOOK_ISBN) << to_string(this->ISBN) << ',';
    fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->page_num) << ',';
    fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->reserve_num) << ',';
    fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->reserving_num) << ',';
    fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->left_num) << ',';
    fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->total_num) << ',';
    fp << setfill('0') << setw(16) << hex << this->book_left;
    fp << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_TIME) << this->publish_time << ',';
    fp << setfill(' ') << setw(LENGTH_NUM) << to_string(this->purchase_time) << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_IMG) << this->img_url << ',';
    fp << setfill(' ') << setw(LENGTH_BOOK_INTRO) << this->intro << endl;

    cout << fp.tellg() << endl;
    fp.close();
}

void Book::Display()
{
    // lazy...
}

int Book::Borrow()
{
    if (this->left_num == 0)
        return BOOK_NOT_LEFT;
    else {
        this->left_num -= 1;
        int i = 63;
        while (i >= 0) {
            if ((this->book_left >> i) & 1 == 1)
                break;
            i--;
        }
        this->book_left -= (0x1 << i);
        this->Save();
        return i;
    }
}

int Book::Return(int book_num)
{
    this->left_num += 1;
    this->book_left += (0x1 << book_num);
    this->Save();
    return SUCCESS;
}

int Book::Reserve()
{
    if (this->left_num - this->reserve_num > 0)
        return FAIL;
    this->reserve_num += 1;
    this->Save();
    return SUCCESS;
}

int Book::ReserveCancel()
{
    this->reserve_num -= 1;
    this->Save();
    return SUCCESS;
}

int Book::Reserving()
{
    if (this->left_num - this->reserving_num <= 0)
        return FAIL;
    this->reserving_num += 1;
    this->Save();
    return SUCCESS;
}

int Book::ReservingCancel()
{
    this->reserving_num -= 1;
    this->reserve_num -= 1;
    this->Save();
    return SUCCESS;
}

int Book::CheckReserve()
{
    return this->reserve_num;
}

int Book::Edit(string line)
{
	try {
		StringParse s(line, ",");
		if (s.str[0].length() <= LENGTH_BOOK_NAME)
			this->name = s.str[0];
		else
			this->name = s.str[0].substr(0, LENGTH_BOOK_NAME);
		if (s.str[1].length() <= LENGTH_BOOK_NAME)
			this->org_name = s.str[1];
		else
			this->org_name = s.str[1].substr(0, LENGTH_BOOK_NAME);
		if (s.str[2].length() <= LENGTH_BOOK_PUBLISHER)
			this->publisher = s.str[2];
		else
			this->publisher = s.str[2].substr(0, LENGTH_BOOK_PUBLISHER);
		if (s.str[3].length() <= LENGTH_BOOK_WRITER)
			this->writer = s.str[3];
		else
			this->writer = s.str[3].substr(0, LENGTH_BOOK_WRITER);
		if (s.str[4].length() <= LENGTH_BOOK_WRITER)
			this->translator = s.str[4];
		else
			this->translator = s.str[4].substr(0, LENGTH_BOOK_WRITER);
		if (s.str[5].length() == 0)
			this->rating = 0.0;
		else {
			this->rating = std::stof(s.str[5]);
		}

		StringParse k(s.str[6], ";");
		this->key_word = KeyWord();
		auto kk = k.str.begin();
		while (kk != k.str.end()) {
			this->key_word += KeyWord(*kk, STRING_MODE);
			kk++;
		}

		this->ISBN = std::stoull(s.str[7]);

		if (s.str[8].length() == 0)
			this->page_num = 0;
		else
			this->page_num = std::stoi(s.str[8]);

		if (s.str[9].length() <= LENGTH_BOOK_TIME)
			this->publish_time = s.str[9];
		else
			this->publish_time = s.str[9].substr(0, LENGTH_BOOK_TIME);

		if (s.str[10].length() <= LENGTH_BOOK_IMG)
			this->img_url = s.str[10];
		else if (s.str[10].find(".jpg") == string::npos)
			this->img_url = "";
		else
			this->img_url = "";

		this->intro = s.str[11];
		vector<string>::iterator it = s.str.begin() + 12;
		while (it != s.str.end()) {
			this->intro = this->intro + ',' + *it;
			it++;
		}

		if (this->intro.length() > LENGTH_BOOK_INTRO) {
			this->intro = this->intro.substr(0, LENGTH_BOOK_INTRO);
		}
		this->Save();
		return SUCCESS;
	}
	catch (std::out_of_range &o) {
		return FAIL;
	}
	catch (invalid_argument &a) {
		return FAIL;
	}

}

/*
int Book::Purchase(int num)
{
    if (this->total_num + num > 64)
        return BOOK_OVER_LIMIT;

    this->purchase_time = *currentTime;
    this->left_num += num;
    this->total_num += num;
    this->Save();
    return SUCCESS;
}
*/

//----------------------- Borrowed --------------------------------

Borrowed_Book::Borrowed_Book(Book &b, int keep_time)
{
    this->book_num = b.Borrow();
    this->book = new Book(b.get_index());
    this->start_time = *currentTime;
    this->keep_time = keep_time;
}

Borrowed_Book::Borrowed_Book(int book_index, int book_num, int start_time, int keep_time)
{
    this->book = new Book(book_index);
    this->book_num = book_num;
    this->start_time = start_time;
    this->keep_time = keep_time;
}

Borrowed_Book::~Borrowed_Book()
{
    // seems nothing here
}

Borrowed_Book::Borrowed_Book(const Borrowed_Book &b)
{
    this->book = b.book;
    this->keep_time = b.keep_time;
    this->start_time = b.start_time;
    this->book_num = b.book_num;
}

int Borrowed_Book::Return()
{
	this->book = new Book(this->book->get_index());
    return this->book->Return(this->book_num);
}

int Borrowed_Book::Extend(int extend_time)
{
    this->keep_time += extend_time;
    return SUCCESS;
}

int Borrowed_Book::OverDue()
{
    int diff = *currentTime - this->start_time;
    if (this->keep_time < diff)
        return diff - this->keep_time;
    else
        return SUCCESS;
}

//------------------------ Reserved ---------------------------------

Reserved_Book::Reserved_Book()
{
    this->book = FAIL;
    this->user = NULL;
    this->reserve_time = -1;
    this->v_time = -1;
}

Reserved_Book::Reserved_Book(int index, int time, int v_time, User &u)
{
    this->book = index;
    this->reserve_time = Time(time);
    this->v_time = Time(v_time);
    this->user = &u;
}

Reserved_Book::Reserved_Book(Book &b, User &u)
{
    this->reserve_time = *currentTime;
    this->v_time = -1;
    this->book = b.inter_num;
    this->user = &u;
}

Reserved_Book::~Reserved_Book()
{
    // empty
}

Reserved_Book::Reserved_Book(const Reserved_Book &b)
{
    this->book = b.book;
    this->reserve_time = b.reserve_time;
    this->v_time = b.v_time;
    this->user = b.user;
}

int Reserved_Book::Cancel()
{
    /*
    struct Compare : binary_function<Reserved_Book*, Reserved_Book*, bool>
    {
        bool operator()(Reserved_Book* p1, Reserved_Book *p2) const {
            return p1->reserve_time == p2->reserve_time;
        }
    };
    */

    Book* bp = new Book(this->book);
    bp->ReserveCancel();
    fstream fl(".\\data\\Log.txt", ios::app);
    fl << currentTime->date() << QString::fromWCharArray(L",用户 ").toStdString() << this->user->user_name << QString::fromWCharArray(L" 预约书籍《").toStdString();
    fl << bp->get_name() << QString::fromWCharArray(L"》,(ISBN: ").toStdString() << bp->get_ISBN() << QString::fromWCharArray(L")逾期，系统自动取消").toStdString() << endl;
    fl.close();
    return SUCCESS;
}

Reserved_Book::operator Book()
{
    return Book(this->book);
}

Reserved_Book::operator int()
{
    if (this->book == FAIL)
        return FAIL;
    else
        return SUCCESS;
}

bool Reserved_Book::operator==(const Reserved_Book &b)
{
    return (this->book == b.book && this->reserve_time == b.reserve_time && *(this->user) == *(b.user));
}

bool Reserved_Book::operator<(const Reserved_Book &b)
{
    return this->reserve_time < b.reserve_time;
}

