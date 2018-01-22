#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 30

typedef struct
{
	char name[MAX_NAME];
	float credit;
	float score;
}COURSE;

char in_name();            // 输入名字 
char in_n();				  // 输入新成绩 
int in_file( COURSE *course, int i, char *str);			// 写入文件 
char read_his( char *str, int ss);			// 读取文件,s = 0为主菜单中直接读, s = 1为存储文件之后读 
char clear();			// 清除文件 

int type = 1; 		// GPA计算类型 

int main()
{
	char s = '1';
	type = 1;
	while(1) 
	{
		switch(s)
		{
			case '1':
				printf("Please input the number:\na.Input and calculate new score\nb.Read the history score\nc.Clear history score\nd.Change the way of GPA calculation\nz.END\n");
				fflush(stdin);
				scanf("%c", &s);
				break;
			
			case 'a':
				s = in_n();
				break;
				
			case 'b':
				s = in_name();
				break;
				
			case 'c':
				clear();
			
			case 'd':
				printf("Input corresponding number:\n1. 4.0 of PKU\n2. 4.0 Standard\n3. 5.0 Standard\n0.Return without changing.\n(Current type: %d)\n", type);
				int x;
				scanf("%d", &x);
				if (x >= 3){
					printf("Invalid input.\n");
					break;
				}
				else if (x > 0)
					type = x;
				s = '1';
				break;
				
			case 'z':
				exit(0);
				break;
				
			case '0':
				exit(0);
				break;
				
			default:
				printf("Invalid input. Please try again:\n");
				fflush(stdin);
				scanf("%c", &s);
			
		}
	}
}

float ChangeTo4(float n)
{
	switch(type)
	{
		case 1:
			if (n >= 90)		return 4.0;
			else if (n >= 85)	return 3.7;
			else if (n >= 82)	return 3.3;
			else if (n >= 78)	return 3.0;
			else if (n >= 75)	return 2.7;
			else if (n >= 72)	return 2.3;
			else if (n >= 68)	return 2.0;
			else if (n >= 64)	return 1.5;
			else if (n >= 60)	return 1.0;
			else				return 0.0;
			break;
			
		case 2:
			if (n >= 90)		return 4.0;
			else if (n >= 80)	return 3.0;
			else if (n >= 70)	return 2.0;
			else if (n >= 60)	return 1.0;
			else				return 0.0;
			break;

		default:
			return 0;
	}
}

char in_n()
{
	COURSE course[50], sum, gpa, gpa4, sum4;
	int i = 0, j;
	char n;
	sum.credit = 0, sum.score = 0;
	sum4.credit = 0; sum4.score = 0;
	
	printf("Please input the name of the course , the score and credit.\nIf you have finished inputing, input a character 'E' to COMPLETE.\n *Attention: Please do not input a blank in the name\n");
	
	while ( i < 50 && (scanf("%s", course[i].name) != EOF, *course[i].name != 'E') )
	{
		scanf("%f %f", &course[i].score, &course[i].credit);
		i++;
	}
	for( j = 0; j < i; j++)
	{
		sum.score += course[j].score * course[j].credit;
		sum4.score += ChangeTo4(course[j].score) * course[j].credit;
		sum.credit += course[j].credit;
		sum4.credit += course[j].credit;
	}
	gpa.score = sum.score / sum.credit;
	gpa4.score = sum4.score / sum4.credit;
	printf("The GPA of the score you put this time is %.2f(%.2f/4)\nPress \n0 to end the programe;\n1 to return to the main manu;\n2 to save the score in your history and check the total GPA.\n", gpa.score, gpa4.score);
	fflush(stdin);
	scanf("%c", &n);
	
	char str[MAX_NAME];
	
	while(1){
		switch(n) {
			case '0':
				return n;
				break;
				
			case '1':
				return n;
				break;
				
			case '2':
				printf("Please input the name of whom you want to operat:\n *Attention: Please do not input a blank in the name\n");
				scanf("%s", str);
				in_file( course, i, str);
				printf("Successful saved! Press\n0 to end the programe;\n1 to return to the main manu;\n2 to save the score in another file.\n");
				fflush(stdin);
				scanf("%c", &n);
				break;
		
			default:
				printf("Input error! Please input the number again.\n");
				fflush(stdin);
				scanf("%c", &n);
				break;
		}
	}
	
//	while ( n > '3' || n < '0')
//	{
//		printf("Input error! Please input the number again.\n");
//		fflush(stdin);
//		scanf("%c", &n);
//	}
//	while ( n == '2')
//	{
//		
//		char str[MAX_NAME];
//		printf("Please input the name of whom you want to operat:\n *Attention: Please do not input a blank in the name\n");
//		scanf("%s", str);
//		if (!in_file( course, i, str)){
//			printf("Successful saved! Press \n0 to end the programe;\n1 to return to the main manu;\n2 to save the score in another file.\n")
//			fflush(stdin);
//			scanf("%c", &n);
//		}
//	}
//	if (n == '0' || n == '1')
//		return n;
}

int in_file (COURSE *course, int n, char *str)
{
	FILE *fp;
	int j;
	
	if ( ( fp = fopen( str, "at+")) == NULL)
	{
		printf("Error to open file '%s', Press any key to go back to the main manu\n", str);
		getch();
		return 0;
	}
	
	for( j = 0; j < n; j++)
	{
		fputs( (course+j)->name, fp);
		fputc('\n', fp);
		fprintf( fp, "%f %f\n", ( course + j)->score, ( course + j)->credit );
	}
	fclose(fp);
	read_his(str, 1);
	return 1;
}

char read_his (char *str, int ss)
{
	FILE *fp;
	float sum_score = 0, sum_credit = 0, s, c, sum_score4 = 0, sum_credit4 = 0;
	char name[MAX_NAME+1];
	char i;
	
	if ((fp = fopen(str, "r")) == NULL)
	{
		printf("Error to open file '%s', Press any key to go back to the main manu\n", str);
		getch();
		return '1';
	}
	while( fgets( name, MAX_NAME, fp) != NULL)
	{
		fscanf( fp, "%f %f", &s, &c);
		sum_score += s * c;
		sum_credit += c;
		sum_score4 += ChangeTo4(s) * c;
		sum_credit4 += c;
		printf("Course:%-20s\tScore:%.2f\tCredit:%.2f\n", name, s, c);
		fgetc(fp);
	}
	fclose(fp);
	printf("The GPA of all the courses is %.2f(%.2f/4).\n", sum_score / sum_credit, sum_score4 / sum_credit4);
	switch(ss) {
		case 0:
			printf("Input numbers:\n1 to return to the main manu\n0 to END\n");
			fflush(stdin);
			scanf("%c", &i);
			return i;
			break;
			
		case 1:
			return '0';
			break;
			
		default:
			return '0';
			break;
	}
	
}

char in_name()
{
	char i;
	char str[MAX_NAME];
	printf("Please input the name of whose GPA you want to operat:\n *Attention: Please do not input a blank in the name\n");
	fflush(stdin);
	scanf("%s", str);
	i = read_his(str, 0);
	return i;
}

char clear()
{
	FILE *fp;
	char i;
	char str[MAX_NAME];
	printf("Please input the name of whose GPA you want to operat:\n *Attention:0.Input 0 for cancel and return to the main manu \n            1.Please do not input a blank in the name\n");
	fflush(stdin);
	scanf("%s", str);
	if ( *str == '0' )
		return '1';
	else
	{
		if ( ( fp = fopen(str, "w")) == NULL)
		{
			printf("Error to open file '%s', Press any key to go back to the main manu\n", str);
			getch();
			return '1';
		}
		fclose(fp);
		printf("Clear complete!\nInput 1 to return to main manu\n0 to END");
		fflush(stdin);
		scanf("%c", &i);
		return i;
	}
}
