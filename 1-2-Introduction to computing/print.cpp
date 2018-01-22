#include "stdafx.h"
#include "train.h"

//��ʱ������ļ��������У�
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

			//��ӡ��ǰʱ��
			printf("��ǰʱ��:%4.1f", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);

			//��ӡ���
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

			//��ӡ��״̬
			for (int i = 0; i < NUM_TRAIN; i++)
			{
				printf("��%c:\t����x:%4.1f y:%4.1f\t�ٶ�:%4.1f\t����:", i + 'A', train[i]->x[0], train[i]->y[0], train[i]->speed);
				fprintf(fp, "��%c:\t����x:%4.1f y:%4.1f\t�ٶ�:%4.1f\t����:", i + 'A', train[i]->x[0], train[i]->y[0], train[i]->speed);

				switch (train[i]->direction)
				{
				case CLOCKWISE: printf("˳ʱ����ʻ\t"); fprintf(fp, "˳ʱ����ʻ\t"); break;
				case ANTICLOCKWISE: printf("��ʱ����ʻ\t"); fprintf(fp, "��ʱ����ʻ\t"); break;
				}

				switch (train[i]->state)
				{
				case RUN: printf("\t״̬:������ʻ\n"); fprintf(fp, "\t״̬:������ʻ\n"); break;
				case STOP_EMERGENCY: printf("\t״̬:����ͣ��\n"); fprintf(fp, "\t״̬:����ͣ��\n"); break;
				case STOP_MANCONTROL:printf("\t״̬:�˹�ͣ��\n"); fprintf(fp, "\t״̬:�˹�ͣ��\n"); break;
				case STOP_PROBE: printf("\t״̬:�ȴ����빫�����\n"); fprintf(fp, "\t״̬:�ȴ����빫�����\n"); break;
				case STOP_STATION: printf("\t״̬:վ̨ͣ��\n"); fprintf(fp, "\t״̬:վ̨ͣ��\n"); break;
				case STOP_START: printf("\t״̬:�ȴ�����\n");  fprintf(fp, "\t״̬:�ȴ�����\n"); break;
				case CRASH: printf("\t״̬:�����¹�!\n"); fprintf(fp, "\t״̬:�����¹�!\n"); break;
				}
			}

			//��ӡ����״̬
			for (int i = 0; i < NUM_PUBLICTRACK * 4; i++)
			{
				printf("����%d״̬:", i + 1);
				fprintf(fp, "����%d״̬:", i + 1);
				switch (probe[i]->state)
				{
				case GREEN: printf("�̵�\t"); fprintf(fp, "˳ʱ����ʻ\t"); break;
				case RED: printf("���\t"); fprintf(fp, "˳ʱ����ʻ\t"); break;
				}
			}
			printf("\n");
			fprintf(fp, "\n");

			//��ӡ�������״̬
			for (int k = 0; k < NUM_PUBLICTRACK; k++)
			{
				switch (PublicTrack[k])
				{
				case BLANK: printf("�������%d״̬:����\t", k + 1); fprintf(fp, "�������%d״̬:����\t", k + 1); break;
				default: printf("�������%d״̬:%c��ռ��\t", k + 1, 'A' + PublicTrack[k] - 1); fprintf(fp, "�������%d״̬:%c��ռ��\t", k + 1, 'A' + PublicTrack[k] - 1); break;
				}
				switch (strategy[k])
				{
				case TURN_BY_TURN: printf("�������%d����:�������\n", k + 1); fprintf(fp, "�������%d����:�������\n", k + 1); break;
				case MAN_COMMAND: printf("�������%d����:�˹�����\n", k + 1); fprintf(fp, "�������%d����:�˹�����\n", k + 1); break;
				case FAST_FIRST: printf("�������%d����:�쳵���в���\n", k + 1); fprintf(fp, "�������%d����:�쳵���в���\n", k + 1); break;
				}
			}

			//����
			printf("���������(����,PA������ͣA.PΪ��ͣ,CΪ����,AΪ����,SΪ����;��ABC������):");

			
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