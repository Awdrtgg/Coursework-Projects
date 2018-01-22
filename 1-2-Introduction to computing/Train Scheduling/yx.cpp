// 图形化测试.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <graphics.h>
#include <vector>
#include "train.h"


using namespace std;
#define STOP 0
#define START 1
#define SHOW 2
#define HIDE 3

vector<TRAIN> trainlist;
vector<TRAIN>::iterator ita;

void transparentimage(int dstx, int dsty, IMAGE *img, COLORREF color)	//最后一个参数透明色
{
	DWORD *src = GetImageBuffer(img);								//要画的image内存
	DWORD *dst = GetImageBuffer(GetWorkingImage());				//获取当前image的内存
	IMAGE *workimage = GetWorkingImage();
	int w1 = img->getwidth(), h1 = img->getheight(), w2, h2;			//获取要画的image长宽
	if (workimage == NULL)
	{
		w2 = getwidth();
		h2 = getwidth();
	}
	else
	{
		w2 = workimage->getwidth();
		h2 = workimage->getwidth();
	}
	for (int i = 0; i<w1; i++)
		for (int j = 0; j<h1; j++)
		{
			if (src[j*w1 + i] != color)								//显存中像素角标为y*w+x
			{
				if (j + dsty >= 0 && j + dsty<h2&&i + dstx >= 0 && i + dstx<w2)
					dst[(j + dsty)*w2 + i + dstx] = src[j*w1 + i];
			}
		}
	if (!GetWorkingImage())										//如果目标是窗口
		FlushBatchDraw();
}

void TMove(int trainid, int dx, int dy)
{
	trainlist[trainid].move(dx, dy);
}

void MoveTo(int trainid, int x, int y)
{
	trainlist[trainid].moveto(x, y);
}

void ChangeState(int trainid, int mode)
{
	trainlist[trainid].changestate(mode);
}

void Train_Reset(int trainid, IMAGE* anim, int frm)
{
	trainlist[trainid].settrain(anim);

	if (frm != -1)
		trainlist[trainid].setframe(frm);
}

void AddTrain(TRAIN train)
{
	trainlist.push_back(train);
}

void DeleteTrain(int id)
{
	trainlist.erase(trainlist.begin() + id);
}

void RunTrain()
{
	for (ita = trainlist.begin(); ita<trainlist.end(); ita++)
	{
		ita->update();
		ita->show();
		if (ita->alive() == true)
		{
			ita = trainlist.erase(ita);
		}
	}
}
