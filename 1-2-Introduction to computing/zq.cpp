#include "stdafx.h"
#include "train.h"

LOGFONT textstyle1 = { 30,20,0,0,0,0,0,0,0,0,0,0,0,0,0 };
LOGFONT textstyle2 = { 20,10,0,0,0,0,0,0,0,0,0,0,0,0,0 };
LOGFONT textstyle3 = { 15,8,0,0,0,0,0,0,0,0,0,0,0,0,0 };
IMAGE imgg, imgr;

int selecttrain = 0;
time_t station_time[NUM_STATION];	//վ̨��ͣ��ʱ��

MOUSEMSG u;	//ȫ�����

void drawbutton()
{

	settextstyle(&textstyle1);
	settextcolor(RGB(8,87,107));
	setbkmode(TRANSPARENT);//͸������
	if(selecttrain ==0)settextcolor(RGB(117,12,156));outtextxy(70, 144, 'A');settextcolor(RGB(41,94,173));
	if(selecttrain ==1)settextcolor(RGB(117,12,156));outtextxy(70, 335, 'B');settextcolor(RGB(41,94,173));
	if(selecttrain ==2)settextcolor(RGB(117,12,156));outtextxy(70, 520, 'C');settextcolor(RGB(41,94,173));
	settextcolor(RGB(8,87,107));
	outtextxy(1450, 140, _T("����"));
	outtextxy(1450, 290, _T("����"));
	outtextxy(1450, 440, _T("����"));
	outtextxy(1450, 580, _T("��ͷ"));
	settextstyle(&textstyle2);
	settextcolor(RGB(8,87,107));
	if(strategy[0]==MAN_COMMAND)settextcolor(RGB(117,12,156));outtextxy(255, 740, _T("�˹�����"));settextcolor(RGB(8,87,107));
	if(strategy[0]==TURN_BY_TURN)settextcolor(RGB(117,12,156));outtextxy(655, 740, _T("�������"));settextcolor(RGB(8,87,107));
	if(strategy[0]==FAST_FIRST)settextcolor(RGB(117,12,156));outtextxy(1055, 740, _T("�쳵����"));settextcolor(RGB(8,87,107));
}

void manselect(Train* train1,Train* train2)
{
//	MOUSEMSG m;
	IMAGE img1;
	int flag = 0,a,b;
	for(int i=0;i<NUM_TRAIN;i++)
	{
		if(train[i]==train1) a=i;
		if(train[i]==train2) b=i;
	}
	//int x = 400, y = 250;
	//setorigin(x, y);
	loadimage(&img1, _T("manselect.jpg"));
	putimage(400,250, &img1);
	//setwritemode(R2_XORPEN);
	settextcolor(BLUE);
	settextstyle(&textstyle1);
	outtextxy(460, 400, a + 'A');
	outtextxy(600, 400, b + 'A');
	while (1)
	{
		settextstyle(&textstyle1);
		if ((u.y - 250 >= 150) && (u.y - 250 <= 180))
		{
			if ((u.x - 400 >= 200) && (u.x - 400 <= 230))
			{
				//putimage(400,250, &img1);
				settextcolor(RED);
				outtextxy(600, 400, b + 'A');
				settextcolor(BLUE);
				outtextxy(460, 400, a + 'A');
				flag = 1;
				if (u.mkLButton)
				{
					train[a]->state =STOP_PROBE;
					train[b]->state =RUN;
					
					break;
				}
			}
			else if ((u.x - 400 >= 60) && (u.x - 400<90))
			{
				//putimage(400,250, &img1);
				settextcolor(RED);
				outtextxy(460, 400, a + 'A');
				settextcolor(BLUE);
				outtextxy(600, 400, b + 'A');
				flag = 1;
				if (u.mkLButton)
				{
					train[b]->state =STOP_PROBE;
					train[a]->state =RUN;
					
					break;
				}
			}
			else if (flag == 1)
			{
				//putimage(400, 250, &img1);
				settextcolor(BLUE);
				outtextxy(460, 400, a + 'A');
				outtextxy(600, 400, b + 'A');
				flag = 0;
			}


		}
		else if (flag == 1)
		{
			//putimage(400,250, &img1);
			settextcolor(BLUE);
			outtextxy(460, 400, a + 'A');
			outtextxy(600, 400, b + 'A');
			flag = 0;
		}
	}
	//IMAGE img;
	//loadimage(&img, _T("beijing.jpg"));
	//setorigin(0, 0);
	//putimage(0, 0, &background);
	drawtrainmsg();
}

void ftoch(double a, char ch[4])        //doubleת�����ַ���
{
	ch[0] = (int)a + '0';
	ch[1] = '.';
	ch[2] = (int)(a * 10) % 10 + '0';
	ch[3] = '\0';
}

void inttoch(int a, char ch[])      //intת�����ַ���
{

	int b = a, i = 0, j;
	while (b != 0)
	{
		b /= 10;
		i++;
	}
	for (j = i - 1; j >= 0; j--)
	{
		ch[j] = a % 10 + '0';
		a /= 10;
	}
	ch[i] = ' ';
	ch[i + 1] = ' ';
	ch[i + 2] = '\0';
}

void printstring(char *ch,int x,int y,int n)    //��ͼ�λ���������ַ���   �ַ������ᡢ�����꣬��������
{
	while(*ch!='\0')
	{
		outtextxy(x,y,*ch);
		if(n==1) x+=25;
		else if (n==2) x+=15;
		else if (n==3) x+=10 ;
		ch+=1;
	}
}

void stationtime()
{
	settextstyle(&textstyle1);
	settextcolor(WHITE);
	char ch1[100];
	time_t end;
	end = clock();
	for (int i = 0; i<NUM_STATION; i++)
	{
		if (station[i]->state == BUSY)
		{
			if ((int)(station[i]->time - end + station_time[i]) / CLOCKS_PER_SEC > 0)
			{
				inttoch((int)(station[i]->time - end + station_time[i]) / CLOCKS_PER_SEC, ch1);
				switch (i)
				{
				case 0:printstring(ch1, 435, 195, 1); break;
				case 1:printstring(ch1, 435, 570, 1); break;
				case 2:printstring(ch1, 1320, 335, 1); break;
				case 3:printstring(ch1, 655, 355, 1); break;
				default:	
					break;
				}
			}
		}
	}
}

void drawprobrstate()
{
	int x, y;
	for (int i = 0; i<NUM_PUBLICTRACK * 4; i++)
	{
		switch (i)
		{
		case 0:x = 572; y = 88; break;
		case 1:x = 895; y = 88; break;
		case 2:x = 572; y = 583; break;
		case 3:x = 895; y = 582; break;
		}
		if (probe[i]->state == RED)  
			putimage(x, y, &imgr);
		else 
			putimage(x, y, &imgg);
	}
}

void drawtrainmsg()
{
	drawprobrstate();
	time_t now1;
	now1 = clock();
	//BeginBatchDraw();
	//IMAGE img;
	//loadimage(&img, _T("beijing.jpg"));
	//putimage(0, 0, &img);
	drawbutton();
	stationtime();
	//changes(u);
	//setbkmode(TRANSPARENT);
	//int x = 180, y = 0;
	//setorigin(x, y);
	char ch[4], ch1[10], ch2[10];  // �ٶȣ�ʣ��ʱ�䣬��ǰʱ��
	//gcvt(start_train[0]-now+finish_initial,1,NULL
	for (int i = 0; i<NUM_TRAIN; i++)
	{
		settextcolor(WHITE);
		settextstyle(&textstyle3);
		inttoch((int)(start_train[i] - now1 + finish_initial) / CLOCKS_PER_SEC, ch1);
		switch (i)
		{
		case 0:if ((start_train[0] - now1 + finish_initial)>0) { outtextxy(300, 60, _T("ʣ��ʱ�䣺"));printstring(ch1,375,  60,3 ); putimage(220, 0, &imga1); }
			   else { putimage(220, 0, &imga2); }
			   break;
		case 1:if ((start_train[1] - now1 + finish_initial)>0) { outtextxy(680, 60, _T("ʣ��ʱ�䣺")); printstring(ch1,755, 60,3); putimage(600,0, &imga1); }
			   else { putimage(600, 0, &imgb2); }
			   break;
		case 2:if ((start_train[2] - now1 + finish_initial)>0) { outtextxy(1060,60, _T("ʣ��ʱ�䣺")); printstring(ch1,1130, 60,3); putimage(980, 0, &imgc1); }
			   else { putimage(980, 0, &imgc2); }
			   break;
		}

		settextcolor(RGB(74,135,243));
		settextstyle(&textstyle2);
		inttoch((int)((now1 - finish_initial) / (double)CLOCKS_PER_SEC), ch2);
		outtextxy(40,10, _T("��ǰʱ��")); printstring(ch2,60, 40,2);

		settextcolor(WHITE);
		settextstyle(&textstyle3);
		ftoch(train[i]->speed, ch);
		outtextxy(i * 380 +300, 0, _T("Name:")); outtextxy(i * 380 + 370, 0, i + 'A');
		//outtextxy(i*400+100+x,20+y,"Type:");outtextxy(i*400+160+x,20+y,(train[i]->type=FAST) ? "�쳵" : "����");
		
		switch (train[i]->state)
		{
		case RUN:outtextxy(i * 380+300, 40 , _T("State:������ʻ")); outtextxy(i * 380 +300, 20, _T("Speed:")); printstring( ch,i * 380 +365, 20,3); break;
		case STOP_EMERGENCY:outtextxy(i * 380 +300, 40 , _T("State:����ͣ��")); outtextxy(i * 380 +300, 20 , _T("Speed:     0")); break;
		case STOP_PROBE:outtextxy(i * 380 +300, 40 , _T("State:�ȴ����빫�����")); outtextxy(i * 380 +300, 20 , _T("Speed:     0")); break;
		case STOP_STATION:outtextxy(i * 380 +300, 40 , _T("State:վ̨ͣ��")); outtextxy(i * 380 +300, 20, _T("Speed:     0")); break;
		case STOP_MANCONTROL:outtextxy(i * 380 +300, 40, _T("State:�˹�ͣ��")); outtextxy(i * 380 +300, 20, _T("Speed:     0")); break;
		case STOP_START:outtextxy(i * 380 +300, 40 , _T("State:��δ����")); outtextxy(i * 380 +300, 20, _T("Speed:     0")); break;
		case CRASH:outtextxy(i * 380 +300, 40 , _T("State:�����¹�")); outtextxy(i * 380 +300, 20, _T("Speed:     0")); break;
		}
	}
	//EndBatchDraw();
}

int drawdialogbox(int j)
{
	//int i,j;
	int flag = 0,n=0;
	//int x = 350, y = 350;
	//setorigin(x, y);
	IMAGE img1, img2, img3;
	loadimage(&img1, _T("dialog.jpg"));
	loadimage(&img2, _T("dialogy.jpg"));
	loadimage(&img3, _T("dialogn.jpg"));
//	MOUSEMSG u;
	//	for(i=0;i<NUM_STATION;i++)
	//{
	//	for(j=0;j<NUM_TRAIN;j++)
	//	{
	//	if ((int)train[j]->x[0] == station[i]->x && (int)train[j]->y[0] == station[i]->y)
	//{

	putimage(350, 350, &img1);
	while (1)
	{
//		u = GetMouseMsg();
		if ((u.y - 350 >= 80) && (u.y - 350 <= 115))
		{
			if ((u.x - 350 >= 370) && (u.x - 350 <= 440))
			{
				putimage(350, 350, &img2);
				flag = 1;
				if (u.mkLButton)
				{
					n=1;
					break;
				}
			}
			else if ((u.x - 350 >= 560) && (u.x - 350 <= 630))
			{
				putimage(350, 350, &img3);
				flag = 1;
				if (u.mkLButton)
				{
					break;
				}
			}
			else if (flag == 1)
			{
				putimage(350, 350, &img1);
				flag = 0;
			}
		}
		else if (flag == 1)
		{
			putimage(350, 350, &img1);
			flag = 0;
		}
	}
	//IMAGE img;
	//loadimage(&img, _T("beijing.jpg"));
	//putimage(0, 0, &img);
	drawtrainmsg();
	return n;
	//}
	//}

	//}
}

void turnback(Train* train)
{
	train->direction = (train->direction + 1) % 2;
	double tempx, tempy;
	int i, j;
	for (i = 0, j = LENGTH - 1; i<LENGTH / 2; i++, j--)
	{
		tempx = train->x[i];
		train->x[i] = train->x[j];
		train->x[j] = tempx;
		tempy = train->y[i];
		train->y[i] = train->y[j];
		train->y[j] = tempy;
	}
	for (i = 0; i<LENGTH; i++)
	{
		train->last_direction[i] = setDirection(train->direction, train->x[i], train->y[i], train->track);
	}
}

void changes(MOUSEMSG u)
{
	//settextstyle(&textstyle1);
	FILE *fp;
	if ((fp = fopen("record_irregular.txt", "a")) == NULL)
		printf("File open fail\n");
	time_t now = clock();

	if (u.mkLButton)
	{
		// �ı���Ʋ���
		if ((u.y >= 695) && (u.y <= 800) && (u.x >= 230) && (u.x <= 375))
		{
			strategy[0] = MAN_COMMAND;
			settextstyle(&textstyle2);
			settextcolor(RGB(117,12,156));
			outtextxy(255, 740, _T("�˹�����"));
			settextcolor(RGB(8,87,107));
			outtextxy(655, 740, _T("�������"));
			outtextxy(1055, 740, _T("�쳵����"));
			fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "�¼�:���%d���Ըı�Ϊ�˹�����\n\n", 0 + 1);
		}
		else if ((u.y >= 695) && (u.y <= 800) && (u.x >= 630) && (u.x <= 775))
		{
			strategy[0] = TURN_BY_TURN;
			settextstyle(&textstyle2);
			settextcolor(RGB(117,12,156));
			outtextxy(655, 740, _T("�������"));
			settextcolor(RGB(8,87,107));
			outtextxy(255, 740, _T("�˹�����"));
			outtextxy(1055, 740, _T("�쳵����"));
			fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "�¼�:���%d���Ըı�Ϊ�������\n\n", 0 + 1);
		}
		else if ((u.y >= 695) && (u.y <= 800) && (u.x >= 1030) && (u.x <= 1175))
		{
			strategy[0] = FAST_FIRST;
			settextstyle(&textstyle2);
			settextcolor(RGB(117,12,156));
			outtextxy(1055, 740, _T("�쳵����"));
			settextcolor(RGB(8,87,107));
			outtextxy(255, 740, _T("�˹�����"));
			outtextxy(655, 740, _T("�������"));
			fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "�¼�:���%d���Ըı�Ϊ�쳵����\n\n", 0 + 1);
		}
		
		//ѡ��A,B,C��
		if ((u.x >= 35) && (u.x <= 125) && (u.y >= 135) && (u.y <= 205))
		{
			selecttrain = 0;
			settextstyle(&textstyle1);
			settextcolor(RGB(117, 12, 156));
			outtextxy(70, 144, 'A');
			settextcolor(RGB(41, 94, 173));
			outtextxy(70, 335, 'B');
			outtextxy(70, 520, 'C');

		}
		else if ((u.x >= 35) && (u.x <= 125) && (u.y >= 325) && (u.y <= 390))
		{
			settextstyle(&textstyle1);
			settextcolor(RGB(117, 12, 156));
			outtextxy(70, 335, 'B');
			settextcolor(RGB(41, 94, 173));
			outtextxy(70, 144, 'A');
			outtextxy(70, 520, 'C');
			selecttrain = 1;
		}
		else if ((u.x >= 35) && (u.x <= 125) && (u.y >= 510) && (u.y <= 580))
		{
			settextstyle(&textstyle1);
			settextcolor(RGB(117, 12, 156));
			outtextxy(70, 520, 'C');
			settextcolor(RGB(41, 94, 173));
			outtextxy(70, 144, 'A');
			outtextxy(70, 335, 'B');
			selecttrain = 2;
		}

		//�㳵ֹͣ/����
		for (int i = 0; i < NUM_TRAIN; i++) {
			if (u.x >= T[i].x && u.x <= (T[i].x + 100) && u.y >= T[i].y && u.y <= (T[i].y + 100)) {
				if (train[i]->state == RUN) {
					train[i]->state = STOP_MANCONTROL;
					fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "�¼�:�˹�ֹͣ��%c\n", selecttrain + 'A');
				}
				else if (train[i]->state == STOP_MANCONTROL) {
					train[i]->state = RUN;
					fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
					fprintf(fp, "�¼�:�˹�������%c\n", selecttrain + 'A');
				}
			}
		}
	}
	else;

	//�ı��״̬
	int flag = 0;
	if ((u.x >= 1430) && (u.x <= 1600) && (u.y >= 100) && (u.y <= 200))
	{
		//settextstyle(&textstyle1);
		settextcolor(RGB(117,12,156));
		settextstyle(&textstyle1);
		outtextxy(1450, 140, _T("����"));
		flag = 1;
		if (u.mkLButton)
		{
			acceleration[selecttrain] += 1.0;
			fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "�¼�:��%c����\n\n", selecttrain + 'A');
		}
	}
	else if ((u.x >= 1430) && (u.x <= 1600) && (u.y >= 250) && (u.y <= 350))
	{
		//settextstyle(&textstyle1);
		settextcolor(RGB(117,12,156));
		settextstyle(&textstyle1);
		outtextxy(1450, 290, _T("����"));
		flag = 1;
		if (u.mkLButton)
		{
			acceleration[selecttrain] -= 1.0;
			fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "�¼�:��%c����\n\n", selecttrain + 'A');
		}
	}
	else if ((u.x >= 1430) && (u.x <= 1600) && (u.y >= 400) && (u.y <= 495))
	{
		//settextstyle(&textstyle1);
		settextcolor(RGB(117,12,156));
		settextstyle(&textstyle1);
		outtextxy(1450, 440, _T("����"));
		flag = 1;
		if (u.mkLButton)
		{
			tct[selecttrain] = 1;
			fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "�¼�:��%c���\n\n", selecttrain + 'A');
		}
	}
	else if ((u.x >= 1430) && (u.x <= 1600) && (u.y >= 540) && (u.y <= 640))
	{
		//settextstyle(&textstyle1);
		settextcolor(RGB(117,12,156));
		settextstyle(&textstyle1);
		outtextxy(1450, 580, _T("��ͷ"));
		flag = 1;
		if (u.mkLButton)
		{
			turnaround[selecttrain] = 1;
			fprintf(fp, "��ǰʱ��:%4.1f\n", (double)(now - finish_initial) / CLOCKS_PER_SEC);
			fprintf(fp, "�¼�:��%c��ͷ\n\n", selecttrain + 'A');
		}
	}
	else if(flag==1)
	{
		//settextstyle(&textstyle1);
		settextcolor(RGB(8,87,107));
		settextstyle(&textstyle1);
		outtextxy(1450, 140, _T("����"));
		outtextxy(1450, 290, _T("����"));
		outtextxy(1450, 440, _T("����"));
		outtextxy(1450, 580, _T("��ͷ"));
		flag = 0;
	}
	else;
	fclose(fp);
}

/*void main()
{
initgraph(1600,800);
IMAGE img,img1,img2,img3;
loadimage(&img,"beijing.jpg");
putimage(0,0,&img);
drawbutton();
//drawdialogbox();
//manselect(1,2);
/*loadimage(&img1,"dialog1.jpg");
putimage(0,0,&img1);
loadimage(&img2,"dialog2.jpg");
putimage(0,300,&img2);
loadimage(&img3,"dialog3.jpg");
putimage(600,500,&img3);
while(1)
{
u=GetMouseMsg();

changes(u);
//drawtrainmsg();
}
getchar();
closegraph();
}*/

unsigned __stdcall Show(void* pArgument)
{
	drawbutton();

	while (1)
	{
		if (MouseHit())
		{
			u = GetMouseMsg();
			changes(u);
		}
	}
	_endthreadex(0);
	return 0;
}