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


//�ӽ�0�ĳ��� 
#define   EXP    0.01  

//����
#define  NUM_PUBLICTRACK 1
#define  NUM_STATION 4
#define  NUM_TRAIN 3

//�������ռ��״̬
#define  BLANK   0
#define  A_OCCUPATION   1
#define  B_OCCUPATION   2
#define  C_OCCUPATION   3

//վ̨ʹ��״̬ 
#define  FREE    0  
#define  BUSY   1

//�𳵳��ȶ��� 
#define  LENGTH  3

//�����з�����
#define  CLOCKWISE   0
#define  ANTICLOCKWISE   1

//������״̬
#define  RUN   0  
#define  STOP_EMERGENCY   1
#define  STOP_PROBE   2
#define  STOP_STATION  3
#define  STOP_MANCONTROL 4
#define  STOP_START	 5
#define  CRASH -1 

//̽���״̬  
#define  GREEN   1  
#define  RED   0

//�����������
#define	 PUBLICTRACK 9

//�������״̬
#define  TURN_BY_TURN 1
#define  MAN_COMMAND 2
#define  FAST_FIRST 3

//����
#define DOWN 0
#define LEFT 1
#define RIGHT 2
#define UP 3


typedef struct
{
	int direction;	//˳��ʱ��
	int last_direction[LENGTH];	//��һ���˶�����
	double last_speed[LENGTH];	//��һ���˶��ٶ�
	int track;	//���ڹ��
	int type;	//����/�쳵
	int state;	//��״̬
	double x[LENGTH], y[LENGTH];	//�𳵸�����λ��
	double speed;	//���ٶ�
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
	time_t time;	//ͣ��ʱ�䣬��ʱ�����Ը���
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
//	int direction_;//����ȷ�������״̬
	IMAGE *train;
	COLORREF transcolor;
public:
	bool active;
	int x;
	int y;
	int direction_;//����ȷ�������״̬
	void Create(int ix, int iy, int imaxframe, int life, bool ishow, bool iactive, IMAGE* itrain, COLORREF itrans)
		//ǰ������������,�������Ƿ��򣬵��ĸ�������֡��,�������������(-1Ϊ����),�����߸��ֱ�Ϊ�Ƿ���ʾ���Ƿ�ˢ��,�ڰ˸���IMAGE����,�ھŸ�͸��ɫ
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

void TMove(int trainid, int dx, int dy); //���ƶ�
void MoveTo(int trainid, int x, int y);	//�ƶ���ָ��λ��
void ChangeState(int trainid, int mode);	//�ı�״̬
void Train_Reset(int trainid, IMAGE* anim, int frm);	
void AddTrain(TRAIN train);
void DeleteTrain(int id);
void RunTrain();

void AddTrain(TRAIN train);
extern TRAIN T[12];
extern IMAGE img[3][4][4];
extern IMAGE background;	//����

extern Train *train[NUM_TRAIN];	//�𳵽ṹ��
extern Train *ing_train[NUM_PUBLICTRACK*4];	//׼�����빫������Ļ�
extern Train *publicTrain[NUM_PUBLICTRACK];	//���ڹ�������ĳ�
extern Probe *probe[NUM_PUBLICTRACK*4];	//̽���ṹ��
extern Station *station[NUM_STATION];	//վ̨�ṹ��
extern int PublicTrack[NUM_PUBLICTRACK];	//�������״̬
int changeDirection(int n);

extern int Map[8][13];	//��ͼ
extern int x;	//�Ƿ���ͼ�λ��汾

extern time_t now;	//���ڵ�ʱ��
extern time_t finish_initial; //��ʼ���У���ɳ�ʼ������ʱ��
extern time_t start_train[NUM_TRAIN]; //������ʱ��
extern time_t time_begin[NUM_TRAIN];
extern time_t time_held[NUM_TRAIN];
extern int turn[NUM_PUBLICTRACK];	//������Եĵ�ǰֵ
extern int strategy[NUM_PUBLICTRACK];

void trainMove(void);	//���ƶ�����
void initial(void);		//��ʼ������
void track_Change(void);


int Direction(Train *strain); //ȷ���𳵵ķ���
int checkDirection(int n, int x, int y);//˳��ʱ��ת������
extern const double direction[4][2];//��������

extern HANDLE thread[6];	//�߳�

unsigned __stdcall move(void* pArgumemt);	//���ƶ��߳�
unsigned int __stdcall Input_Command(void* pArgument);	//�����������߳�
unsigned int __stdcall Input_file(void* pArgument);	//�ļ������߳�
unsigned int __stdcall Output(void* pArgument);	//�����а汾����߳�
unsigned __stdcall Show(void* pArgument);	//ͼ�λ��汾����߳�


void drawbutton();	//��ť
void manselect(Train* train1,Train* train2);	//�˹�ѡ����
void ftoch(double a, char ch[4]);
void inttoch(int a, char ch[]);	//ת������
void drawtrainmsg();	//ͼ�λ���������ʾ
void printstring(char *ch, int x, int y, int n);
int drawdialogbox(int j);
void turnback(Train* train);	//��ͷ
void stationtime();		//����ʱ
int setDirection(int wise, double x, double y, int track);	//�ж�˳��ʱ��
void initial_graph(void);	//ͼ�λ���ʼ��

extern IMAGE imga1, imga2, imgb1, imgb2, imgc1, imgc2;
extern IMAGE imgg, imgr;
extern double acceleration[NUM_TRAIN];	//���ٶ�
extern int turnaround[NUM_TRAIN];	//��ͷ
extern time_t station_time[NUM_STATION];	//վ̨����ʱ
extern int tct[NUM_TRAIN];	//����

extern MOUSEMSG u;	//���

#pragma once

