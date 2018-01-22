// Algorithm4.cpp : Designed by Xiao Yunming.
//

#include "stdafx.h" // VS projects head file

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <functional>
#include <time.h>

#define EARTH_RADIUS 6378.137
#define M_PI 3.14159265358979323846

using namespace std;

// ------------------- GREEDY_MIN_WERIGHT_TRAIAN ---------------------------

class BaseStation
{
public:
	BaseStation(int enodebid, float longitude, float latitude);
	BaseStation(string line);
	BaseStation(const BaseStation &b);

	float longitude, latitude;
	int enodebid;
};

BaseStation::BaseStation(int enodebid, float longitude, float latitude)
{
	this->enodebid = enodebid;
	this->longitude = longitude;
	this->latitude = latitude;
}

BaseStation::BaseStation(string line)
{
	size_t pos = 0;
	size_t len = line.length();
	size_t delim_len = 1;
	// to devide the line by ','
	vector<string> s;
	while (pos < len) {
		int find_pos = line.find(',', pos);
		if (find_pos < 0) {
			s.push_back(line.substr(pos, len - pos));
			break;
		}
		s.push_back(line.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}

	this->enodebid = stoi(s[0]);
	this->longitude = stod(s[1]);
	this->latitude = stod(s[2]);
}

BaseStation::BaseStation(const BaseStation &b)
{
	this->enodebid = b.enodebid;
	this->longitude = b.longitude;
	this->latitude = b.latitude;
}

double GetDistance(BaseStation A, BaseStation B)
{
	auto rad = [](const double& f) {return f * M_PI / 180.0; };
	double radLatA = rad(A.latitude), radLatB = rad(B.latitude);
	double radLonA = rad(A.longitude), radLonB = rad(B.longitude);
	double s = 1000 * EARTH_RADIUS * acos(cos(radLatA) * cos(radLatB) * cos(radLonA - radLonB) + sin(radLatA) * sin(radLatB));
	return s;
}

double Weight(vector<BaseStation> B, int a, int b, int c)
{
	double ab = GetDistance(B[a], B[b]);
	double ac = GetDistance(B[a], B[c]);
	double bc = GetDistance(B[b], B[c]);
	return ab + ac + bc;
}

void GreedyMinWerightTriangulation(double **t, int **s, vector<BaseStation> B, double &total_length)
{
	int n = B.size();
	for (int i = 0; i < n; i++)
		t[i][i] = 0;

	function<int(vector<BaseStation>&, double&)>min;
	min = [](vector<BaseStation>& b, double &total_length)
	{
		int max = b.size();
		int min_weight = Weight(b, 0, 1, 2);
		int prop = 1;
		for (int i = 1; i < max; i++) {
			int new_weight = Weight(b, i, (i + 1) % max, (i + 2) % max);
			if (new_weight < min_weight) {
				prop = (i + 1) % max;
				min_weight = new_weight;
			}
		}
		total_length += min_weight;
		return prop % max;
	};

	vector<BaseStation> S = B;
	vector<int> alive;
	for (int i = 0; i < n; i++)
		alive.push_back(1);

	while (S.size() >= 3){
		int x = min(S, total_length), y = x; // x is the relative position of the target point
							   // where y is the absolute position
		for (int j = 0; j <= y; j++)
			if (alive[j] == 0)
				y++;
		vector<BaseStation>::iterator ss = S.begin() + x;
		S.erase(ss);

		alive[y] = 0;
		int begin = y, end = y;	// begin and end are both the absolute position starting from 0
		while (alive[begin] == 0) {
			begin = (begin - 1) % n;
			while (begin < 0)
				begin += n; // to make sure that begin is a positive number
		}
		while (alive[end] == 0)
			end = (end + 1) % n;

		int first, second, third;
		first = (x - 1) % (int)S.size();
		while (first < 0)
			first += S.size();
		second = (first + 1) % (int)S.size();
		third = (first + 2) % (int)S.size();
		t[begin][end] = Weight(S, first, second, third);
		s[begin][end] = y;
	}
}

// -------------------------- HUFFMAN_TREE ----------------------------------

#define LEAF 1
#define INTER_NODE 0

class HuffmanTree
{
public:
	double weight;
	HuffmanTree *left, *right;
	char child;

	HuffmanTree() { left = NULL; right = NULL; weight = 0.0; child = '\0'; };
	HuffmanTree(HuffmanTree left, HuffmanTree right, double weight);
	HuffmanTree(char child, double weight);
	HuffmanTree(const HuffmanTree &t);

	bool operator<(HuffmanTree &t) { return this->weight < t.weight; };
	operator int() { return left == NULL ? LEAF : INTER_NODE; };

	HuffmanTree(string s);	// the algorithm realization

	void Show(ofstream &fr);
	void ShowCode(ofstream &fr, string code);
	void HuffmanCode(map<char, string> &m, string code);
};

HuffmanTree::HuffmanTree(HuffmanTree left, HuffmanTree right, double weight)
{
	this->left = new HuffmanTree(left);
	this->right = new HuffmanTree(right);
	this->weight = weight;
	this->child = '\0';
}

HuffmanTree::HuffmanTree(char child, double weight)
{
	this->left = NULL;
	this->right = NULL;
	this->child = child;
	this->weight = weight;
}

HuffmanTree::HuffmanTree(const HuffmanTree &t)
{
	this->left = t.left == NULL ? NULL : new HuffmanTree(*(t.left));
	this->right = t.right == NULL ? NULL : new HuffmanTree(*(t.right));
	this->weight = t.weight;
	this->child = t.child;
}

// the algorithm realization
HuffmanTree::HuffmanTree(string s)
{
	map<char, int>stat;
	for (int i = 0; i < s.length(); i++) {
		auto it = stat.find(s[i]);
		if (it == stat.end())
			stat.insert(map<char, int>::value_type(s[i], 1));
		else
			it->second += 1;
	}
	vector<HuffmanTree> result;
	auto it = stat.begin();
	while (it != stat.end()) {
		result.push_back(HuffmanTree(it->first, it->second));
		it++;
	}
	sort(result.begin(), result.end());

	while (result.size() > 1) {
		HuffmanTree N(result[0], result[1], result[0].weight + result[1].weight);
		result.erase(result.begin(), result.begin() + 2);
		result.push_back(N);
		sort(result.begin(), result.end());
	}

	*this = result[0];
}

void HuffmanTree::Show(ofstream &fr)
{
	if ((int)*this == INTER_NODE) {
		this->left->Show(fr);
		this->right->Show(fr);
	}
	else {
		fr << this->child << ":" << this->weight << endl;
	}
}

void HuffmanTree::ShowCode(ofstream &fr, string code)
{
	if ((int)*this == INTER_NODE) {
		this->left->ShowCode(fr, code + "0");
		this->right->ShowCode(fr, code + "1");
	}
	else {
		fr << this->child << ": weight = " << this->weight << ", code = " << code << endl;
	}
}

void HuffmanTree::HuffmanCode(map<char, string> &m, string code)
{
	if ((int)*this == INTER_NODE) {
		this->left->HuffmanCode(m, code + "0");
		this->right->HuffmanCode(m, code + "1");
	}
	else {
		m.insert(map<char, string>::value_type(this->child, code));
	}
}

// ----------------------------- GRAPH -------------------------------------

#define INF -1
#define FAIL -1

void Dijkstra(int v, vector<vector<double>> a, vector<double> &dist, vector<int> &prev)
{
	int n = a.size();
	if (v < 0 || v >= n)
		return;
	bool *s = new bool[n];

	dist.clear();
	prev.clear();
	for (int i = 0; i < n; i++) {
		dist.push_back(a[v][i]);
		s[i] = false;
		if (dist[i] == INF)
			prev.push_back(FAIL);
		else
			prev.push_back(v);
	}
	dist[v] = 0;
	s[v] = true;

	for (int i = 0; i < n; i++) {
		double temp = INF;
		int u = v;
		for (int j = 0; j < n; j++) {
			bool judge = (dist[j] > 0) && ((temp == INF) || (temp != INF && dist[j] < temp)); // that dist[j] is smaller than temp and the infinity
			if ((s[j] == false) && (judge == true)) {
				u = j;
				temp = dist[j];
			}
		}
		s[u] = true;

		for (int j = 0; j < n; j++) {
			if ((s[j] == false) && (a[u][j] != INF)) {
				double new_dist = dist[u] + a[u][j];
				if ((dist[j] != INF && new_dist < dist[j]) || dist[j] == INF) {
					dist[j] = new_dist;
					prev[j] = u;
				}
			}
		}
	}
}

void Prim(vector<vector<double>> c, vector<pair<int, int>> &result)
{
	int n = c.size();
	double *low_cost = new double[n];
	int *closest = new int[n];
	bool *s = new bool[n];
	result.clear();

	s[0] = true;
	result.push_back(make_pair(0,0));
	for (int i = 1; i < n; i++) {
		low_cost[i] = c[0][i];
		closest[i] = 0;
		s[i] = false;
	}

	for (int i = 0; i < n; i++) {
		double min = INF;
		int j = 0;
		for (int k = 1; k < n; k++) {
			bool judge = (low_cost[k] > 0) && ((min == INF) || (low_cost[k] < min));
			if ((judge == true) && s[k] == false) {
				min = low_cost[k];
				j = k;
			}
		}

		s[j] = true;
		result.push_back(make_pair(closest[j], j));

		for (int k = 1; k < n; k++) {
			bool judge = (c[j][k] != INF) && ((low_cost[k] == INF) || (c[j][k] < low_cost[k]));
			if ((judge == true) && (s[k] == false)) {
				low_cost[k] = c[j][k];
				closest[k] = j;
			}
		}
	}
}

// ------------------------------ MAIN -------------------------------------

int main()
{
	ofstream fr("Result.txt");
	clock_t start, end;
	double duration;
	string temp;

	// GREEDY_MIN_WERIGHT_TRAIAN
	// init
	fstream f11("附件3-1.21个基站凸多边形数据2017.csv", ios::in | ios::out);
	fstream f12("附件3-2.29个基站凸多边形数据2017.csv", ios::in | ios::out);
	vector<BaseStation> b11, b12;
	getline(f11, temp);		getline(f12, temp); // file header
	while (getline(f11, temp) && f11.good())
		b11.push_back(BaseStation(temp));
	while (getline(f12, temp) && f12.good())
		b12.push_back(BaseStation(temp));
	f11.close();
	f12.close();

	double **result11_t = new double*[b11.size()], **result12_t = new double*[b12.size()];
	int **result11_s = new int*[b11.size()], **result12_s = new int*[b12.size()];

	for (int i = 0; i < b11.size(); i++) {
		result11_t[i] = new double[b11.size()];
		result11_s[i] = new int[b11.size()];
	}
	for (int i = 0; i < b12.size(); i++) {
		result12_t[i] = new double[b12.size()];
		result12_s[i] = new int[b12.size()];
	}
	for (int i = 0; i < b11.size(); i++) {
		for (int j = 0; j < b11.size(); j++) {
			result11_s[i][j] = -1;
			result11_t[i][j] = -1;
		}
	}
	for (int i = 0; i < b12.size(); i++) {
		for (int j = 0; j < b12.size(); j++) {
			result12_s[i][j] = -1;
			result12_t[i][j] = -1;
		}
	}
	double result11_w = 0, result12_w = 0;

	// the algorithm
	start = clock();
	GreedyMinWerightTriangulation(result11_t, result11_s, b11, result11_w);
	GreedyMinWerightTriangulation(result12_t, result12_s, b12, result12_w);
	end = clock();
	duration = (double)(end - start);

	// output
	fr << "1.凸多边形最优三角部分(贪心算法):" << endl;
	fr << "20凸多边形：(每个三维序列表示一个三角剖分的三个顶点)" << endl;
	for (int i = 0; i < b11.size(); i++) {
		for (int j = 0; j < b11.size(); j++) {
			if (result11_s[i][j] != -1) {
				fr << "(" << i << ", " << result11_s[i][j] << ", " << j << "), weight = " << result11_t[i][j] << endl;
				//result11_w += result11_t[i][j];
			}
		}
	}
	cout << result11_w << ", " << result12_w << endl;
	auto it1 = b11.begin();
	while ((it1 + 1) != b11.end()) {
		result11_w += GetDistance(*it1, *(it1 + 1));
		it1++;
	}
	result11_w += GetDistance(b11[0], *it1);
	result11_w = result11_w / 2;
	fr << "最小边长弦长和为：" << result11_w << endl << endl;

	fr << "28凸多边形：(每个三维序列表示一个三角剖分的三个顶点)" << endl;
	for (int i = 0; i < b12.size(); i++) {
		for (int j = 0; j < b12.size(); j++) {
			if (result12_s[i][j] != -1) {
				fr << "(" << i << ", " << result12_s[i][j] << ", " << j << "), weight = " << result12_t[i][j] << endl;
				//result12_w += result12_t[i][j];
			}
		}
	}
	auto it2 = b12.begin();
	while ((it2 + 1) != b12.end()) {
		result12_w += GetDistance(*it2, *(it2 + 1));
		it2++;
	}
	result12_w += GetDistance(b12[0], *it2);
	result12_w = result12_w / 2;
	fr << "最小边长弦长和为：" << result12_w << endl << endl;
	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;
	fr << endl << endl;

	// HUFFMAN_TREE
	fstream f21("附件2.哈夫曼编码输入文本(1).txt", ios::in | ios::out);
	string s21 = "";
	while (getline(f21, temp) && f21.good()) {
		s21 += temp;
	}
	// algorithm
	start = clock();
	HuffmanTree result21(s21);
	end = clock();
	duration = (double)(end - start);

	// output
	fr << "2.哈夫曼编码:" << endl;
	fr << "哈夫曼编码结果：" << endl;
	result21.ShowCode(fr, "");
	map<char, string> m21;
	result21.HuffmanCode(m21, "");

	int len_huffman21 = 0;
	fr << endl << "文本的哈夫曼编码：" << endl;
	for (int i = 0; i < s21.length(); i++) {
		fr << m21[s21[i]];
		len_huffman21 += m21[s21[i]].length();
	}
	fr << endl << endl << "哈夫曼编码长度:" << len_huffman21 << endl;
	fr << "定长编码长度:" << ((int)log2(27) + 1) * s21.length() << endl;

	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;
	fr << endl << endl;
	
	// GRAPH
	fstream f31("附件1-1.22基站图的邻接矩阵-v1.csv", ios::in | ios::out);
	fstream f32("附件1-1.42基站图的邻接矩阵-v1.csv", ios::in | ios::out);
	vector<vector<double>> c31, c32;
	map<int, int> m31, m32;	 // the order number (starting from 0) mapping to the enodebid

	std::function<vector<double>(string, string)> StringParse; // to parse the long line to pieces with "delim"
	StringParse = [](string s, string delim)
	{
		vector<string> str;
		vector<double>result_int;
		size_t pos = 0;
		size_t len = s.length();
		size_t delim_len = delim.length();
		while (pos < len) {
			int find_pos = s.find(delim, pos);
			if (find_pos < 0) {
				str.push_back(s.substr(pos, len - pos));
				break;
			}
			str.push_back(s.substr(pos, find_pos - pos));
			pos = find_pos + delim_len;
		}

		vector<string>::iterator it = str.begin();
		while (it != str.end()) {
			it->erase(0, it->find_first_not_of(' '));
			result_int.push_back(stod(*it));
			it++;
		}
		return result_int;
	};
	
	getline(f31, temp);		getline(f31, temp);
	while (getline(f31, temp) && f31.good()) {
		vector<double> x = StringParse(temp, ",");
		m31.insert(map<int, int>::value_type(((int)x[0] - 1), (int)x[1]));
		x.erase(x.begin(), x.begin() + 2);
		c31.push_back(x);
	}
	getline(f32, temp);		getline(f32, temp);
	while (getline(f32, temp) && f32.good()) {
		vector<double> x = StringParse(temp, ",");
		m32.insert(map<int, int>::value_type(((int)x[0] - 1), (int)x[1]));
		x.erase(x.begin(), x.begin() + 2);
		c32.push_back(x);
	}

	vector<double> dist31, dist32;
	vector<int> prev31, prev32;	// result of Dijsktra
	vector<pair<int,int>> result31, result32;	// result of Prim
	int tar31 = 20 - 1, tar32 = 16 - 1;

	// algorithm
	start = clock();
	Dijkstra(tar31, c31, dist31, prev31);
	Dijkstra(tar32, c32, dist32, prev32);
	Prim(c31, result31);
	Prim(c32, result32);
	end = clock();
	duration = (double)(end - start);

	// output
	fr << "22基站单源最短路径：" << endl;
	for (int i = 0; i < dist31.size(); i++) {
		double t = dist31[i];
		int s = prev31[i];
		fr << "基站" << tar31+1 << "(" << m31[tar31] << ")";
		fr << "至" << i + 1 << "(" << m31[i] << ")";
		fr << ":" << i + 1 << "(" << m31[i] << ") <- ";
		while (s != tar31 && s != FAIL) {
			fr << s + 1 << "(" << m31[s] << ") <- ";
			s = prev31[s];
		}
		fr << tar31+1 << "(" << m31[tar31] << ")" << endl;
		fr << "长度为:" << t << endl;
	}
	fr << endl;

	fr << "42基站单源最短路径：" << endl;
	for (int i = 0; i < dist32.size(); i++) {
		double t = dist32[i];
		int s = prev32[i];
		fr << "基站" << tar32+1 << "(" << m32[tar32] << ")";
		fr << "至" << i + 1 << "(" << m32[i] << ")";
		fr << ":" << i + 1 << "(" << m32[i] << ") <- ";
		while (s != tar32 && s != FAIL) {
			fr << s + 1 << "(" << m32[s] << ") <- ";
			s = prev32[s];
		}
		fr << tar32+1 << "(" << m32[tar32] << ")" << endl;
		fr << "长度为:" << t << endl;
	}
	fr << endl;

	
	auto it31 = result31.begin(), it32 = result32.begin();
	fr << "22基站最小生成树:" << endl;
	while (++it31 != result31.end() - 1) {
		fr << (it31->first + 1) << "(" << m31[it31->first] << ")";
		fr << " -> " << (it31->second + 1) << "(" << m31[it31->second] << ")" << endl;
	}
	fr << endl;

	fr << "42基站最小生成树:" << endl;
	while (++it32 != result32.end() - 1) {
		fr << (it32->first + 1) << "(" << m32[it32->first] << ")";
		fr << " -> " << (it32->second + 1) << "(" << m32[it32->second] << ")" << endl;
	}
	fr << endl;
	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;
	fr << endl << endl;

	fr.close();
}

