#include "library.h"

//----------------------- StringParse --------------------------------

StringParse::StringParse(string s, string delim, int len_limit, ...)
{
    // split the string s by delim to this->str

    va_list arg_ptr;
    va_start(arg_ptr, len_limit);


    size_t pos = 0;
    size_t len = s.length();
    size_t delim_len = delim.length();
    if (delim_len == 0)
        return;
    while (pos < len) {
        int find_pos = s.find(delim, pos);
        if (find_pos < 0) {
            this->str.push_back(s.substr(pos, len - pos));
            break;
        }
        this->str.push_back(s.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }

    vector<string>::iterator it = this->str.begin();
    while (it != str.end()) {
        it->erase(0, it->find_first_not_of(' '));
        if (len_limit != 0) {
            int i = va_arg(arg_ptr, int);
            len_limit -= 1;
            if (it->length() > i)
                *it = it->substr(0, i);
        }
        it++;
    }
    va_end(arg_ptr);
}

StringParse::~StringParse()
{
    this->str.clear();
}

StringParse::StringParse(const StringParse &s)
{
    this->str = s.str;
}

//----------------------- KeyTree --------------------------------

KeyTree::KeyTree(string key, int value)
{
    this->result.push_back(value);
    if (key.length() < 1) {
        return;
    }
    else {
        char c = key[0];
        key = key.erase(0, 1); //删除字符串key的首字符
        this->children.insert(map<char, KeyTree>::value_type(c, KeyTree(key, value)));
    }
}

KeyTree::~KeyTree()
{
    this->result.clear();
    this->children.clear();
}

KeyTree::KeyTree(const KeyTree &k)
{
    this->children = k.children;
    this->result = k.result;
}

void KeyTree::insert(string key, int value)
{
    vector<int>::iterator r = std::find(this->result.begin(), this->result.end(), value);
    if (r == this->result.end())
        this->result.push_back(value);

    if (key.length() < 1) {
        return;
    }
    else {
        char c = key[0];
        key = key.erase(0, 1);
        map<char, KeyTree>::iterator it = this->children.find(c);
        if (it == this->children.end()) {
            this->children.insert(map<char, KeyTree>::value_type(c, KeyTree(key, value)));
        }
        else {
            this->children[c].insert(key, value);
        }
    }
}

void KeyTree::erase(string key, int value)
{
	vector<int>::iterator r = std::find(this->result.begin(), this->result.end(), value);
	if (r != this->result.end())
		this->result.erase(r);

	if (key.length() < 1) {
		return;
	}
	else {
		char c = key[0];
		key = key.erase(0, 1);
		map<char, KeyTree>::iterator it = this->children.find(c);
		if (it != this->children.end()) 
			this->children[c].erase(key, value);
	}
}

Search_Result KeyTree::find(string key)
{
    if (key.length() < 1) {
        return Search_Result(this->result);
    }
    else {
        char c = key[0];
        key = key.erase(0, 1);
        map<char, KeyTree>::iterator it = this->children.find(c);
        if (it == this->children.end()) {
            return Search_Result(this->result);
        }
        else {
            return this->children[c].find(key);
        }
    }
}

//----------------------- SearchResult --------------------------------

Search_Result::Search_Result()
{
    this->current_page = 0;
}

Search_Result::Search_Result(vector<int> b)
{
    this->book = b;
    for (int i = 0;i < MAX_PER_PAGE && i < this->book.size(); i++)
    {
        Book* bb = new Book(book[i]);
        this->current_book.push_back(bb);
    }

    this->current_page = 1;
}

Search_Result::Search_Result(int b)
{
    this->book.push_back(b);
    Book* bb = new Book(book[0]);
    this->current_book.push_back(bb);
    this->current_page = 1;
}

Search_Result::~Search_Result()
{
    this->book.clear();
    this->current_book.clear();
}

Search_Result::Search_Result(const Search_Result &s)
{
    this->book = s.book;
    this->current_book = s.current_book;
    this->current_page = s.current_page;
}

int Search_Result::NextPage()
{
    if (this->book.size() == 0)
        return FAIL;
    if (current_page * MAX_PER_PAGE + 1 <= book.size())
    {
        current_book.clear();
        current_page += 1;
        for (int i = (current_page - 1) * MAX_PER_PAGE; i < current_page * MAX_PER_PAGE && i < book.size(); i++)
        {
            Book* bb = new Book(book[i]);
            current_book.push_back(bb);
        }
        return SUCCESS;
    }
    else
        return FAIL;
}

int Search_Result::FirstPage()
{
    if (this->book.size() == 0)
        return FAIL;
    if (this->current_page != 1) {
        this->current_book.clear();
        for (int i = 0; i < MAX_PER_PAGE && i < this->book.size(); i++)
        {
            Book* bb = new Book(book[i]);
            this->current_book.push_back(bb);
        }
        this->current_page = 1;
        return SUCCESS;
    }
    else
        return SUCCESS;
}

int Search_Result::PreviousPage()
{
    if (this->book.size() == 0 || this->current_page == 1)
        return FAIL;

    current_page -= 1;
    this->current_book.clear();
    for (int i = (current_page-1) * MAX_PER_PAGE; i < current_page * MAX_PER_PAGE && i < book.size(); i++)
    {
        Book* bb = new Book(book[i]);
        current_book.push_back(bb);
    }
    return SUCCESS;
}

int Search_Result::LastPage()
{
    if (this->book.size() == 0)
        return FAIL;

    int last_page;
    if (this->book.size() % MAX_PER_PAGE == 0)
        last_page = this->book.size() / MAX_PER_PAGE;
    else
        last_page = this->book.size() / MAX_PER_PAGE + 1;

    if (current_page != last_page) {
        current_page = last_page;
        this->current_book.clear();
        for (int i = (current_page - 1) * MAX_PER_PAGE; i < current_page * MAX_PER_PAGE && i < book.size(); i++)
        {
            Book* bb = new Book(book[i]);
            current_book.push_back(bb);
        }
    }
    return SUCCESS;
}

int Search_Result::JumpPage(int page)
{
    if (this->book.size() == 0 || (page-1)*MAX_PER_PAGE + 1 > book.size())
        return FAIL;
    if (current_page != page) {
        this->current_book.clear();
        this->current_page = page;
        for (int i = (current_page - 1) * MAX_PER_PAGE; i < current_page * MAX_PER_PAGE && i < book.size(); i++)
        {
            Book* bb = new Book(book[i]);
            current_book.push_back(bb);
        }
        return SUCCESS;
    }
    else
        return SUCCESS;
}

void Search_Result::Fresh()
{
    this->current_book.clear();
    for (int i = (current_page - 1) * MAX_PER_PAGE; i < current_page * MAX_PER_PAGE && i < book.size(); i++)
    {
        Book* bb = new Book(book[i]);
        current_book.push_back(bb);
    }
}

Search_Result::operator int()
{
    if (this->book.size() == 0)
        return FAIL;
    else
        return this->book.size();
}

//----------------------- KeyWord --------------------------------

KeyWord::KeyWord()
{
	for (int i = 0; i < MAX_KEYWORD_NUM; i++)
		this->xx[i] = 0;
}

KeyWord::KeyWord(string keyword, int mode)
{
    if (mode == HEX_MODE) {
        for (int i = 0; i < MAX_KEYWORD_NUM; i++)
        {
            if (keyword.length() >= 16) {
                this->xx[i] = stoull(keyword.substr(keyword.length() - 16, keyword.length() - 1), nullptr, 16);
                keyword.erase(keyword.end() - 16, keyword.end());
            }
            else if (keyword.length() > 0) {
                this->xx[i] = stoull(keyword, nullptr, 16);
                keyword.clear();
            }
            else
                this->xx[i] = 0;
        }
    }
    else if (mode == STRING_MODE) {
        for (int i = 0; i < MAX_KEYWORD_NUM; i++)
        {
            unsigned long long x = 0x1;
            this->xx[i] = 0x0;
            for (int j = 0; j < 64 && (64*i+j) < keywords.size(); j++, x = x << 1)
            {
                if (keyword == keywords[64 * i + j]) {
                    this->xx[i] = x;
                    break;
                }
            }
        }

    }

}

KeyWord::~KeyWord()
{
    // empty
}

KeyWord::KeyWord(const KeyWord &k)
{
    for (int i = 0; i < MAX_KEYWORD_NUM; i++)
    {
        this->xx[i] = k.xx[i];
    }
}

string KeyWord::ToFile()
{
    stringstream s;
    for (int i = MAX_KEYWORD_NUM - 1; i >= 0; i--)
    {
        s << setfill('0') << setw(16) << hex << this->xx[i];
    }
    return s.str();
}

int KeyWord::ToInt()
{
    for (int i = 0; i < MAX_KEYWORD_NUM; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            if ((this->xx[i] >> j) & 1 == 1) {
                return (64 * i + j);
            }
        }
    }
    return FAIL;
}

KeyWord::operator string()
{
    string s = "";

    for (int i = 0; i < MAX_KEYWORD_NUM; i++)
    {
        for (int j = 0; j < 64 && (64*i+j) < keywords.size(); j++)
        {
			if ((this->xx[i] >> j) & 1 == 1)
				s += keywords[64 * i + j] + ";";
        }
    }
    if (s.length() > 0)
        s.pop_back(); // delete the last ';'
    return s;
}

KeyWord KeyWord::operator+=(KeyWord &k)
{
    for (int i = 0; i < MAX_KEYWORD_NUM; i++)
    {
        this->xx[i] = this->xx[i] | k.xx[i];
    }
    return *this;
}
