#include "stdafx.h"
#include "train.h"


//�û�����ı��״̬���̣߳� aΪ�𳵼��٣� sΪ�𳵼��٣� pΪ��ͣ���� cΪ�𳵼�������
//����������
unsigned int __stdcall Input_Command(void* pArgument)
{
	FILE *fp1, *fp;

	int i = -1;
	char operate, name;
	getchar();
	_fflush_nolock(stdin);

	while (1)
	{
		//����������
		if (_kbhit())
		{
			operate = getchar();
			SuspendThread(thread[0]);
			SuspendThread(thread[1]);
			SuspendThread(thread[2]);
			name = getchar();

			//�ж�������ȷ��
			i = (int)name - 'A';
			if (i < 0 || i > NUM_TRAIN - 1 || operate < 'A' || operate > 'Z')
			{
				printf("�������!\n");
				_fflush_nolock(stdin);
			}

			else {
				if ((fp = fopen("record_irregular.txt", "a")) == NULL)
					printf("File open fail\n");

				switch (operate)
				{
				case 'P':
					train[i]->state = STOP_MANCONTROL;
					fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "�¼�:�˹�ֹͣ��%c\n\n", name);
					printf("�¼�:�˹�ֹͣ��%c\n", name);
					break;

				case 'C':
					train[i]->state = RUN;
					fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "�¼�:�˹�������%c\n\n", name);
					printf("�¼�:�˹�������%c\n", name);
					break;

				case 'A':
					train[i]->speed += 1.0;
					fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "�¼�:��%c����\n\n", name);
					printf("�¼�:��%c����\n", name);
					break;

				case 'S':
					train[i]->speed -= 1.0;
					fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "�¼�:��%c����\n\n", name);
					printf("�¼�:��%c����\n", name);
					break;

				case 'T':
					tct[i] = 1;
					now = clock();
					fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "�¼�:��%c���\n\n", name);
					printf("�¼�:��%c���\n", name);
					break;

				case 'B':
					turnaround[i] = 1;
					now = clock();
					fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "�¼�:��%c��ͷ\n\n", name);
					printf("�¼�:��%c��ͷ\n", name);
					break;

				

				default:
					printf("�������!\n");
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

//�ļ�����
unsigned int __stdcall Input_file(void* pArgument)
{
	FILE *fp1, *fp2;

	if ((fp1 = fopen("input.txt", "r")) == NULL)
		printf("File open fail\n");

	int i = -1;
	char operate, name;

	while (1)
	{
		//�ļ�����
		if (fscanf(fp1, "%c%c", &operate, &name) != EOF)
		{
			fscanf(fp1, "%c%c", &operate, &name);
			i = (int)name - 'A';
			if (i < 0 || i > NUM_TRAIN - 1 || operate < 'A' || operate > 'Z')
			{
				printf("�������!\n");
			}

			else {
				if ((fp2 = fopen("record_irregular.txt", "a")) == NULL)
					printf("File open fail\n");

				//����ʱ���
				switch (operate)
				{
				case 'P':
					train[i]->state = STOP_MANCONTROL;
					now = clock();
					fprintf(fp2, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "�¼�:�˹�ֹͣ��%c\n\n", name);
					printf("�¼�:�˹�ֹͣ��%c\n", name);
					break;

				case 'C':
					train[i]->state = RUN;
					now = clock();
					fprintf(fp2, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "�¼�:�˹�������%c\n\n", name);
					printf("�¼�:�˹�������%c\n", name);
					break;

				case 'A':
					train[i]->speed +=
						0;
					now = clock();
					fprintf(fp2, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "�¼�:��%c����\n\n", name);
					printf("�¼�:��%c����\n", name);
					break;

				case 'S':
					train[i]->speed -= 1.0;
					now = clock();
					fprintf(fp2, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "�¼�:��%c����\n\n", name);
					printf("�¼�:��%c����\n", name);
					break;

				case 'T':
					tct[i] = 1;
					now = clock();
					fprintf(fp2, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "�¼�:��%c���\n\n", name);
					printf("�¼�:��%c���\n", name);
					break;

				case 'B':
					turnaround[i] = 1;
					now = clock();
					fprintf(fp2, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp2, "�¼�:��%c��ͷ\n\n", name);
					printf("�¼�:��%c��ͷ\n", name);
					break;

				default:
					printf("�������!\n");
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