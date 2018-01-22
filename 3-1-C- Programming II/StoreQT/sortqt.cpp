/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		sortqt.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Realize the sort of search list.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include "storeqt.h"

void StoreQT::SearchSort(int index) {
	if (this->search_result_1.size() == 0)
		return;
	
	//std::function<bool(const vector<string>&, const vector<string>&)> *comp_v_s;
	QHeaderView *head = common_table_1->horizontalHeader();
	int order = head->sortIndicatorOrder();

	if (index == 2 || index == 3) {
		if (order == Qt::AscendingOrder) {
			auto comp_v_s = [index](const vector<string> &a, const vector<string> &b) {
				return (stof(a[index]) < stof(b[index]));
			};
			sort(search_result_1.begin(), search_result_1.end(), comp_v_s);
		}
		else {
			auto comp_v_s = [index](const vector<string> &a, const vector<string> &b) {
				return (stof(a[index]) > stof(b[index]));
			};
			sort(search_result_1.begin(), search_result_1.end(), comp_v_s);
		}
	}
	else {
		if (order == Qt::AscendingOrder) {
			auto comp_v_s = [index](const vector<string> &a, const vector<string> &b) {
				return (a[index] < b[index]);
			};
			sort(search_result_1.begin(), search_result_1.end(), comp_v_s);
		}
		else {
			auto comp_v_s = [index](const vector<string> &a, const vector<string> &b) {
				return (a[index] > b[index]);
			};
			sort(search_result_1.begin(), search_result_1.end(), comp_v_s);
		}
	}

	RefreshTable1();
}
