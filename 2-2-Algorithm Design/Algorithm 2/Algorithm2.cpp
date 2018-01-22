// Algorithm1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <cmath>

using namespace std;

#define NUM_STATION 1033
#define EARTH_RADIUS 6378.137
#define M_PI 3.14159265358979323846
#define INF 100000.0

// ------------------------------ Over All ---------------------------------------

class BaseStation
{
public:
	int enodebid;
	double longitude, latitude, k_dist;

	BaseStation();
	BaseStation(string line);
	~BaseStation() {};
	BaseStation(const BaseStation &b);

	bool operator<(BaseStation &b) { return this->k_dist < b.k_dist; };
	bool operator<=(BaseStation &b) { return this->k_dist <= b.k_dist; };
	bool operator>(BaseStation &b) { return this->k_dist > b.k_dist; };
	bool operator>=(BaseStation &b) { return this->k_dist >= b.k_dist; };
	bool operator==(BaseStation &b) { return this->k_dist == b.k_dist; };
};

// this class is for the convinience of sorting by longitude in the closest pair algorithm
class BaseStationLongitude :public BaseStation
{
public:
	BaseStationLongitude() :BaseStation() {};
	BaseStationLongitude(const BaseStationLongitude &b);
	BaseStationLongitude operator=(BaseStation &b);

	bool operator<(BaseStationLongitude &b) { return this->longitude < b.longitude; };
	bool operator<=(BaseStationLongitude &b) { return this->longitude <= b.longitude; };
	bool operator==(BaseStationLongitude &b) { return this->longitude == b.longitude; };
	bool operator>=(BaseStationLongitude &b) { return this->longitude >= b.longitude; };
	bool operator>(BaseStationLongitude &b) { return this->longitude > b.longitude; };
};

// this class is for the convinience of sorting by latitude in the closest pair algorithm
class BaseStationLatitude :public BaseStation
{
public:
	int p;

	BaseStationLatitude(BaseStationLongitude &b, int p);
	BaseStationLatitude() :BaseStation() { p = 0; };
	BaseStationLatitude(const BaseStationLatitude &b);
	BaseStationLatitude operator=(BaseStation &b);

	bool operator<(BaseStationLatitude &b) { return this->latitude < b.latitude; };
	bool operator<=(BaseStationLatitude &b) { return this->latitude <= b.latitude; };
	bool operator==(BaseStationLatitude &b) { return this->latitude == b.latitude; };
	bool operator>=(BaseStationLatitude &b) { return this->latitude >= b.latitude; };
	bool operator>(BaseStationLatitude &b) { return this->latitude > b.latitude; };
};

BaseStation::BaseStation()
{
	this->enodebid = 0;
	this->k_dist = 0;
	this->latitude = 0;
	this->longitude = 0;
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
	this->k_dist = stod(s[3]);
}

BaseStation::BaseStation(const BaseStation &b)
{
	this->enodebid = b.enodebid;
	this->longitude = b.longitude;
	this->latitude = b.latitude;
	this->k_dist = b.k_dist;
}

BaseStationLongitude::BaseStationLongitude(const BaseStationLongitude &b)
{
	this->enodebid = b.enodebid;
	this->longitude = b.longitude;
	this->latitude = b.latitude;
	this->k_dist = b.k_dist;
}

BaseStationLatitude::BaseStationLatitude(const BaseStationLatitude &b)
{
	this->enodebid = b.enodebid;
	this->longitude = b.longitude;
	this->latitude = b.latitude;
	this->k_dist = b.k_dist;
	this->p = b.p;
}

BaseStationLatitude BaseStationLatitude::operator=(BaseStation &b)
{
	this->enodebid = b.enodebid;
	this->k_dist = b.k_dist;
	this->latitude = b.latitude;
	this->longitude = b.longitude;
	return *this;
}

BaseStationLongitude BaseStationLongitude::operator=(BaseStation &b)
{
	this->enodebid = b.enodebid;
	this->k_dist = b.k_dist;
	this->latitude = b.latitude;
	this->longitude = b.longitude;
	return *this;
}

BaseStationLatitude::BaseStationLatitude(BaseStationLongitude &b, int p)
{
	this->enodebid = b.enodebid;
	this->k_dist = b.k_dist;
	this->latitude = b.latitude;
	this->longitude = b.longitude;
	this->p = p;
}

static vector<BaseStation> base; // file reading

void Init()
{
	fstream fp("1033base.csv");
	string s;
	getline(fp, s); // the first line

	while (getline(fp, s) && fp.good()) {
		BaseStation b(s);
		base.push_back(b);
	}
}

static int depthest; // wildly used to get the depth of many algorithms

double GetDistance(BaseStation A, BaseStation B)
{
	auto rad = [](const double& f) {return f * M_PI / 180.0; };
	double radLatA = rad(A.latitude), radLatB = rad(B.latitude);
	double radLonA = rad(A.longitude), radLonB = rad(B.longitude);
	double s = 1000 * EARTH_RADIUS * acos(cos(radLatA) * cos(radLatB) * cos(radLonA - radLonB) + sin(radLatA) * sin(radLatB));
	//s = round(s * 1000);// round(s*1000,6)????
	return s;
}

// ------------------------ MergeSort With Recurrence --------------------------------

template <class Type>
void Merge(Type c[], Type d[], int left, int middle, int right)
{
	int i = left, j = middle + 1, k = left;
	while ((i <= middle) && (j <= right)) {
		if (c[i] < c[j])
			d[k++] = c[i++];
		else
			d[k++] = c[j++];
	}

	if (i > middle)
		while (j <= right)
			d[k++] = c[j++];
	else
		while (i <= middle)
			d[k++] = c[i++];
}

template <class Type>
void MergeSort(Type a[], Type b[], int left, int right, int depth = 0)
{
	depth += 1;
	if (depth > depthest)
		depthest = depth;

	if (left < right) {
		int i = (left + right) / 2;
		MergeSort(a, b, left, i, depth);
		MergeSort(a, b, i + 1, right, depth);
		Merge(a, b, left, i, right);
		for (int i = left; i <= right; i++)
			a[i] = b[i];
	}
}

// ----------------------- MergeSort Without Recurrence -------------------------------

template <class Type>
void MergePass(Type x[], Type y[], int s, int n)
{
	int i = 0;
	while (i <= n - 2 * s) {
		// Share the same funcion Merge with the above merge sort with recurrence
		Merge(x, y, i, i + s - 1, i + 2 * s - 1);
		i = i + 2 * s;
	}
	if (i + s <= n)
		Merge(x, y, i, i + s - 1, n - 1);
	else
		for (int j = i; j < n; j++) // Attention! The ending condition is wrong in PPT, should be j < n
			y[j] = x[j];
}

template <class Type>
void MergeSortNoRe(Type a[], int n)
{
	Type b[NUM_STATION];
	int s = 1;
	while (s < n) {
		MergePass(a, b, s, n);
		s += s;
		MergePass(b, a, s, n);
		s += s;
	}
}

// -------------------------------- Quick Sort ----------------------------------------

template<class Type>
int Partition(Type a[], int p, int r)
{
	int i = p, j = r + 1;
	Type x = a[p];
	while (true) {
		while (a[++i] < x && i < r);
		while (a[--j] > x);

		if (i >= j)
			break;

		// swap a[i] and a[j]
		Type t = a[i];
		a[i] = a[j];
		a[j] = t;
	}
	a[p] = a[j];
	a[j] = x;
	return j;
}

template<class Type>
void QuickSort(Type a[], int p, int r, int depth = 0)
{
	depth += 1;
	if (depth > depthest)
		depthest = depth;

	// to check if a[p:r] fits the condition that it doesn't decrease as the index increases
	int j = p;
	while (j < r && a[j] <= a[j + 1])	++j;
	if (j >= r)
		return;

	if (p < r) {
		int q = Partition(a, p, r);
		QuickSort(a, p, q - 1, depth);
		QuickSort(a, q + 1, r, depth);
	}
}

// --------------------------- Randomized Quick Sort ----------------------------------

template<class Type>
int RandomizedPartition(Type a[], int p, int r)
{
	// get a random value from p to r
	srand((unsigned int)clock());
	int i = p + ((int)rand()) % (r - p);

	// swap a[i] and a[p]
	Type t = a[p];
	a[p] = a[i];
	a[i] = t;

	return Partition(a, p, r);
}

template<class Type>
void RandomizedQuickSort(Type a[], int p, int r, int depth = 0)
{
	depth += 1;
	if (depth > depthest)
		depthest = depth;

	// to check if a[p:r] fits the condition that it doesn't decrease as the index increases
	int j = p;
	while (j < r && a[j] <= a[j + 1])	++j;
	if (j >= r)
		return;

	if (p < r) {
		int q = Partition(a, p, r);
		RandomizedQuickSort(a, p, q - 1,depth);
		RandomizedQuickSort(a, q + 1, r, depth);
	}
}

// ----------------------------- Randomized Select ------------------------------------

// 2 parts
template<class Type>
Type RandomizedSelect2(Type a[], int p, int r, int k, int depth = 0)
{
	depth += 1;
	if (depth > depthest)
		depthest = depth;

	if (p == r)
		return a[p];
	int i = RandomizedPartition(a, p, r);
	int L = i - p + 1;
	if (k <= L)
		return RandomizedSelect2(a, p, i, k, depth);
	else
		return RandomizedSelect2(a, i + 1, r, k - L, depth);
}

// 3 parts
template<class Type>
Type RandomizedSelect3(Type a[], int p, int r, int k, int depth = 0)
{
	depth += 1;
	if (depth > depthest)
		depthest = depth;

	if (p == r)
		return a[p];
	int i = RandomizedPartition(a, p, r);
	int L = i - p + 1;
	if (k == L)
		return a[i];
	else if (k < L)
		return RandomizedSelect3(a, p, i - 1, k, depth);
	else
		return RandomizedSelect3(a, i + 1, r, k - L, depth);
}

// ------------------------------- Closest Pair --------------------------------------

class Pair
{
public:
	BaseStation a, b;
	double dist;

	Pair() { dist = INF; };
	Pair(BaseStation a, BaseStation b, double d);
	Pair(const Pair &p) { this->a = p.a; this->b = p.b; this->dist = p.dist; };
	
	bool operator==(Pair &p) { return (this->a == p.a && this->b == p.b); };
	Pair operator=(Pair &p) { this->a = p.a; this->b = p.b; this->dist = p.dist; return *this; };
};

Pair::Pair(BaseStation a, BaseStation b, double d)
{
	this->a = a;
	this->b = b;
	this->dist = d;
}

Pair ClosestPair(BaseStationLongitude x[], BaseStationLatitude y[], BaseStationLatitude z[], int left, int right, int depth = 0)
{
	depth += 1;
	if (depth > depthest)
		depthest = depth;

	if (right - left <= 0)
		return Pair();
	else if (right - left == 1)
		return Pair(x[left], x[right], GetDistance(x[left], x[right]));
	else if (right - left == 2) {
		double d1 = GetDistance(x[left], x[left + 1]);
		double d2 = GetDistance(x[left + 1], x[right]);
		double d3 = GetDistance(x[left], x[right]);
		if (d1 <= d2 && d1 <= d3)
			return Pair(x[left], x[left + 1], d1);
		else if (d2 <= d3)
			return Pair(x[left + 1], x[right], d2);
		else
			return Pair(x[left], x[right], d3);
	}


	int mid = (left + right) / 2;
	int f = left;
	for (int i = left; i <= right; i++) {
		if (y[i].longitude <= x[mid].longitude)
			z[f++] = y[i];
	}
	for (int i = left; i <= right; i++) {
		if (y[i].longitude > x[mid].longitude)
			z[f++] = y[i];
	}

	Pair dmin = ClosestPair(x, z, y, left, mid, depth);
	Pair dminr = ClosestPair(x, z, y, mid + 1, right, depth);

	dmin = dmin.dist < dminr.dist ? dmin : dminr;

	Merge(z, y, left, mid, right);

	int k = left;
	auto angle = [](const double& f) {return f * 180.0 / M_PI / EARTH_RADIUS / 1000; };
	double d = angle(dmin.dist);
	for (int i = left; i <= right; i++)
		if (abs(x[mid].longitude - y[i].longitude) <= d)
			z[k++] = y[i];

	for (int i = left; i < k; i++) {
		for (int j = i + 1; j < k && abs(z[j].latitude - z[i].latitude) <= d; j++) {
			double dp = GetDistance(z[i], z[j]);
			if (dp < dmin.dist)
				dmin = Pair(z[i], z[j], dp);
		}
	}

	return dmin;
}

// return the pair with smallest distance
Pair cpair2(BaseStation x[])
{
	// For the convinience of the calculation of the closest pair, 
	// here we sort the bases by longitude and latitude respectively
	BaseStationLongitude LO[NUM_STATION];
	BaseStationLatitude LA1[NUM_STATION], LA2[NUM_STATION];
	for (int i = 0; i < NUM_STATION; i++)
		LO[i] = x[i];
	MergeSortNoRe(LO, NUM_STATION); // sort LO by longitude

	for (int i = 0; i < NUM_STATION; i++)
		LA1[i] = BaseStationLatitude(LO[i], i);
	MergeSortNoRe(LA1, NUM_STATION); // sort LA1 by latitude

	return ClosestPair(LO, LA1, LA2, 0, NUM_STATION - 1);
}

// ------------------------------ Closest Pair 2 -------------------------------------

void Compare(Pair &min1, Pair &min2, Pair &tmin1, Pair &tmin2)
{
	// To check and change if the min1 and min2 are
	// not the smallest and 2nd smallest number 
	// in above 4 parameters
	if (tmin1.dist < min1.dist) {
		min2 = min1;
		min1 = tmin1;
		if (tmin2.dist < min2.dist)
			min2 = tmin2;
	}
	else if (tmin1.dist > min1.dist && tmin1.dist < min2.dist)
		min2 = tmin1;
	else if (tmin1.dist == min1.dist && tmin2.dist < min2.dist)
		min2 = tmin2;
}

void ClosestPair2(BaseStationLongitude x[], BaseStationLatitude y[], BaseStationLatitude z[], Pair &min1, Pair &min2, int left, int right, int depth = 0)
{
	depth += 1;
	if (depth > depthest)
		depthest = depth;

	Pair tmin1, tmin2; //temperarily variable

	if (right - left <= 0)
		return;
	else if (right - left == 1) {
		tmin1 = Pair(x[left], x[right], GetDistance(x[left], x[right]));
		Compare(min1, min2, tmin1, tmin2);
		return;
	}
	else if (right - left == 2) {
		double d1 = GetDistance(x[left], x[left + 1]);
		double d2 = GetDistance(x[left + 1], x[right]);
		double d3 = GetDistance(x[left], x[right]);
		if (d1 <= d2 && d1 <= d3) {
			tmin1 = Pair(x[left], x[left + 1], d1);
			tmin2 = d2 <= d3 ? Pair(x[left + 1], x[right], d2) : Pair(x[left], x[right], d3);
		}
		else if (d2 <= d3) {
			tmin1 = Pair(x[left + 1], x[right], d2);
			tmin2 = d1 <= d3 ? Pair(x[left], x[left + 1], d1) : Pair(x[left], x[right], d3);
		}
		else {
			tmin1 = Pair(x[left], x[right], d3);
			tmin2 = d1 <= d2 ? Pair(x[left], x[left + 1], d1) : Pair(x[left + 1], x[right], d2);
		}
		Compare(min1, min2, tmin1, tmin2);
		return;
	}


	int mid = (left + right) / 2;
	int f = left;
	for (int i = left; i <= right; i++) {
		if (y[i].longitude <= x[mid].longitude)
			z[f++] = y[i];
	}
	for (int i = left; i <= right; i++) {
		if (y[i].longitude > x[mid].longitude)
			z[f++] = y[i];
	}

	ClosestPair2(x, z, y, tmin1, tmin2, left, mid, depth);
	Compare(min1, min2, tmin1, tmin2);
	ClosestPair2(x, z, y, tmin1, tmin2, mid + 1, right, depth);
	Compare(min1, min2, tmin1, tmin2);

	Merge(z, y, left, mid, right);

	int k = left;
	auto angle = [](const double& f) {return f * 180.0 / M_PI / EARTH_RADIUS / 1000 ; };
	double d = angle(min2.dist);
	for (int i = left; i <= right; i++)
		if (abs(x[mid].longitude - y[i].longitude) <= d)
			z[k++] = y[i];

	for (int i = left; i < k; i++) {
		for (int j = i + 1; j < k && abs(z[j].latitude - z[i].latitude) <= d; j++) {
			double dp = GetDistance(z[i], z[j]);
			if (dp < min1.dist) {
				min2 = min1;
				min1 = Pair(z[i], z[j], dp);
			}
			else if (dp > min1.dist && dp < min2.dist) {
				min2 = Pair(z[i], z[j], dp);
			}
		}
	}
}

// return both the pairs with smallest and 2nd smallest distances by min1 & min2 respectively
void cpair22(BaseStation x[], Pair &min1, Pair &min2)
{
	BaseStationLongitude LO[NUM_STATION];
	BaseStationLatitude LA1[NUM_STATION], LA2[NUM_STATION];
	for (int i = 0; i < NUM_STATION; i++)
		LO[i] = x[i];
	MergeSortNoRe(LO, NUM_STATION); // sort LO by longitude

	for (int i = 0; i < NUM_STATION; i++)
		LA1[i] = BaseStationLatitude(LO[i], i);
	MergeSortNoRe(LA1, NUM_STATION); // sort LA1 by latitude

	ClosestPair2(LO, LA1, LA2, min1, min2, 0, NUM_STATION - 1);
}

// ----------------------------------- Main ------------------------------------------

int main()
{
	// Read file and initialization
	Init();
	wfstream fp("Result.txt");
	BaseStation a[NUM_STATION], b[NUM_STATION];
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	double duration;
	clock_t start, end;

	// Merge Sort With Recurrence
	depthest = 0;
	start = clock();
	MergeSort(a, b, 0, NUM_STATION - 1);
	end = clock();
	duration = (double)(end - start);
	fp << "The Merge Sort With Recurrence costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "Here's the sorting result:" << endl << "The format is: ENODEBID/LONGITUDE/LATITUDE/K_DIST " << endl;
	for (int i = 0; i < NUM_STATION; i++)
		fp << a[i].enodebid << ", " << a[i].longitude << ", " << a[i].latitude << ", " << a[i].k_dist << endl;
	fp << endl << endl;
	
	// Merge Sort Without Recurrence
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	start = clock();
	MergeSortNoRe(a, NUM_STATION);
	end = clock();
	duration = (double)(end - start);
	fp << "The Merge Sort Without Recurrence costs " << duration / CLOCKS_PER_SEC << " secondes" << endl;
	fp << "Here's the sorting result:" << endl << "The format is: ENODEBID/LONGITUDE/LATITUDE/K_DIST " << endl;
	for (int i = 0; i < NUM_STATION; i++)
		fp << a[i].enodebid << ", " << a[i].longitude << ", " << a[i].latitude << ", " << a[i].k_dist << endl;
	fp << endl << endl;

	// Quick Sort
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	QuickSort(a, 0, NUM_STATION - 1);
	end = clock();
	duration = (double)(end - start);
	fp << "The Quick Sort costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "Here's the sorting result:" << endl << "The format is: ENODEBID/LONGITUDE/LATITUDE/K_DIST " << endl;
	for (int i = 0; i < NUM_STATION; i++)
		fp << a[i].enodebid << ", " << a[i].longitude << ", " << a[i].latitude << ", " << a[i].k_dist << endl;
	fp << endl << endl;

	// Randomized Quick Sort
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	RandomizedQuickSort(a, 0, NUM_STATION - 1);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Quick Sort costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "Here's the sorting result:" << endl << "The format is: ENODEBID/LONGITUDE/LATITUDE/K_DIST " << endl;
	for (int i = 0; i < NUM_STATION; i++)
		fp << a[i].enodebid << ", " << a[i].longitude << ", " << a[i].latitude << ", " << a[i].k_dist << endl;
	fp << endl << endl;

	BaseStation temp;

	// Randomized Select (2 parts) for 1st smallest k_dist
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	temp = RandomizedSelect2(a, 0, NUM_STATION - 1, 1);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Select (2 parts) costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The station with 1st smallest k_dist is " << temp.enodebid << ", and its k_dist is " << temp.k_dist << endl << endl;

	// Randomized Select (3 parts) for 1st smallest k_dist
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	temp = RandomizedSelect3(a, 0, NUM_STATION - 1, 1);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Select (3 parts) costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The station with 1st smallest k_dist is " << temp.enodebid << ", and its k_dist is " << temp.k_dist << endl << endl;

	// Randomized Select (2 parts) for 5th smallest k_dist
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	temp = RandomizedSelect2(a, 0, NUM_STATION - 1, 5);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Select (2 parts) costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The station with 5th smallest k_dist is " << temp.enodebid << ", and its k_dist is " << temp.k_dist << endl << endl;

	// Randomized Select (3 parts) for 5th smallest k_dist
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	temp = RandomizedSelect3(a, 0, NUM_STATION - 1, 5);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Select (3 parts) costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The station with 5th smallest k_dist is " << temp.enodebid << ", and its k_dist is " << temp.k_dist << endl << endl;

	// Randomized Select (2 parts) for 50th smallest k_dist
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	temp = RandomizedSelect2(a, 0, NUM_STATION - 1, 50);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Select (2 parts) costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The station with 50th smallest k_dist is " << temp.enodebid << ", and its k_dist is " << temp.k_dist << endl << endl;

	// Randomized Select (3 parts) for 50th smallest k_dist
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	temp = RandomizedSelect3(a, 0, NUM_STATION - 1, 50);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Select (3 parts) costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The station with 50th smallest k_dist is " << temp.enodebid << ", and its k_dist is " << temp.k_dist << endl << endl;

	// Randomized Select (2 parts) for largest k_dist
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	temp = RandomizedSelect2(a, 0, NUM_STATION - 1, NUM_STATION);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Select (2 parts) costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The station with largest k_dist is " << temp.enodebid << ", and its k_dist is " << temp.k_dist << endl << endl;

	// Randomized Select (3 parts) for largest k_dist
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	temp = RandomizedSelect3(a, 0, NUM_STATION - 1, NUM_STATION);
	end = clock();
	duration = (double)(end - start);
	fp << "The Randomized Select (3 parts) costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The station with largest k_dist is " << temp.enodebid << ", and its k_dist is " << temp.k_dist << endl << endl;

	// Closest Pair
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	Pair dmin = cpair2(a);
	end = clock();
	duration = (double)(end - start);
	fp << "The Closest Pair costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The pair station with minimun distance are " << dmin.a.enodebid << " and " << dmin.b.enodebid;
	fp << ", and its distance is " << dmin.dist << endl << endl;

	// Closest Pair 2
	Pair min1, min2;
	for (int i = 0; i < NUM_STATION; i++)
		a[i] = base[i];
	depthest = 0;
	start = clock();
	cpair22(a, min1, min2);
	end = clock();
	duration = (double)(end - start);
	fp << "The Closest Pair 2 costs " << duration / CLOCKS_PER_SEC << " secondes, ";
	fp << "The depth of the recurrence is " << depthest << endl;
	fp << "The pair station with minimun distance are " << min1.a.enodebid << " and " <<min1.b.enodebid;
	fp << ", and its distance is " << min1.dist << endl;
	fp << "The pair station with 2nd minimun distance are " << min2.a.enodebid << " and " << min2.b.enodebid;
	fp << ", and its distance is " << min2.dist << endl<< endl;
	fp.close();
}
