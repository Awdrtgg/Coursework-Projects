#include <stdlib.h>
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <sstream>
#include <cmath>

using namespace std;

int s;
int s2;
char a; //存储读入字符
char A; //生成式左部符号
char inpu[100]; //输入字符串
double inmap[100];  //记录每个表达式的值
double d;   //存储读入表达式
string b;   //生成式右部符号
pair<char, string> rules[10];   //表达式组成方式是char，string的pair类型
char * ip;  //指向当前读入字符
double *id; //指向当前读入字符的值
stack<pair<int, char> > stk; //分析栈
stack<pair<double, string> > st2; //存储属性的栈
map<pair<int, char>, string> lookup;    //转移表

template <class T,class U>
static T convertfromto(const T , const U &s){
     //类型转换
    T va;
    stringstream ss;
    ss << s;
    ss >> va;
    return va;
}

void initrules(){
    //生成式初始化
    rules[0] =make_pair('S', "E");
    rules[1] =make_pair('E', "E+T");
    rules[2] =make_pair('E', "E-T");
    rules[3] =make_pair('E', "T");
    rules[4] =make_pair('T', "T*F");
    rules[5] =make_pair('T', "T/F");
    rules[6] =make_pair('T', "F");
    rules[7] =make_pair('F', "(E)");
    rules[8] =make_pair('F', "n");
    rules[9] =make_pair('F', "n.n");
}

void st2push(){
    //属性栈入栈操作
    st2.push(make_pair(d, "int"));
}

void p2(){
    //属性栈出栈操作
    pair<double, string> x;

    if (A == 'E') {
        if (b == "E+T") {
            x = st2.top();
            st2.pop();
            st2.pop();
            st2.top().first += x.first;
            if (st2.top().second == "div_zero_error")
                return;
            else if(x.second != "int")
                st2.top().second = x.second;
        }else if(b == "E-T"){
            x = st2.top();
            st2.pop();
            st2.pop();
            st2.top().first -= x.first;
            if (st2.top().second == "div_zero_error")
                return;
            else if(x.second != "int")
                st2.top().second = x.second;
        }
    }
    else if (A == 'T') {
        if (b == "T*F") {
            x = st2.top();
            st2.pop();
            st2.pop();
            st2.top().first *= x.first;
            if (st2.top().second == "div_zero_error") {
                return;
            }
            if (x.second != "int") {
                st2.top().second = x.second;
            }
        }else if(b == "T/F"){
            x = st2.top();
            st2.pop();
            st2.pop();
            if(x.first ==0 || st2.top().second == "div_zero_error" ){
                st2.top().first = 0;
                st2.top().second = "div_zero_error";
            }else{
                
                st2.top().first /= x.first;
                st2.top().second = "double";
            }
        }
    }
    else if (A == 'F') {
        if (b == "(E)") {
            st2.pop();
            x = st2.top();
            st2.pop();
            st2.top().first = x.first;
            st2.top().second = x.second;
            
        }else if(b == "n.n"){
            double x = st2.top().first * pow(0.1,int(log(st2.top().first)/log(10))+1);
            st2.pop();
            st2.pop();
            st2.top().first = st2.top().first + x;
            st2.top().second = "double";
        }
    }
}

void initlookup(){
    //转移表初始化
    lookup[make_pair(0, '(')] = "S4";lookup[make_pair(0, 'n')] = "S5";lookup[make_pair(0, 'E')] = "1";lookup[make_pair(0, 'T')] = "2"; lookup[make_pair(0, 'F')] = "3";
    lookup[make_pair(1, '\0')] = "ACC";lookup[make_pair(1, '+')] = "S6";lookup[make_pair(1, '-')] = "S7";
    lookup[make_pair(2, '\0')] = "R3";lookup[make_pair(2, '+')] = "R3";lookup[make_pair(2, '-')] = "R3";lookup[make_pair(2, ')')] = "R3";lookup[make_pair(2, '*')] = "S8";lookup[make_pair(2, '/')] = "S9";
    lookup[make_pair(3, '\0')] = "R6";lookup[make_pair(3, '+')] = "R6";lookup[make_pair(3, '-')] = "R6";lookup[make_pair(3, ')')] = "R6";lookup[make_pair(3, '*')] = "R6";lookup[make_pair(3, '/')] = "R6";
    lookup[make_pair(4, '(')] = "S4";lookup[make_pair(4, 'n')] = "S5";lookup[make_pair(4, 'E')] = "10";lookup[make_pair(4, 'T')] = "2";lookup[make_pair(4, 'F')] = "3";
    lookup[make_pair(5, '\0')] = "R8";lookup[make_pair(5, '+')] = "R8";lookup[make_pair(5, '-')] = "R8";lookup[make_pair(5, ')')] = "R8";lookup[make_pair(5, '*')] = "R8";lookup[make_pair(5, '/')] = "R8";lookup[make_pair(5, '.')] = "S16";
    lookup[make_pair(6, '(')] = "S4";lookup[make_pair(6, 'n')] = "S5";lookup[make_pair(6, 'T')] = "12"; lookup[make_pair(6, 'F')] = "3";
    lookup[make_pair(7, '(')] = "S4";lookup[make_pair(7, 'n')] = "S5";lookup[make_pair(7, 'T')] = "11"; lookup[make_pair(7, 'F')] = "3";
    lookup[make_pair(8, '(')] = "S4";lookup[make_pair(8, 'n')] = "S5";lookup[make_pair(8, 'F')] = "13";
    lookup[make_pair(9, '(')] = "S4";lookup[make_pair(9, 'n')] = "S5";lookup[make_pair(9, 'F')] = "14";
    lookup[make_pair(10, ')')] = "S15";lookup[make_pair(10, '+')] = "S6";lookup[make_pair(10, '-')] = "S7";
    lookup[make_pair(11, '\0')] = "R2";lookup[make_pair(11, '+')] = "R2";lookup[make_pair(11, '-')] = "R2";lookup[make_pair(11, ')')] = "R2";lookup[make_pair(11, '*')] = "S8";lookup[make_pair(11, '/')] = "S9";
    lookup[make_pair(12, '\0')] = "R1";lookup[make_pair(12, '+')] = "R1";lookup[make_pair(12, '-')] = "R1";lookup[make_pair(12, ')')] = "R1";lookup[make_pair(12, '*')] = "S8";lookup[make_pair(12, '/')] = "S9";
    lookup[make_pair(13, '\0')] = "R4";lookup[make_pair(13, '+')] = "R4";lookup[make_pair(13, '-')] = "R4";lookup[make_pair(13, ')')] = "R4";lookup[make_pair(13, '*')] = "R4";lookup[make_pair(13, '/')] = "R4";
    lookup[make_pair(14, '\0')] = "R5";lookup[make_pair(14, '+')] = "R5";lookup[make_pair(14, '-')] = "R5";lookup[make_pair(14, ')')] = "R5";lookup[make_pair(14, '*')] = "R5";lookup[make_pair(14, '/')] = "R5";
    lookup[make_pair(15, '\0')] = "R7";lookup[make_pair(15, '+')] = "R7";lookup[make_pair(15, '-')] = "R7";lookup[make_pair(15, ')')] = "R7";lookup[make_pair(15, '*')] = "R7";lookup[make_pair(15, '/')] = "R7";
    lookup[make_pair(16, 'n')] = "S17";
    lookup[make_pair(17, '\0')] = "R9";lookup[make_pair(17, '+')] = "R9";lookup[make_pair(17, '-')] = "R9";lookup[make_pair(17, ')')] = "R9";lookup[make_pair(17, '*')] = "R9";lookup[make_pair(17, '/')] = "R9";
    
}

void forip(){
    //指针向前
    a = *(++ip);
    d = *(++id);
}

bool isshift(pair<int, char> inp){
    //判断移进
    string temp = lookup[inp];
    
    if (temp[0] == 'S'){
        s2 = atoi(temp.substr(1).c_str());
        return true;
    }
    return false;
}

bool isreduce(pair<int, char> inp){
    //判断规约
    string temp = lookup[inp];
    
    int num;
    if (temp[0] == 'R'){
        num = atoi(temp.substr(1).c_str());
        A = rules[num].first;
        b = rules[num].second;
        return true;
    }
    return false;
}

bool isacc(pair<int, char> inp){
    //判断结束
    string temp = lookup[inp];

    if (temp[0] == 'A'){
        return true;
    }
    return false;
}

void analyze(char *input){
    //分析
    initrules();
    initlookup();

    ip = input;
    id = inmap;
    a = *(input);
    d = *(inmap);
    stk.push(make_pair(0, '-'));

    s = 0;
    do{
        s = stk.top().first;
        if (isshift(make_pair(s, a))) {
            stk.push(make_pair(s2, a));
            st2push();
            forip();
        }else if(isreduce(make_pair(s, a))){
            p2();
            for(int i = 0; i < b.length();i++){
                stk.pop();
            }
            s2 = stk.top().first;
            stk.push(make_pair(atoi(lookup[make_pair(s2, A)].c_str()), A));
            cout<<A<<"->"<<b<<endl;
            cout<<"类型为"<<st2.top().second<<endl;
            if(st2.top().second != "div_zero_error")
                cout<<"表达式的值为"<<st2.top().first<<endl;
            cout<<endl;
        }else if(isacc(make_pair(s, a)))
            break;
        else{
            cout<<"don't accept!"<<endl;
            exit(1);
        }
    } while(true);
}

void trans(){
    //输入字符串转换(相当于词法分析)
    int st = 0;
    int i = 0;
    double temp = 0;

    for (int j = 0;; j++) {
        if (st == 0) {
            if (inpu[j] <= '9' && inpu[j] >= '0') {
                st = 1;
                temp = convertfromto(0.1, inpu[j]);
                inpu[i++] = 'n';
                
            }
            else
                inpu[i++] = inpu[j];
        }else if (inpu[j] > '9' || inpu[j] < '0'){
            st = 0;
            inmap[i-1] = temp;
            inpu[i++] = inpu[j];
        }else
            temp = temp*10 + convertfromto(0.1, inpu[j]);
        if (inpu[j] == '\0') {
            break;
        }
    }
}

int main(int argc, const char * argv[]) {
    cout << "请输入一个语句：" << endl;
    stk.push(make_pair(0, '-'));
    stk.pop();
    cin>>inpu;
    trans();
    analyze(inpu);
    return 0;
}


