#include "stdafx.h"
#include "train.h"


//用户输入改变火车状态的线程， a为火车加速， s为火车减速， p为火车停靠， c为火车继续运行
//命令行输入
unsigned int __stdcall Input_Command(void* pArgument)
{
	FILE *fp1, *fp;

	int i = -1;
	char operate, name;
	getchar();
	_fflush_nolock(stdin);

	while (1)
	{
		//命令行输入
		if (_kbhit())
		{
			operate = getchar();
			SuspendThread(thread[0]);
			SuspendThread(thread[1]);
			SuspendThread(thread[2]);
			name = getchar();

			//判断输入正确性
			i = (int)name - 'A';
			if (i < 0 || i > NUM_TRAIN - 1 || operate < 'A' || operate > 'Z')
			{
				printf("输入错误!\n");
				_fflush_nolock(stdin);
			}

			else {
				if ((fp = fopen("record_irregular.txt", "a")) == NULL)
					printf("File open fail\n");

				switch (operate)
				{
				case 'P':
					train[i]->state = STOP_MANCONTROL;
					fprintf(fp, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "事件:人工停止火车%c\n\n", name);
					printf("事件:人工停止火车%c\n", name);
					break;

				case 'C':
					train[i]->state = RUN;
					fprintf(fp, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "事件:人工启动火车%c\n\n", name);
					printf("事件:人工启动火车%c\n", name);
					break;

				case 'A':
					train[i]->speed += 1.0;
					fprintf(fp, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "事件:火车%c加速\n\n", name);
					printf("事件:火车%c加速\n", name);
					break;

				case 'S':
					train[i]->speed -= 1.0;
					fprintf(fp, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "事件:火车%c减速\n\n", name);
					printf("事件:火车%c减速\n", name);
					break;

				case 'T':
					tct[i] = 1;
					now = clock();
					fprintf(fp, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "事件:火车%c变轨\n\n", name);
					printf("事件:火车%c变轨\n", name);
					break;

				case 'B':
					turnaround[i] = 1;
					now = clock();
					fprintf(fp, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "事件:火车%c掉头\n\n", name);
					printf("事件:火车%c掉头\n", name);
					break;

				

				default:
					printf("输入错误!\n");
					break;
				}
				fclose(fp);

				getchar();
				_fflush_nolock(stdin);
				i = -1;
			}
			ResumeThread(thread[0]);
			ResumeThread(thread[1]);
			ResumeThread(thread[2]);
		}
	}
	
	_endthreadex(0);
	return 0;
}

//文件输入
unsigned int __stdcall Input_file(void* pArgument)
{
	FILE *fp1, *fp2;

	if ((fp1 = fopen("input.txt", "r")) == NULL)
		printf("File open fail\n");

	int i = -1;
	char operate, name;

	while (1)
	{
		//文件输入
		if (fscanf(fp1, "%c%c", &operate, &name) != EOF)
		{
			fscanf(fp1, "%c%c", &operate, &name);
			i = (int)name - 'A';
			if (i < 0 || i > NUM_TRAIN - 1 || operate < 'A' || operate > 'Z')
			{
				printf("输入错误!\n");
			}

			else {
				if ((fp2 = fopen("record_irregular.txt", "a")) == NULL)
					printf("File open fail\n");

				//不定时输出
				switch (operate)
				{
				case 'P':
					train[i]->state = STOP_MANCONTROL;
					now = clock();
					fprintf(fp2, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "事件:人工停止火车%c\n\n", name);
					printf("事件:人工停止火车%c\n", name);
					break;

				case 'C':
					train[i]->state = RUN;
					now = clock();
					fprintf(fp2, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "事件:人工启动火车%c\n\n", name);
					printf("事件:人工启动火车%c\n", name);
					break;

				case 'A':
					train[i]->speed +=
						0;
					now = clock();
					fprintf(fp2, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "事件:火车%c加速\n\n", name);
					printf("事件:火车%c加速\n", name);
					break;

				case 'S':
					train[i]->speed -= 1.0;
					now = clock();
					fprintf(fp2, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "事件:火车%c减速\n\n", name);
					printf("事件:火车%c减速\n", name);
					break;

				case 'T':
					tct[i] = 1;
					now = clock();
					fprintf(fp2, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "事件:火车%c变轨\n\n", name);
					printf("事件:火车%c变轨\n", name);
					break;

				case 'B':
					turnaround[i] = 1;
					now = clock();
					fprintf(fp2, "当前时间:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "事件:火车%c掉头\n\n", name);
					printf("事件:火车%c掉头\n", name);
					break;

				default:
					printf("输入错误!\n");
					fprintf(fp2, "\n");
					break;
				}
				fclose(fp1);
				fp1 = fopen("input.txt", "w");
				fclose(fp1);

				fclose(fp2);

				if ((fp1 = fopen("input.txt", "r")) == NULL)
					printf("File open fail\n");

				i = -1;
			}
		}
	}
	fclose(fp1);
	_endthreadex(0);
	return 0;
}