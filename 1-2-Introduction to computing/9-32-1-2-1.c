#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student
{
	char name[20];
	char id[20];
	int score;
	struct Student *next;
};

struct Student *ListA = NULL;

struct Student *creat( struct Student *List)				//创建一个链表，获取输入的数据 
{
	struct Student *newPtr, *currentPtr;
	currentPtr = newPtr = malloc(sizeof(struct Student));
	
	printf("Please input the information of the students:\n");
	scanf("%s", &newPtr->name);
	while( strcmp(newPtr->name, "#####") ){
		if( List ==  NULL)
			List = currentPtr;
		else
			currentPtr->next = newPtr;
		currentPtr = newPtr;
		scanf("%s""%d", &newPtr->id, &newPtr->score);
		newPtr = malloc(sizeof(struct Student));
		scanf("%s", &newPtr->name);
		currentPtr->next = NULL;
	}
	return List;
}

struct Student *cut_and_paste(struct Student *ListB)		//将A链表中满足条件的节点切下连接到B链表后 
{
	struct Student *newPtr_A, *prePtr_A, *currentPtr_B = NULL, *newPtr_B = NULL;
	prePtr_A = ListA;
	newPtr_A = ListA->next;

	//第一个成绩就大于等于1400的情况 
	while( newPtr_A->next != NULL && prePtr_A->score >= 1400){
		printf("1\n");
		newPtr_B = prePtr_A;			//先将B的new指向该节点 
		newPtr_A = newPtr_A->next;		//将A的new向前移动一格 
		prePtr_A = prePtr_A->next;		//A的pre也向前移动一格 
		if( currentPtr_B == NULL)		//判断B是否为第一个节点，若是，则连上头指针，若不是，则将之前的链表连接到新的节点上 
			ListB = currentPtr_B = newPtr_B;
		else{
			currentPtr_B->next = newPtr_B;
			currentPtr_B = newPtr_B;
		}
		newPtr_B->next = NULL;			//将B新的节点指向NULL
	}
	ListA = prePtr_A;
	
	//当前第一个成绩不大于1400的情况 
	while( newPtr_A->next != NULL){
		printf("2\n");
		if( newPtr_A->score >= 1400){		//满足条件的节点的剪切与粘贴 
			printf("2.1\n");
			newPtr_B = newPtr_A;			//先将B的new指向该节点 
			newPtr_A = newPtr_A->next;		//A的new指针向前移动一个节点 
			prePtr_A->next = newPtr_A;		//将A的之前一个节点连接到new指针所指的节点，此时满足条件的节点已经被跳过 
			if( currentPtr_B == NULL)		//判断B是否为第一个节点，若是，则连上头指针，若不是，则将之前的链表连接到新的节点上 
				ListB = currentPtr_B = newPtr_B;
			else{
				currentPtr_B->next = newPtr_B;
				currentPtr_B = newPtr_B;
			}
			newPtr_B->next = NULL;			//将新的节点指向NULL 
		}
		else{								//若不满足条件，则将A的前后两个指针都向前移动一个节点 
			printf("2.2\n");
			newPtr_A = newPtr_A->next;
			prePtr_A = prePtr_A->next;
		}
	}
	
	//最后一个节点 
	if( newPtr_A->score >= 1400)
	{
		printf("3\n");
		newPtr_B = newPtr_A;
		if( currentPtr_B == NULL)		//判断B是否为第一个节点，若是，则连上头指针，若不是，则将之前的链表连接到新的节点上 
			ListB = currentPtr_B = newPtr_B;
		else{
			currentPtr_B->next = newPtr_B;
			currentPtr_B = newPtr_B;
		}
		prePtr_A->next = NULL;
	}
	else;
	
	return ListB;
}

void print(struct Student *ListB, char property[])			//打印目标链表的数据 
{
	if( ListB == NULL)
		printf("There is no student with a score higher than 1400\n"); 
	else{
		int x = 0;
		struct Student *p = ListB;
		while( p != NULL){
			x++;
			p = p->next;
		}
		
		p = ListB;
		printf("There are %d students' score %s than 1400. They are:\n", x, property);
		while( p != NULL){
			printf("Name:%s ID:%s Score:%d\n", p->name, p->id, p->score);
			p = p->next;
		}; 
	}
}

int main()
{
	struct Student *ListB = NULL;
	
	ListA = creat(ListA);
	ListB = cut_and_paste(ListB);
	
	print(ListB, "higher");
	print(ListA, "lower");
	free(ListA);
	free(ListB);
}
