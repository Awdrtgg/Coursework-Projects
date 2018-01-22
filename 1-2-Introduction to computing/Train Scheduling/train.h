#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>
#include <vector>

#pragma warning (disable: 4996)
#define _CRT_SECURE_NO_WARNINGS


//接近0的常量 
#define   EXP    0.01  

//数量
#define  NUM_PUBLICTRACK 1
#define  NUM_STATION 4
#define  NUM_TRAIN 3

//公共轨道占用状态
#define  BLANK   0
#define  A_OCCUPATION   1
#define  B_OCCUPATION   2
#define  C_OCCUPATION   3

//站台使用状态 
#define  FREE    0  
#define  BUSY   1

//火车长度定义 
#define  LENGTH  3

//火车运行方向定义
#define  CLOCKWISE   0
#define  ANTICLOCKWISE   1

//火车运行状态
#define  RUN   0  
#define  STOP_EMERGENCY   1
#define  STOP_PROBE   2
#define  STOP_STATION  3
#define  STOP_MANCONTROL 4
#define  STOP_START	 5
#define  CRASH -1 

//探测点状态  
#define  GREEN   1  
#define  RED   0

//公共轨道数据
#define	 PUBLICTRACK 9

//公共轨道状态
#define  TURN_BY_TURN 1
#define  MAN_COMMAND 2
#define  FAST_FIRST 3

//方向
#define DOWN 0
#define LEFT 1
#define RIGHT 2
#define UP 3


typedef struct
{
	int direction;	//顺逆时针
	int last_direction[LENGTH];	//上一次运动方向
	double last_speed[LENGTH];	//上一次运动速度
	int track;	//所在轨道
	int type;	//慢车/快车
	int state;	//火车状态
	double x[LENGTH], y[LENGTH];	//火车各车厢位置
	double speed;	//火车速度
} Train;

typedef struct
{
	int state;
	int x, y;
} Probe;

typedef struct
{
	int state;
	int x, y;
	time_t time;	//停靠时间，暂时不可以更改
} Station;

void transparentimage(int dstx, int dsty, IMAGE *img, COLORREF color);

class TRAIN
{
private:
	bool shows;
	//bool active;
	//int x;
	//int y;
	int frame;
	int maxframe;
	int life;
//	int direction_;//用来确定人物的状态
	IMAGE *train;
	COLORREF transcolor;
public:
	bool active;
	int x;
	int y;
	int direction_;//用来确定人物的状态
	void Create(int ix, int iy, int imaxframe, int life, bool ishow, bool iactive, IMAGE* itrain, COLORREF itrans)
		//前两个像素坐标,第三个是方向，第四个动画总帧数,第五个动画寿命(-1为无限),第六七个分别为是否显示、是否刷新,第八个是IMAGE数组,第九个透明色
	{
		x = ix;
		y = iy;
		frame = 0;

		maxframe = imaxframe;
		shows = ishow;
		active = iactive;
		train = itrain;
		transcolor = itrans;
	}
	void move(int dx, int dy)
	{
		x += dx;
		y += dy;
	}
	void moveto(int mx, int my)
	{
		x = mx;
		y = my;
	}
	void changestate(int mode)
	{
		switch (mode)
		{
		case RUN:
			active = true;
			break;
		case STOP_EMERGENCY:
			active = false;
			break;
		case STOP_MANCONTROL:
			active = false;
			break;
		case STOP_PROBE:
			active = false;
			break;
		case STOP_STATION:
			active = false;
			break;
		case STOP_START:
			active = false;
			break;
		case CRASH:
			shows = false;
			break;
		}
	}

	void settrain(IMAGE* strain)
	{
		train = strain;
	}

	void setframe(int frm)
	{
		frame = frm;
	}
	void update()
	{
		if (active == true)
		{
			frame += 1;
			if (frame == maxframe)
				frame = 0;
			if (life>0)
				life -= 1;

		}
	}
	bool alive()
	{
		if (life>0)
			return true;
		else if (life == -1)
			return true;
		else
			return false;
	}
	void show()
	{
		if (shows == true)
		{
			transparentimage(x, y, &train[frame], transcolor);
		}
	}
};

void TMove(int trainid, int dx, int dy); //火车移动
void MoveTo(int trainid, int x, int y);	//移动到指定位置
void ChangeState(int trainid, int mode);	//改变状态
void Train_Reset(int trainid, IMAGE* anim, int frm);	
void AddTrain(TRAIN train);
void DeleteTrain(int id);
void RunTrain();

void AddTrain(TRAIN train);
extern TRAIN T[12];
extern IMAGE img[3][4][4];
extern IMAGE background;	//背景

extern Train *train[NUM_TRAIN];	//火车结构体
extern Train *ing_train[NUM_PUBLICTRACK*4];	//准备进入公共轨道的火车
extern Train *publicTrain[NUM_PUBLICTRACK];	//正在公共轨道的车
extern Probe *probe[NUM_PUBLICTRACK*4];	//探测点结构体
extern Station *station[NUM_STATION];	//站台结构体
extern int PublicTrack[NUM_PUBLICTRACK];	//公共轨道状态
int changeDirection(int n);

extern int Map[8][13];	//地图
extern int x;	//是否是图形化版本

extern time_t now;	//现在的时间
extern time_t finish_initial; //开始运行（完成初始化）的时间
extern time_t start_train[NUM_TRAIN]; //火车启动时间
extern time_t time_begin[NUM_TRAIN];
extern time_t time_held[NUM_TRAIN];
extern int turn[NUM_PUBLICTRACK];	//交替策略的当前值
extern int strategy[NUM_PUBLICTRACK];

void trainMove(void);	//火车移动函数
void initial(void);		//初始化函数
void track_Change(void);


int Direction(Train *strain); //确定火车的方向
int checkDirection(int n, int x, int y);//顺逆时针转换函数
extern const double direction[4][2];//方向数组

extern HANDLE thread[6];	//线程

unsigned __stdcall move(void* pArgumemt);	//火车移动线程
unsigned int __stdcall Input_Command(void* pArgument);	//命令行输入线程
unsigned int __stdcall Input_file(void* pArgument);	//文件输入线程
unsigned int __stdcall Output(void* pArgument);	//命令行版本输出线程
unsigned __stdcall Show(void* pArgument);	//图形化版本输出线程


void drawbutton();	//按钮
void manselect(Train* train1,Train* train2);	//人工选择车辆
void ftoch(double a, char ch[4]);
void inttoch(int a, char ch[]);	//转化数组
void drawtrainmsg();	//图形化火车数据显示
void printstring(char *ch, int x, int y, int n);
int drawdialogbox(int j);
void turnback(Train* train);	//掉头
void stationtime();		//倒计时
int setDirection(int wise, double x, double y, int track);	//判断顺逆时针
void initial_graph(void);	//图形化初始化

extern IMAGE imga1, imga2, imgb1, imgb2, imgc1, imgc2;
extern IMAGE imgg, imgr;
extern double acceleration[NUM_TRAIN];	//加速度
extern int turnaround[NUM_TRAIN];	//掉头
extern time_t station_time[NUM_STATION];	//站台倒计时
extern int tct[NUM_TRAIN];	//换轨

extern MOUSEMSG u;	//鼠标

#pragma once

