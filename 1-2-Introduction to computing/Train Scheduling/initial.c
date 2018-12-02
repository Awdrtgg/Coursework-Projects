#include "stdafx.h"
#include "train.h"
#define MAX_WAITING_TIME 5.0

Train *train[NUM_TRAIN];
Train *ing_train[NUM_PUBLICTRACK*4];
Probe *probe[NUM_PUBLICTRACK*4];
Station *station[NUM_STATION];
Train *publicTrain[NUM_PUBLICTRACK];
int PublicTrack[NUM_PUBLICTRACK];
int turn[NUM_PUBLICTRACK];
int strategy[NUM_PUBLICTRACK];
int setDirection(int wise, double x, double y, int track);

IMAGE img0, img1, img2, img3;
IMAGE background;

time_t time_begin[NUM_TRAIN];
time_t time_held[NUM_TRAIN];
time_t now, finish_initial;
time_t start_train[NUM_TRAIN];
int Map[8][13];
int x = 0;

TRAIN T[12];
IMAGE img[3][4][4];

IMAGE imga1, imga2, imgb1, imgb2, imgc1, imgc2;

//方向
const double direction[4][2] = {
	{ 0.0, -1.0 },{ 1.0, 0.0 },{ 0.0, 1.0 },{ -1.0, 0.0 }
};

//自动设置多节车厢
void setMoreCarriage(Train *strain)
{
	int i = 1; 
	while (i < LENGTH)
	{
		int d = (strain->last_direction[i - 1] + 2) % 4;
		strain->x[i] = strain->x[i - 1] + direction[d][0];
		strain->y[i] = strain->y[i - 1] + direction[d][1];

		strain->last_speed[i] = (strain->speed > 1.0 ? 1.0 : strain->speed);

		strain->last_direction[i] = setDirection(strain->direction, strain->x[i], strain->y[i], strain->track);
		i++;
	}
}

//显示名称，输入值，返回整数
int input_i(char *name, int min, int max) {
	int size = 0;
	clock_t wastestart = 0, wastefinish = 0;
	double duration = 0.0;
	int i = 0;

	printf("%s(范围:%d与%d之间):", name, min, max);
	//等待用户输入
	fflush(stdin);
	wastestart = clock();
	while (duration < MAX_WAITING_TIME) {
		if (_kbhit()) {
			scanf_s("%d", &size);
			i = 1;
			break;
		}
		wastefinish = clock();
		duration = (double)(wastefinish - wastestart) / CLOCKS_PER_SEC;
	}

	//判断用户是否输入合适
	if (i == 0) {
		printf("\a用户没有输入内容,使用默认设置\n");
		return -1;
	}

	else
	{
		if (size >= min && size <= max) {
			fflush(stdin);
			return size;
		}
		else {
			printf("\a输入值大于规定值\n");
			fflush(stdin);	
			return input_i(name, min, max);
		}
	}
}

//显示名称，输入值，返回小数
double input_f(char *name, int min, int max) {
	double size = 0;
	clock_t wastestart = 0, wastefinish = 0;
	double duration = 0.0;
	int i = 0;

	printf("%s(范围:%d与%d之间):", name, min, max);
	//等待用户输入
	fflush(stdin);
	wastestart = clock();
	while (duration < MAX_WAITING_TIME) {
		if (_kbhit()) {
			scanf_s("%f", &size);
			i = 1;
			break;
		}
		wastefinish = clock();
		duration = (double)(wastefinish - wastestart) / CLOCKS_PER_SEC;
	}

	//判断用户是否输入合适
	if (i == 0) {
		printf("\a用户没有输入内容,使用默认设置\n");
		return -1;
	}

	else
	{
		if (size <= max && size >= min) {
			fflush(stdin);
			return size;
		}
		else {
			printf("\a输入值大于规定值\n");
			fflush(stdin);

			return input_f(name, min, max);
		}
	}
}

//图形版本初始化 
void initial_graph(void)
{
	IMAGE temp[3];
	IMAGE rotimg;
	loadimage(&temp[0], _T("train1.bmp"));
	loadimage(&temp[1], _T("train2.bmp"));
	loadimage(&temp[2], _T("train3.bmp"));
	loadimage(&rotimg, _T("train4.bmp"));
	for (int k = 0; k < 3; k++)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)

			{
				SetWorkingImage(&temp[k]);
				getimage(&img[k][j][i], i * 48, j * 64, 48, 64);
			}

	T[0].Create(((train[0]->x[0]) + 1) * 100 + 15, (train[0]->y[0]) * 100, 4, -1, true, true, img[0][changeDirection(train[0]->last_direction[0])], WHITE);
	T[3].Create(((train[0]->x[1]) + 1) * 100 + 15, (train[0]->y[1]) * 100, 4, -1, true, true, img[0][changeDirection(train[0]->last_direction[1])], WHITE);
	T[6].Create(((train[0]->x[2]) + 1) * 100 + 15, (train[0]->y[2]) * 100, 4, -1, true, true, img[0][changeDirection(train[0]->last_direction[2])], WHITE);
	//

	T[1].Create(((train[1]->x[0]) + 1) * 100 + 15, (train[1]->y[0]) * 100, 4, -1, true, true, img[1][changeDirection(train[1]->last_direction[0])], WHITE);
	T[4].Create(((train[1]->x[1]) + 1) * 100 + 15, (train[1]->y[1]) * 100, 4, -1, true, true, img[1][changeDirection(train[1]->last_direction[1])], WHITE);
	T[7].Create(((train[1]->x[2]) + 1) * 100 + 15, (train[1]->y[2]) * 100, 4, -1, true, true, img[1][changeDirection(train[1]->last_direction[2])], WHITE);

	//
	T[2].Create(((train[2]->x[0]) + 1) * 100 + 15, (train[2]->y[0]) * 100, 4, -1, true, true, img[2][changeDirection(train[2]->last_direction[0])], WHITE);
	T[5].Create(((train[2]->x[1]) + 1) * 100 + 15, (train[2]->y[1]) * 100, 4, -1, true, true, img[2][changeDirection(train[2]->last_direction[1])], WHITE);
	T[8].Create(((train[2]->x[2]) + 1) * 100 + 15, (train[2]->y[2]) * 100, 4, -1, true, true, img[2][changeDirection(train[2]->last_direction[2])], WHITE);

	AddTrain(T[0]);
	AddTrain(T[1]);
	AddTrain(T[2]);
	AddTrain(T[3]);
	AddTrain(T[4]);
	AddTrain(T[5]);
	AddTrain(T[6]);
	AddTrain(T[7]);
	AddTrain(T[8]);


	initgraph(1600, 800, SHOWCONSOLE);
	IMAGE backbuffer(1600, 800);

	loadimage(&background, _T("beijing.jpg"));

	//-------------------------------
	/*	loadimage(&img1, _T("dialog1.jpg"));
	putimage(0, 0, &img1);
	loadimage(&img2, _T("dialog2.jpg"));
	putimage(0, 300, &img2);
	loadimage(&img3, _T("dialog3.jpg"));
	putimage(600, 500, &img3);*/

	loadimage(&imga1, _T("Atrain1.jpg")); loadimage(&imga2, _T("Atrain2.jpg"));
	loadimage(&imgb1, _T("Btrain1.jpg")); loadimage(&imgb2, _T("Btrain2.jpg"));
	loadimage(&imgc1, _T("Ctrain1.jpg")); loadimage(&imgc2, _T("Ctrain2.jpg"));
	loadimage(&imgg, _T("green.jpg"));
	loadimage(&imgr, _T("red.jpg"));
}

//人工输入
void initial_input(void)
{
	int a = 0;
	char t;
	int temp_i, temp_direction;
	double temp_f;

	a = (( temp_i = input_i("请选择是否人工输入初始数据(人工输入请输入1, 默认设置输入0)", 0, 1)) == -1 ? 0 : temp_i);
	while (a)
	{
		int i;

		printf("请输入初始化的车辆(A/B/C):");
		getchar();
		scanf("%c", &t);
		i = (int)t - 'A';

		if (i > NUM_TRAIN - 1 || i < 0)
		{
			printf("输入错误,请重新输入\n");
			continue;
		}

		temp_direction = input_i("[火车方向]:0 代表顺时针; 1 代表逆时针", 0, 1);
		if (temp_direction != -1)
			train[i]->last_direction[0] = setDirection(train[i]->direction, train[i]->x[0], train[i]->y[0], train[i]->track);

		start_train[i] = ((temp_i = input_i("[火车启动时间]", 0, 15)) == -1 ? start_train[i] : temp_i);

		train[i]->type = ((temp_i = input_i("[火车类型]:0 代表快车; 1 代表慢车", 0, 1)) == -1 ? train[i]->type : temp_i);
		train[i]->speed = ((temp_f = input_f("[火车运行速度]", 0.0, 5.0)) == 0 ? train[i]->speed : temp_f);

		switch (i)
		{
		case 0:
			train[i]->y[0] = ((temp_f = input_f("[火车位置]:纵坐标y", 1, 6)) == -1 ? train[i]->y[0] : temp_f);
			if ((int)train[i]->y[0] == 1 || (int)train[i]->y[0] == 6)
				train[i]->x[0] = ((temp_f = input_f("[火车位置]:横坐标x", 1, probe[0]->x)) == -1 ? train[i]->x[0] : temp_f);
			else
				train[i]->x[0] = ((temp_f = input_f("[火车位置]:横坐标x", 1, 1)) == -1 ? train[i]->x[0] : temp_f);
			break;

		default:
			train[i]->y[0] = ((temp_f = input_f("[火车位置]:纵坐标y", 1, 6)) == -1 ? train[i]->y[0] : temp_f);
			if ((int)train[i]->y[0] == 1 || (int)train[i]->y[0] == 6)
				train[i]->x[0] = ((temp_f = input_f("[火车位置]:横坐标x", probe[1]->x, 11)) == -1 ? train[i]->x[0] : temp_f);
			else
				train[i]->x[0] = ((temp_f = input_f("[火车位置]:横坐标x", 11, 11)) == -1 ? train[i]->x[0] : temp_f);
			break;
		}

		a = ((temp_i = input_i("若初始化其他车辆,请输入1,否则输入0", 0, 1)) == -1 ? 0 : temp_i);
	}
	Sleep(1000);
}

//按文件初始化
void initial_file(void)
{
	//清空之前文件中的数据
	FILE *fp1;
	fp1 = fopen("record_regular.txt", "w");
	fclose(fp1);
	FILE *fp2;
	fp2 = fopen("record_irregular.txt", "w");
	fclose(fp2);

	//初始化各个值
	FILE *fp_train, *fp_probe, *fp_station, *fp_map;

	//初始化火车
	if ((fp_train = fopen("initial_train.txt", "r")) == NULL)
		printf("File open fail\n");
	for (int i = 0; i < NUM_TRAIN; i++) {
		train[i] = (Train*)malloc(sizeof(Train));
		fscanf(fp_train, "%d %d %d %d %d %lf %lf %lf %ld", &train[i]->direction, &train[i]->last_direction, &train[i]->track, &train[i]->type, &train[i]->state, &train[i]->x[0], &train[i]->y[0], &train[i]->speed, &start_train[i]);
		train[i]->last_speed[0] = (train[i]->speed > 1.0 ? 1.0 : train[i]->speed);
		train[i]->last_direction[0] = setDirection(train[i]->direction, train[i]->x[0], train[i]->y[0], train[i]->track);
		acceleration[i] = 0;
		tct[i] = 0;
	}
	fclose(fp_train);

	//初始化探测点
	if ((fp_probe = fopen("initial_probe.txt", "r")) == NULL)
		printf("File open fail\n");
	for (int i = 0; i < NUM_PUBLICTRACK * 4; i++)
	{
		probe[i] = (Probe*)malloc(sizeof(Probe));
		ing_train[i] = (Train*)malloc(sizeof(Train));
		ing_train[i] = NULL;
		fscanf(fp_probe, "%d %d %d", &probe[i]->x, &probe[i]->y, &probe[i]->state);
	}
	fclose(fp_probe);

	//初始化站台
	if ((fp_station = fopen("initial_station.txt", "r")) == NULL)
		printf("File open fail\n");
	for (int i = 0; i < NUM_STATION; i++)
	{
		station[i] = (Station*)malloc(sizeof(Station));
		fscanf(fp_station, "%d %d %ld %d", &station[i]->x, &station[i]->y, &station[i]->time, &station[i]->state);
		station[i]->time *= CLOCKS_PER_SEC;
	}
	fclose(fp_probe);

	//初始化地图
	if ((fp_map = fopen("initial_map.txt", "r")) == NULL)
		printf("File open fail\n");
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 13; j++)
			fscanf(fp_map, "%d", &Map[i][j]);
	fclose(fp_map);

	//初始化公共轨道
	for (int i = 0; i < NUM_PUBLICTRACK; i++)
	{
		publicTrain[i] = (Train*)malloc(sizeof(Train));
		PublicTrack[i] = BLANK;
		strategy[i] = TURN_BY_TURN;
		turn[i] = 1;
	}
}

//初始化方向
int setDirection(int wise, double x, double y, int track)
{
	if (wise == CLOCKWISE)
	{
		if (track == 1)
		{
			switch ((int)y)
			{
			case 1:
				if (x == 1)
					return 0;
				else
					return 1;
				break;

			case 6:
				if (x == 6)
					return 2;
				else
					return 3;
				break;

			default:
				if (x == 1)
					return 0;
				else if (x == 6)
					return 2;
				break;
			}
		}
		else if (track == 2)
		{
			switch ((int)y)
			{
			case 1:
				if (x == 6)
					return 0;
				else
					return 1;
				break;

			case 6:
				if (x == 11)
					return 2;
				else
					return 3;
				break;

			default:
				if (x == 6)
					return 0;
				else if (x == 11)
					return 2;
				break;
			}
		}
	}
	else if (wise == ANTICLOCKWISE)
	{
		if (track == 1)
		{
			switch ((int)y)
			{
			case 1:
				if (x == 6)
					return 0;
				else
					return 3;
				break;

			case 6:
				if (x == 1)
					return 2;
				else
					return 1;
				break;

			default:
				if (x == 1)
					return 2;
				else if (x == 6)
					return 0;
				break;
			}
		}
		else if (track == 2)
		{
			switch ((int)y)
			{
			case 1:
				if (x == 11)
					return 0;
				else
					return 3;
				break;

			case 6:
				if (x == 6)
					return 2;
				else
					return 1;
				break;

			default:
				if (x == 6)
					return 2;
				else if (x == 11)
					return 0;
				break;
			}
		}
	}
}

void initial(void)
{
	int temp_i = 0;
	x = ((temp_i = input_i("请选择命令行版本或者图形版本(图形版本请输入1, 命令行输入0)", 0, 1)) == -1 ? 0 : temp_i);

	//文件初始化
	initial_file();

	//人工初始化
	initial_input();
	
	for (int i = 0; i < NUM_TRAIN; i++)
		setMoreCarriage(train[i]);

	//图像初始化
	if (x == 1)
		initial_graph();

	//按秒计算初始化时间
	for (int i = 0; i < NUM_TRAIN; i++)
		start_train[i] *= CLOCKS_PER_SEC;
}

