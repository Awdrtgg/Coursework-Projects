// Algorithm4.cpp : Designed by Xiao Yunming.
//

#include "stdafx.h" // VS projects head file

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <functional>
#include <time.h>

using namespace std;

// ---------------------------- M_COLOUR -----------------------------------

#define INF 99999

class MColor
{
public:
	int n, m;
	unsigned long long count[2];
	vector<vector<bool>> matrix;
	int *x;
	vector<vector<int>> result;
	unsigned long long sum;

	MColor(vector<vector<double>> matrix, int m, clock_t &start, clock_t &end);
	~MColor();

	void BackTrack(int t);
	bool OK(int k);
};

MColor::MColor(vector<vector<double>> matrix, int m, clock_t &start, clock_t &end)
{
	n = matrix.size();
	this->m = m;
	this->x = new int[n];
	for (int i = 0; i < n; i++) {
		x[i] = -1;
		vector<bool> v;
		this->matrix.push_back(v);
		for (int j = 0; j < n; j++) {
			if (matrix[i][j] == INF)
				this->matrix[i].push_back(false);
			else
				this->matrix[i].push_back(true);
		}
	}
	this->sum = 0;
	this->count[0] = 0;
	this->count[1] = 0;

	start = clock();
	BackTrack(0);
	end = clock();
}

MColor::~MColor()
{
	this->matrix.clear();
	delete[] this->x;
}

void MColor::BackTrack(int t)
{
	if (this->sum > 0)
		return;
	if (t >= n) {
		this->sum++;
		vector<int>xx;
		for (int i = 0; i < n; i++)
			xx.push_back(x[i]);
		this->result.push_back(xx);
	}
	else {
		for (int i = 1; i <= m; i++) {
			x[t] = i;
			if (OK(t))
				BackTrack(t + 1);
			x[t] = -1;
		}
	}
}

bool MColor::OK(int k)
{
	for (int j = 0; j < n; j++) {
		this->count[0]++;
		if (this->count[0] == 0)
			this->count[1]++;
		if (matrix[k][j] == true && x[j] == x[k])
			return false;
	}
	return true;
}

// -------------------------- TSP_BACKTRACK --------------------------------

class BTTSP
{
public:
	int n, *x, *bestx, count;
	double best_cost, current_cost;
	vector<vector<double>> a;

	BTTSP(vector<vector<double>> matrix);
	~BTTSP();

	void BackTrack(int t);
};

BTTSP::BTTSP(vector<vector<double>> matrix)
{
	this->a = matrix;
	n = this->a.size();
	x = new int[n];
	bestx = new int[n];
	for (int i = 0; i < n; i++) {
		x[i] = i;
		bestx[i] = i;
	}
	best_cost = INF;
	current_cost = 0;
	count = 0;

	BackTrack(1);
}

BTTSP::~BTTSP()
{
	delete[] x;
	delete[] bestx;
	a.clear();
}

void BTTSP::BackTrack(int i)
{
	if (i == n - 1) {
		if (a[x[n - 2]][x[n - 1]] < INF && 
			a[x[n - 1]][x[0]] < INF && 
			(best_cost == INF || current_cost + a[x[n - 2]][x[n - 1]] + a[x[n - 1]][x[0]] < best_cost))
		{
			for (int j = 0; j < n; j++)
				bestx[j] = x[j];
			best_cost = current_cost + a[x[n - 2]][x[n - 1]] + a[x[n - 1]][x[0]];
		}
	}
	else {
		for (int j = i; j < n; j++) {
			if (a[x[i - 1]][x[j]] < INF &&
				(best_cost == INF || current_cost + a[x[i - 1]][x[j]] < best_cost))
			{
				count++;
				swap(x[i], x[j]);
				current_cost += a[x[i - 1]][x[i]];
				BackTrack(i + 1);
				current_cost -= a[x[i - 1]][x[i]];
				swap(x[i], x[j]);
			}
		}
	}
}

// ----------------------- TSP_BRANCH_BOUNDING -----------------------------

class BBTSP
{
public:
	class HeapNode
	{
	public:
		double low_cost, current_cost, rest_cost;
		int s;
		vector<int> x;

		HeapNode(double lc, double cc, double rc, int ss, vector<int> xx) {
			this->low_cost = lc;
			this->current_cost = cc;
			this->rest_cost = rc;
			this->s = ss;
			this->x = xx;
		}
		HeapNode(const HeapNode &h) {
			this->low_cost = h.low_cost;
			this->current_cost = h.current_cost;
			this->rest_cost = h.rest_cost;
			this->s = h.s;
			this->x = h.x;
		}
		~HeapNode()
		{
			this->x.clear();
		}

		bool operator<(const HeapNode &h) const { return this->low_cost > h.low_cost; }; // here we do so to change the priority_queue to min_heap
	};

	vector<vector<double>> a;
	int n;
	double best_cost;
	vector<int> bestx;
	int count;

	BBTSP(vector<vector<double>> matrix);
	~BBTSP();
	BBTSP(const BBTSP &b);
};

BBTSP::BBTSP(vector<vector<double>> matrix)
{
	this->a = matrix;
	int n = a.size();
	priority_queue<HeapNode> heap;
	double *min_out = new double[n];
	double min_sum = 0;
	for (int i = 0; i < n; i++) {
		double min = INF;
		for (int j = 0; j < n; j++)
			if (a[i][j] < INF && a[i][j] < min)
				min = a[i][j];

		if (min == INF)
			return;
		min_out[i] = min;
		min_sum += min;
	}

	vector<int> x;
	for (int i = 0; i < n; i++)
		x.push_back(i);
	HeapNode enode(0, 0, min_sum, 0, x);
	heap.push(enode);
	this->best_cost = INF;
	this->count = 0;
	while (heap.empty() == false && enode.s < n - 1)
	{
		x = enode.x;
		if (enode.s == n - 2) {
			if (a[x[n - 2]][x[n - 1]] < INF &&
				a[x[n - 1]][x[0]] < INF &&
				enode.current_cost + a[x[n - 2]][x[n - 1]] + a[x[n - 1]][x[0]] < best_cost)
			{
				this->best_cost = enode.current_cost + a[x[n - 2]][x[n - 1]] + a[x[n - 1]][x[0]];
				this->bestx = x;
				enode.current_cost = best_cost;
				enode.low_cost = best_cost;
				enode.s++;
				heap.push(enode);
			}
		}
		else {
			for (int i = enode.s + 1; i < n; i++) {
				if (a[x[enode.s]][x[i]] < INF) {
					double current_cost = enode.current_cost + a[x[enode.s]][x[i]];
					double rest_cost = enode.rest_cost - min_out[x[enode.s]];
					double b = current_cost + rest_cost;
					if (b < best_cost) {
						this->count++;
						vector<int> xx = x;
						xx[enode.s + 1] = x[i];
						xx[i] = x[enode.s + 1];
						HeapNode node(b, current_cost, rest_cost, enode.s + 1, xx);
						heap.push(node);
					}
				}
			}
		}
		enode = heap.top();
		heap.pop();
	}

	//this->bestx = x;
	delete[] min_out;
}

BBTSP::~BBTSP()
{
	a.clear();
	bestx.clear();
}

BBTSP::BBTSP(const BBTSP &b)
{
	this->a = b.a;
	this->n = b.n;
	this->count = b.count;
	this->best_cost = b.best_cost;
	this->bestx = b.bestx;
}

// ------------------------------ MAIN -------------------------------------

int main()
{
	ofstream fr("Result.txt");
	clock_t start, end;
	double duration;
	string temp;

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

	
	fstream f21("附件1-1.22基站图的邻接矩阵-v2-20170601.csv", ios::in | ios::out);
	fstream f22("附件1-1.30基站图的邻接矩阵-v2-20170601.csv", ios::in | ios::out);
	fstream f23("附件1-1.42基站图的邻接矩阵-v2-20170601.csv", ios::in | ios::out);
	vector<vector<double>> c21, c22, c23;
	map<int, int> m21, m22, m23;	 // the order number (starting from 0) mapping to the enodebid

	getline(f21, temp);		getline(f21, temp);
	while (getline(f21, temp) && f21.good()) {
		vector<double> x = StringParse(temp, ",");
		m21.insert(map<int, int>::value_type(((int)x[0] - 1), (int)x[1]));
		x.erase(x.begin(), x.begin() + 2);
		c21.push_back(x);
	}
	getline(f22, temp);		getline(f22, temp);
	while (getline(f22, temp) && f22.good()) {
		vector<double> x = StringParse(temp, ",");
		m22.insert(map<int, int>::value_type(((int)x[0] - 1), (int)x[1]));
		x.erase(x.begin(), x.begin() + 2);
		c22.push_back(x);
	}
	getline(f23, temp);		getline(f23, temp);
	while (getline(f23, temp) && f23.good()) {
		vector<double> x = StringParse(temp, ",");
		m23.insert(map<int, int>::value_type(((int)x[0] - 1), (int)x[1]));
		x.erase(x.begin(), x.begin() + 2);
		c23.push_back(x);
	}

	fr << "22节点：" << endl;
	for (int i = 5; i <= 22; i++) {
		MColor M(c21, i, start, end);
		duration = (double)(end - start);
		fr << "m = " << i << ", solution = " << M.sum << ", L = ";
		if (M.count[1] != 0)
			fr << M.count[1] << "*2^64 + ";
		fr << M.count[0] << ", time cost = " << duration / CLOCKS_PER_SEC << endl;
		if (M.sum != 0) {
			auto it = M.result.begin();
			while (it != M.result.end()) {
				auto itt = it->begin();
				while (itt != it->end()) {
					fr << *itt << ", ";
					itt++;
				}
				fr << endl;
				it++;
			}
			break;
		}
	}
	fr << endl << endl;

	fr << "30节点：" << endl;
	for (int i = 4; i <= 30; i++) {
		MColor M(c22, i, start, end);
		duration = (double)(end - start);
		fr << "m = " << i << ", solution = " << M.sum << ", L = ";
		if (M.count[1] != 0)
			fr << M.count[1] << "*2^64 + ";
		fr << M.count[0] << ", time cost = " << duration / CLOCKS_PER_SEC << endl;
		if (M.sum != 0) {
			auto it = M.result.begin();
			while (it != M.result.end()) {
				auto itt = it->begin();
				while (itt != it->end()) {
					fr << *itt << ", ";
					itt++;
				}
				fr << endl;
				it++;
			}
			break;
		}
	}
	fr << endl << endl;

	fr << "42节点：" << endl;
	for (int i = 5; i <= 42; i++) {
		MColor M(c23, i, start, end);
		duration = (double)(end - start);
		fr << "m = " << i << ", solution = " << M.sum << ", L = ";
		if (M.count[1] != 0)
			fr << M.count[1] << "*2^64 + ";
		fr << M.count[0] << ", time cost = " << duration / CLOCKS_PER_SEC << endl;
		if (M.sum != 0) {
			auto it = M.result.begin();
			while (it != M.result.end()) {
				auto itt = it->begin();
				while (itt != it->end()) {
					fr << *itt << ", ";
					itt++;
				}
				fr << endl;
				it++;
			}
			break;
		}
	}

	f21.close();
	f22.close();
	f23.close();
	

	fstream f31("附件1-2.15基站图的邻接矩阵-v4-20160613.csv", ios::in | ios::out);
	fstream f32("附件1-2.20基站图的邻接矩阵-v4-20160613.csv", ios::in | ios::out);
	fstream f33("附件1-2.22基站图的邻接矩阵-v4-20160613.csv", ios::in | ios::out);
	vector<vector<double>> c31, c32, c33;
	map<int, int> m31, m32, m33;	 // the order number (starting from 0) mapping to the enodebid

	getline(f31, temp);		getline(f31, temp);
	int tempi31 = 0, tempi32 = 0, tempi33 = 0;
	while (getline(f31, temp) && f31.good()) {
		vector<double> x = StringParse(temp, ",");
		m31.insert(map<int, int>::value_type(tempi31, ((int)x[0])));
		x.erase(x.begin(), x.begin() + 2);
		c31.push_back(x);
		tempi31++;
	}
	getline(f32, temp);		getline(f32, temp);
	while (getline(f32, temp) && f32.good()) {
		vector<double> x = StringParse(temp, ",");
		m32.insert(map<int, int>::value_type(tempi32, ((int)x[0])));
		x.erase(x.begin(), x.begin() + 2);
		c32.push_back(x);
		tempi32++;
	}
	getline(f33, temp);		getline(f33, temp);
	while (getline(f33, temp) && f33.good()) {
		vector<double> x = StringParse(temp, ",");
		m33.insert(map<int, int>::value_type(tempi33, ((int)x[0])));
		x.erase(x.begin(), x.begin() + 2);
		c33.push_back(x);
		tempi33++;
	}
	f31.close();
	f32.close();
	f33.close();

	vector<vector<double>> temp3 = c31;
	c31[0] = temp3[12];
	c31[12] = temp3[0];
	for (int i = 0; i < temp3.size(); i++) {
		c31[i][0] = temp3[i][12];
		c31[i][12] = temp3[i][0];
	}
	m31.erase(0);
	m31.erase(12);
	m31.insert(map<int, int>::value_type(0, 20));
	m31.insert(map<int, int>::value_type(12, 3));
	temp3 = c32;
	c32[0] = temp3[17];
	c32[17] = temp3[0];
	for (int i = 0; i < temp3.size(); i++) {
		c32[i][0] = temp3[i][17];
		c32[i][17] = temp3[i][0];
	}
	m32.erase(0);
	m32.erase(17);
	m32.insert(map<int, int>::value_type(0, 20));
	m32.insert(map<int, int>::value_type(17, 1));
	temp3 = c33;
	c33[0] = temp3[19];
	c33[19] = temp3[0];
	for (int i = 0; i < temp3.size(); i++) {
		c33[i][0] = temp3[i][19];
		c33[i][19] = temp3[i][0];
	}
	m33.erase(0);
	m33.erase(19);
	m33.insert(map<int, int>::value_type(0, 20));
	m33.insert(map<int, int>::value_type(19, 1));

	
	fr << "回溯法：" << endl;
	fr << "15节点：" << endl;
	start = clock();
	BTTSP B31(c31);
	end = clock();
	duration = (double)(end - start);
	fr << "最短路径为: " << m31[B31.bestx[0]];
	for (int i = 1; i < c31.size(); i++)
		fr << " -> " << m31[B31.bestx[i]];
	fr << endl << "路程为: " << B31.best_cost << "，访问节点数: " << B31.count << endl;
	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;
	fr << endl << endl;

	fr << "20节点：" << endl;
	start = clock();
	BTTSP B32(c32);
	end = clock();
	duration = (double)(end - start);
	fr << "最短路径为: " << m32[B32.bestx[0]];
	for (int i = 1; i < c32.size(); i++)
		fr << " -> " << m32[B32.bestx[i]];
	fr << endl << "路程为: " << B32.best_cost << "，访问节点数: "<< B32.count << endl;
	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;
	fr << endl << endl;

	fr << "22节点：" << endl;
	start = clock();
	BTTSP B33(c33);
	end = clock();
	duration = (double)(end - start);
	fr << "最短路径为: " << m33[B33.bestx[0]];
	for (int i = 1; i < c33.size(); i++)
		fr << " -> " << m33[B33.bestx[i]];
	fr << endl << "路程为: " << B33.best_cost << "，访问节点数: " << B33.count << endl;
	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;

	fr << endl << endl;
	

	fr << "分支限界法：" << endl;
	fr << "15节点：" << endl;
	start = clock();
	BBTSP B41(c31);
	end = clock();
	duration = (double)(end - start);
	fr << "最短路径为: " << m31[B41.bestx[0]];
	for (int i = 1; i < c31.size(); i++)
		fr << " -> " << m31[B41.bestx[i]];
	fr << endl << "路程为: " << B41.best_cost << "，访问节点数: " << B41.count << endl;
	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;

	fr << "20节点：" << endl;
	start = clock();
	BBTSP B42(c32);
	end = clock();
	duration = (double)(end - start);
	fr << "最短路径为: " << m32[B42.bestx[0]];
	for (int i = 1; i < c32.size(); i++)
		fr << " -> " << m32[B42.bestx[i]];
	fr << endl << "路程为: " << B42.best_cost << "，访问节点数: "<< B42.count << endl;
	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;

	fr << "22节点：" << endl;
	start = clock();
	BBTSP B43(c33);
	end = clock();
	duration = (double)(end - start);
	fr << "最短路径为: " << m33[B43.bestx[0]];
	for (int i = 1; i < c33.size(); i++)
		fr << " -> " << m33[B43.bestx[i]];
	fr << endl << "路程为: " << B43.best_cost << "，访问节点数: " << B43.count << endl;
	fr << "耗时" << duration / CLOCKS_PER_SEC << "秒" << endl;

	fr << endl << endl;

	fr.close();
}

