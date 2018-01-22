// main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "train.h"

HANDLE thread[6];

///*
void main()
{
	//清空记录文件
	FILE *fp;
	fp = fopen("record_regular.txt", "w");
	fclose(fp);
	fp = fopen("record_irregular.txt", "w");
	fclose(fp);

	//初始化
	initial();

	thread[0] = (HANDLE)_beginthreadex(NULL, 0, move, NULL, 0, NULL);
	thread[1] = (HANDLE)_beginthreadex(NULL, 0, Input_file, NULL, 0, NULL);
	thread[2] = (HANDLE)_beginthreadex(NULL, 0, Output, NULL, 0, NULL);
	thread[3] = (HANDLE)_beginthreadex(NULL, 0, Input_Command, NULL, 0, NULL);
	if( x == 1)
		thread[4] = (HANDLE)_beginthreadex(NULL, 0, Show, NULL, 0, NULL);


	for (int i = 0; i < 5; i++)
		CloseHandle(thread[i]);

	Sleep(2000000);

}
//*/

/*
int main()
{
	initial();

	while (1)
	{
		move();
	}

}
*/
