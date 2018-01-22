#include "stdafx.h"
#include "train.h"

//定时输出（文件和命令行）
unsigned int __stdcall Output(void* pArgument)
{
	FILE *fp;
	if ((fp = fopen("record_regular.txt", "a")) == NULL)
		printf("File open fail\n");

	while (1)
	{
		now = clock();
		double tf = (double)(now - finish_initial) / CLOCKS_PER_SEC;

		if (tf - (double)(int)tf < EXP)
		{
//			if(x == 1)
//				drawtrainmsg();
			system("CLS");

			//打印当前时间
			printf("当前时间:%4.1f", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);

			//打印轨道
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 13; j++)
				{
					int flag = 0;
					for (int k = 0; k < NUM_TRAIN; k++)
						for (int m = 0; m < LENGTH; m++)
							if ((int)train[k]->x[m] == j && (int)train[k]->y[m] == i) {
								printf("%c", k + 'A');
								flag = 1;
								break;
							}
					for (int k = 0; k < NUM_STATION; k++)
						if (flag != 1)
							if (station[k]->x == j && station[k]->y == i) {
								printf("@");
								flag = 1;
								break;
							}
					if (flag != 1)
						for (int k = 0; k < NUM_PUBLICTRACK * 4; k++) {
							if (probe[k]->x == j && probe[k]->y == i) {
								printf("*");
								flag = 1;
								break;
							}
						}
					if (flag)
						continue;
					switch (Map[i][j]) {
					case 0: printf(" "); break;
					default: printf("."); break;
					}
				}
				printf("\n");
			}

			//打印火车状态
			for (int i = 0; i < NUM_TRAIN; i++)
			{
				printf("火车%c:\t坐标x:%4.1f y:%4.1f\t速度:%4.1f\t方向:", i + 'A', train[i]->x[0], train[i]->y[0], train[i]->speed);
				fprintf(fp, "火车%c:\t坐标x:%4.1f y:%4.1f\t速度:%4.1f\t方向:", i + 'A', train[i]->x[0], train[i]->y[0], train[i]->speed);

				switch (train[i]->direction)
				{
				case CLOCKWISE: printf("顺时针行驶\t"); fprintf(fp, "顺时针行驶\t"); break;
				case ANTICLOCKWISE: printf("逆时针行驶\t"); fprintf(fp, "逆时针行驶\t"); break;
				}

				switch (train[i]->state)
				{
				case RUN: printf("\t状态:正常行驶\n"); fprintf(fp, "\t状态:正常行驶\n"); break;
				case STOP_EMERGENCY: printf("\t状态:紧急停靠\n"); fprintf(fp, "\t状态:紧急停靠\n"); break;
				case STOP_MANCONTROL:printf("\t状态:人工停靠\n"); fprintf(fp, "\t状态:人工停靠\n"); break;
				case STOP_PROBE: printf("\t状态:等待进入公共轨道\n"); fprintf(fp, "\t状态:等待进入公共轨道\n"); break;
				case STOP_STATION: printf("\t状态:站台停靠\n"); fprintf(fp, "\t状态:站台停靠\n"); break;
				case STOP_START: printf("\t状态:等待启动\n");  fprintf(fp, "\t状态:等待启动\n"); break;
				case CRASH: printf("\t状态:发生事故!\n"); fprintf(fp, "\t状态:发生事故!\n"); break;
				}
			}

			//打印检测点状态
			for (int i = 0; i < NUM_PUBLICTRACK * 4; i++)
			{
				printf("检测点%d状态:", i + 1);
				fprintf(fp, "检测点%d状态:", i + 1);
				switch (probe[i]->state)
				{
				case GREEN: printf("绿灯\t"); fprintf(fp, "顺时针行驶\t"); break;
				case RED: printf("红灯\t"); fprintf(fp, "顺时针行驶\t"); break;
				}
			}
			printf("\n");
			fprintf(fp, "\n");

			//打印公共轨道状态
			for (int k = 0; k < NUM_PUBLICTRACK; k++)
			{
				switch (PublicTrack[k])
				{
				case BLANK: printf("公共轨道%d状态:空闲\t", k + 1); fprintf(fp, "公共轨道%d状态:空闲\t", k + 1); break;
				default: printf("公共轨道%d状态:%c车占用\t", k + 1, 'A' + PublicTrack[k] - 1); fprintf(fp, "公共轨道%d状态:%c车占用\t", k + 1, 'A' + PublicTrack[k] - 1); break;
				}
				switch (strategy[k])
				{
				case TURN_BY_TURN: printf("公共轨道%d策略:交替策略\n", k + 1); fprintf(fp, "公共轨道%d策略:交替策略\n", k + 1); break;
				case MAN_COMMAND: printf("公共轨道%d策略:人工策略\n", k + 1); fprintf(fp, "公共轨道%d策略:人工策略\n", k + 1); break;
				case FAST_FIRST: printf("公共轨道%d策略:快车先行策略\n", k + 1); fprintf(fp, "公共轨道%d策略:快车先行策略\n", k + 1); break;
				}
			}

			//操作
			printf("请输出操作(例如,PA代表暂停A.P为暂停,C为启动,A为加速,S为减速;共ABC三辆车):");

			
			for (int i = 0; i < NUM_TRAIN; i++) {
				fprintf(fp, "train %c", i + 'A');
				for (int j = 0; j < LENGTH; j++)
					fprintf(fp, "(%d,%d)\t", T[i + 3 * j].x, T[i + 3 * j].y);
				fprintf(fp, "\n");
			}
			

			fprintf(fp, "\n\n");

		}
	}
	fclose(fp);
	_endthreadex(0);
	return 0;
}