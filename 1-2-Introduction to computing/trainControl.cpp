#include "stdafx.h"
#include "train.h"

extern Train *ing_train[NUM_PUBLICTRACK*4];	//准备进入公共轨道的火车
int flag = 0;	//火车状态是否改变
int ing_station[NUM_TRAIN];	//火车正在使用的站台
int flag_station = 0, flag_imergency = 0, flag_probe = 0;
double acceleration[NUM_TRAIN];	//火车加速度
extern Train *publicTrain[NUM_PUBLICTRACK];	//公共轨道的火车
int tct[NUM_TRAIN];	//换轨
int turnaround[NUM_TRAIN] = { 0, 0, 0 };	//掉头

int init_direction[NUM_TRAIN*LENGTH];	//开始方向
double init_displacement[NUM_TRAIN*LENGTH];	//开始位移
int last_dir[NUM_TRAIN*LENGTH];	//结束方向
double last_displacement[NUM_TRAIN*LENGTH];	//结束位移

//初始方向初始化
void init_ds(void)
{
	for (int i = 0; i < NUM_TRAIN; i++)
		for (int j = 0; j < LENGTH; j++)
		{
			init_direction[i + 3 * j] = 0;
			init_displacement[i + 3 * j] = 0;
			last_dir[i + 3 * j] = 0;
			last_displacement[i + 3 * j] = 0;
		}
}

//两个模块的方向转化
int changeDirection(int n)
{
	switch (n)
	{
	case 0:	return UP; break;
	case 1:	return RIGHT; break;
	case 2:	return DOWN; break;
	case 3:	return LEFT; break;
	default: return UP; break;
	}
}

//探测方向
int checkDirection(int n, int x, int y)
{
	switch (n)
	{
	case 0: return Map[y - 1][x];
	case 1: return Map[y][x + 1];
	case 2: return Map[y + 1][x];
	case 3: return Map[y][x - 1];
	default: return -1;
	}
}

//确定下一次的方向
int Direction(Train *strain)
{
	//头车厢寻找路线 
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		if (checkDirection(i, (int)strain->x[0], (int)strain->y[0]) == strain->track || checkDirection(i, (int)strain->x[0], (int)strain->y[0]) == PUBLICTRACK)
			if (i != (strain->last_direction[0] + 2) % 4)
				return i;
	}
	//未能找到路
	if (i == 4)
		strain->state = CRASH;
	return i;
}

//火车移动
void Move(Train *strain, double speed, int n)
{
	int j;
	//后一个车厢跟着前一个车厢走 
	for (j = LENGTH - 1; j > 0; j--)
	{
		strain->x[j] += strain->last_speed[j - 1] * direction[strain->last_direction[j - 1]][0];
		strain->y[j] += strain->last_speed[j - 1] * direction[strain->last_direction[j - 1]][1];

		strain->last_direction[j] = strain->last_direction[j - 1];
		strain->last_speed[j] = strain->last_speed[j - 1];

		//传递数值的增减
		if (train[n]->last_direction[j] == init_direction[n + 3 * j])
			init_displacement[n + 3 * j] += strain->last_speed[j - 1];
		else
			last_displacement[n + 3 * j] += strain->last_speed[j - 1];

	}

	int i;
	i = Direction(strain);

	strain->x[0] += speed * direction[i][0];
	strain->y[0] += speed * direction[i][1];
	strain->last_direction[0] = i;
	strain->last_speed[0] = speed;

	//传递数值的增减
	if (train[n]->last_direction[0] == init_direction[n + 3 * 0])
		init_displacement[n + 3 * j] += speed;
	else
		last_displacement[n + 3 * j] += speed;

}

//变轨
void track_Change(void)
{
	for (int i = 0; i < NUM_TRAIN; i++)
	{
		if (tct[i] != 0)
		{
			if ((int)train[i]->x[0] == 6)
			{
				if (train[i]->track == 1)
					train[i]->track = 2;
				else
					train[i]->track = 1;
				train[i]->direction = (train[i]->direction + 1) % 2;
				tct[i] = 0;
			}
		}
		else;
	}
}

//查看火车前方的安全
int checkSecurity(Train *strain)
{
	//寻找方向
	int d;
	d = Direction(strain);

	for (int i = 0; i < NUM_TRAIN; i++)
	{
		if (strain != train[i]) {
			//前方有车辆,紧急停靠
			for (int j = 0; j < LENGTH; j++)
			{
				if ((int)(strain->x[0] + direction[d][0]) == (int)train[i]->x[j])
					if ((int)(strain->y[0] + direction[d][1]) == (int)train[i]->y[j])
						return STOP_EMERGENCY;
			}
		}
		else;
	}
	return strain->state;
}

//查看站台
int checkStation(Train *strain, int n)
{
	int i = 0;
	while (i < NUM_STATION)
	{
		if ((int)strain->x[0] == station[i]->x && (int)strain->y[0] == station[i]->y)
		{
			if (x == 1) {

				if (drawdialogbox(n))
				{
					ing_station[n] = i;
					station[i]->state = BUSY;
					station_time[i] = time_begin[n] = clock();
					time_held[n] = station[i]->time;
					return STOP_STATION;
				}
			}

			else
			{
				ing_station[n] = i;
				station[i]->state = BUSY;
				time_begin[n] = clock();
				time_held[n] = station[i]->time;
				return STOP_STATION;
			}
				
		}
		else;
		i++;
	}
	return strain->state;
}

//查看探测点
void checkProbe(Train *strain)
{
	for (int j = 0; j < NUM_PUBLICTRACK; j++)
	{
		int i = 0;
		while (i < NUM_PUBLICTRACK * 4) {
			//遇到站台
			if (probe[i + 4*j]->x == (int)strain->x[0] && probe[i + 4 * j]->y == (int)strain->y[0])
			{
				//出公共轨道 
				if (strain == publicTrain[j]) {
					PublicTrack[j] = BLANK;
					publicTrain[j] = NULL;
					probe[0+4*j]->state = GREEN; probe[1+4*j]->state = GREEN; probe[2+4*j]->state = GREEN; probe[3+4*j]->state = GREEN;
				}
				//准备进公共轨道
				else {
					if (probe[i+4*j] == RED)
						strain->state = STOP_PROBE;
					else if (ing_train[i+4*j] == NULL)
						ing_train[i+4*j] = strain;
					else
						strain->state = STOP_PROBE;
				}
			}
			else;

			if (strain == ing_train[i+4*j]) {
				//准备进入公共轨道
				if (Map[(int)strain->y[0]][(int)strain->x[0]] == PUBLICTRACK)
				{
					for (int k = 0; k < NUM_TRAIN; k++)
						if (strain == train[k])
							PublicTrack[j] = BLANK + k + 1;

					//进入轨道之后交换交替策略的值
					if ((ing_train[1+4*j] != NULL && ing_train[0+4*j] != NULL) || (ing_train[1+4*j] != NULL && ing_train[2+4*j] != NULL))
						turn[j] = !turn[j];
					else if((ing_train[3 + 4 * j] != NULL && ing_train[0 + 4 * j] != NULL) || (ing_train[3 + 4 * j] != NULL && ing_train[2 + 4 * j] != NULL))
						turn[j] = !turn[j];
					else;

					ing_train[i+4*j] = NULL;
					publicTrain[j] = strain;
					probe[0+4*j]->state = RED; probe[1+4*j]->state = RED; probe[2+4*j]->state = RED; probe[3+4*j]->state = RED;
				}
				else;
			}
			else;
			i++;
		}
	}
}

void checkProbe1(Train *strain)
{
	for (int j = 0; j < NUM_PUBLICTRACK; j++)
	{
		int i = 0;
		while (i < NUM_PUBLICTRACK * 4) {
			if (strain == ing_train[i + 4 * j]) {
				//准备进入公共轨道
				if (Map[(int)strain->y[0]][(int)strain->x[0]] == PUBLICTRACK)
				{
					for (int k = 0; k < NUM_TRAIN; k++)
						if (strain == train[k])
							PublicTrack[j] = BLANK + k + 1;

					//进入轨道之后交换交替策略的值
					if ((ing_train[1 + 4 * j] != NULL && ing_train[0 + 4 * j] != NULL) || (ing_train[1 + 4 * j] != NULL && ing_train[2 + 4 * j] != NULL))
						turn[j] = !turn[j];
					else if ((ing_train[3 + 4 * j] != NULL && ing_train[0 + 4 * j] != NULL) || (ing_train[3 + 4 * j] != NULL && ing_train[2 + 4 * j] != NULL))
						turn[j] = !turn[j];
					else;

					ing_train[i + 4 * j] = NULL;
					publicTrain[j] = strain;
					probe[0 + 4 * j]->state = RED; probe[1 + 4 * j]->state = RED; probe[2 + 4 * j]->state = RED; probe[3 + 4 * j]->state = RED;
				}
				else;
			}
			else;
			i++;
		}
	}
}

//公共轨道控制
void probeControl(void)
{
	//多条公共轨道
	for (int k = 0; k < NUM_PUBLICTRACK; k++)
	{
		//公共轨道有车 
		if (PublicTrack[k] != BLANK)
		{
			int i = k * 4;
			while (i < (NUM_PUBLICTRACK + k) * 4) {
				if (ing_train[i] != NULL)
					ing_train[i]->state = STOP_PROBE;
				i++;
			}
		}

		else
		{
			flag_probe = 0;
			switch (strategy[k])
			{
			case TURN_BY_TURN:
				//同时有两辆车准备进入公共轨道
				for (int i = 1; i < 4; i += 2)
				{
					if (ing_train[0 + 4 * k] != NULL && ing_train[i + 4 * k] != NULL) {
						if (turn[k] == 0) {
							ing_train[0 + 4 * k]->state = RUN;
							ing_train[i + 4 * k]->state = STOP_PROBE;
						}
						else {
							ing_train[0 + 4 * k]->state = STOP_PROBE;
							ing_train[i + 4 * k]->state = RUN;
						}
						flag_probe = 1;
					}
					else if (ing_train[2 + 4 * k] != NULL && ing_train[i + 4 * k] != NULL) {
						if (turn[k] == 0) {
							ing_train[2 + 4 * k]->state = RUN;
							ing_train[i + 4 * k]->state = STOP_PROBE;
						}
						else {
							ing_train[2 + 4 * k]->state = STOP_PROBE;
							ing_train[i + 4 * k]->state = RUN;
						}
						flag_probe = 1;
					}
				}

			case MAN_COMMAND:
				for (int i = 1; i < 4; i += 2)
				{
					if (ing_train[0 + 4 * k] != NULL && ing_train[i + 4 * k] != NULL) {
						if (ing_train[0 + 4 * k]->state + ing_train[i + 4 * k]->state == 0 || (ing_train[0 + 4 * k]->state == STOP_PROBE && ing_train[i + 4 * k]->state == STOP_PROBE))
							manselect(ing_train[0 + 4 * k], ing_train[i + 4 * k]);
						flag_probe = 1;
					}

					else if (ing_train[2 + 4 * k] != NULL && ing_train[i + 4 * k] != NULL) {
						if (ing_train[2 + 4 * k]->state + ing_train[i + 4 * k]->state == 0 || (ing_train[2 + 4 * k]->state == STOP_PROBE && ing_train[i + 4 * k]->state == STOP_PROBE))
							manselect(ing_train[2 + 4 * k], ing_train[i + 4 * k]);
						flag_probe = 1;
					}

				}

			case FAST_FIRST:
				//同时有两辆车准备进入公共轨道
				for (int i = 1; i < 4; i += 2)
				{
					if (ing_train[0 + 4 * k] != NULL && ing_train[i + 4 * k] != NULL) {
						if (ing_train[0 + 4 * k]->speed > ing_train[i + 4 * k]->speed) {
							ing_train[0 + 4 * k]->state = RUN;
							ing_train[i + 4 * k]->state = STOP_PROBE;
						}
						else {
							ing_train[0 + 4 * k]->state = STOP_PROBE;
							ing_train[i + 4 * k]->state = RUN;
						}
						flag_probe = 1;
					}
					else if (ing_train[2 + 4 * k] != NULL && ing_train[i + 4 * k] != NULL) {
						if (ing_train[2 + 4 * k]->speed > ing_train[i + 4 * k]->speed) {
							ing_train[2 + 4 * k]->state = RUN;
							ing_train[i + 4 * k]->state = STOP_PROBE;
						}
						else {
							ing_train[2 + 4 * k]->state = STOP_PROBE;
							ing_train[i + 4 * k]->state = RUN;
						}
						flag_probe = 1;
					}
				}
			}
			//公共轨道变为空闲
			if (flag_probe == 0) {
				for (int i = 4 * k; i < (NUM_PUBLICTRACK + k) * 4; i++) {
					if (ing_train[i + 4 * k] != NULL &&ing_train[i + 4 * k]->state == STOP_PROBE) {
						ing_train[i + 4 * k]->state = RUN;
					}
				}
			}
			break;
		}
	}
}

//火车移动
void trainMove(void)
{
//	while (1)
//	{
		double i;
		int j;	//火车的标号
		init_ds();

		for (j = 0; j < NUM_TRAIN; )
		{
			for (int k = 0; k < LENGTH; k++)
				init_direction[j + 3 * k] = train[j]->last_direction[k];

			train[j]->speed += acceleration[j];
			acceleration[j] = 0;

			switch (train[j]->state)
			{
			case RUN:
				i = train[j]->speed;
				while (i > 1.0)
				{
					if(!flag_station) 
						train[j]->state = checkStation(train[j], j);	//检测站台
					flag_station = 0;

					checkProbe(train[j]);							//检测探测点
					probeControl();									//交通管理
					train[j]->state = checkSecurity(train[j]);		//查看安全

					//如有情况发生
					if (train[j]->state != RUN) {
						flag = 1;
						break;
					}

					Move(train[j], 1.0, j);

					i -= 1.0;
					checkProbe1(train[j]);
				}
				//如有情况发生,跳过剩下的行驶
				if (flag == 1)
				{
					flag = 0;
					break;
				}
				//解决剩余速度
				else
				{
					if (!flag_station)
						train[j]->state = checkStation(train[j], j);	//检测站台
					flag_station = 0;

					checkProbe(train[j]);
					probeControl();
					train[j]->state = checkSecurity(train[j]);
					if (train[j]->state != RUN)
						break;

					Move(train[j], i, j);
					checkProbe1(train[j]);
				}
				break;

			case STOP_EMERGENCY:
				if (checkSecurity(train[j]) == STOP_EMERGENCY)
				{
					train[j]->state = RUN;
					flag_imergency = 1;
					continue;
				}
				break;

			case STOP_PROBE:
				checkProbe(train[j]);
				probeControl();
				if (train[j]->state != STOP_PROBE)
					continue;
				break;

			case STOP_STATION:
				if (clock() - time_begin[j] >= time_held[j]) {
					flag_station = 1;
					train[j]->state = RUN;
					station[ing_station[j]]->state = FREE;
					continue;
				}
				break;

			case STOP_MANCONTROL:
				break;

			case STOP_START:
				now = clock();
				if (now - finish_initial > start_train[j])
				{
					train[j]->state = RUN;
					continue;
				}
				break;

			case CRASH:
				printf("事件:火车%c发生碰撞!", j + 'A');
				Sleep(5000);
				exit(0);
				break;
			}

			for (int k = 0; k < LENGTH; k++)
				last_dir[j + 3 * k] = train[j]->last_direction[k];
			j++;
		}
		probeControl();
//		Sleep(1000);
//	}
//	_endthreadex(0);
//	return 0;
}

unsigned __stdcall move(void* pArgumemt)
{
	IMAGE backbuffer(1600, 800);

	while (1)
	{
		//掉头
		for (int i = 0; i < NUM_TRAIN; i++) {
			if (turnaround[i] != 0) {
				turnback(train[i]);

				if (x == 1) {
					MoveTo(i, (int)((train[i]->x[0]) + 1) * 100 + 15, (int)(train[i]->y[0]) * 100);
					MoveTo(i + 6, (int)((train[i]->x[2]) + 1) * 100 + 15, (int)(train[i]->y[2]) * 100);
					
					for (int k = 0; k < NUM_TRAIN; k++)
					{
						Train_Reset(i + k * 3, img[i][changeDirection(train[i]->last_direction[k])], -1);
					}
				}

				turnaround[i] = 0;
			}
		}

		trainMove();

		//图形化版本
		if (x == 1)
		{
			for (int i = 0; i < 100; i++)
			{
				track_Change();
				SetWorkingImage(&backbuffer);
				cleardevice();
				putimage(0, 0, &background);
				drawtrainmsg();
				RunTrain();

				for (int j = 0; j < NUM_TRAIN; j++)
				{
					for (int k = 0; k < LENGTH; k++)
						ChangeState(j + k * 3, STOP_MANCONTROL);

					for (int k = 0; k < LENGTH; k++)
					{

						//动画转身
						if (init_displacement[j + k * 3] <= 0.00 && last_displacement[j + k * 3] >= 0.00)
						{
							Train_Reset(j + k * 3, img[j][changeDirection(train[j]->last_direction[k])], -1);
						}

						//延初始方向移动
						if (init_displacement[j + k * 3] > 0.00)
						{
							ChangeState(j + k * 3, RUN);
							switch (changeDirection(init_direction[j+3*k]))
							{
							case DOWN:
								TMove(j + k * 3, 0, (int)train[j]->speed);
								T[j + k * 3].x += 0;
								T[j + k * 3].y += (int)train[j]->speed;
								break;
							case LEFT:
								TMove(j + k * 3, (int)-(train[j]->speed), 0);
								T[j + k * 3].x += (int)-(train[j]->speed);
								T[j + k * 3].y += 0;
								break;
							case RIGHT:
								TMove(j + k * 3, (int)train[j]->speed, 0);
								T[j + k * 3].x += (int)train[j]->speed;
								T[j + k * 3].y += 0;
								break;
							case UP:
								TMove(j + k * 3, 0, (int)-(train[j]->speed));
								T[j + k * 3].x += 0;
								T[j + k * 3].y += (int)-(train[j]->speed);
								break;
							default:
								break;
							}

							init_displacement[j + k * 3] -= train[j]->speed * 0.01;
						}
						//延变轨后方向移动
						else if(init_displacement[j + k * 3] <= 0.00 && last_displacement[j + k * 3] > 0.00)
						{
							ChangeState(j + k * 3, RUN);
							switch (changeDirection(last_dir[j + 3 * k]))
							{
							case DOWN:
								TMove(j + k * 3, 0, (int)train[j]->speed);
								T[j + k * 3].x += 0;
								T[j + k * 3].y += (int)train[j]->speed;
								break;
							case LEFT:
								TMove(j + k * 3, (int)-(train[j]->speed), 0);
								T[j + k * 3].x += (int)-(train[j]->speed);
								T[j + k * 3].y += 0;
								break;
							case RIGHT:
								TMove(j + k * 3, (int)train[j]->speed, 0);
								T[j + k * 3].x += (int)train[j]->speed;
								T[j + k * 3].y += 0;
								break;
							case UP:
								TMove(j + k * 3, 0, (int)-(train[j]->speed));
								T[j + k * 3].x += 0;
								T[j + k * 3].y += (int)-(train[j]->speed);
								break;
							default:
								break;
							}

							last_displacement[j + k * 3] -= train[j]->speed * 0.01;
						}
					}
				}

				SetWorkingImage();
				putimage(0, 0, &backbuffer);
				Sleep(40);
			}
		}

		//命令行版本
		else
			Sleep(2000);
		
	}
//	_endthreadex(0);
//	return 0;
}