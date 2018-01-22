#include <stdio.h>
#include <stdlib.h>

struct ListNode
{
	int data;
	struct ListNode *next;
};

struct ListNode *create( struct ListNode *head)				//输入一个链表 
{
	struct ListNode *currentPtr, *newPtr;
	printf("Please input the elements of the list:");
	currentPtr = newPtr = malloc(sizeof(struct ListNode));	
	scanf("%d", &newPtr->data);
	while( newPtr->data != -1){
		if ( head == NULL)			//判断是否是第一个，是的话接入头地址 
			head = newPtr;
		else
			currentPtr->next = newPtr;
		currentPtr = newPtr;
		newPtr = malloc(sizeof(struct ListNode));
		scanf("%d", &newPtr->data);
	}
	currentPtr->next = NULL;
	free(newPtr);
	return head;
}

void print(struct ListNode *p, char *list)				//打印一个链表的内容 
{
	if( p == NULL)
		printf("There is no item in %s list.\n", list); 
	else{
		printf("The List %s:", list);
		while( p->next != NULL){
			printf("%d ", p->data);
			p = p->next;
		}; 
		printf("%d\n", p->data);
	}
}

struct ListNode *Conjunction( struct ListNode *head_A, struct ListNode *head_B)		//提取出两个链表数据相同的值的节点 
{
	struct ListNode *C = NULL, *p_A = head_A, *p_B = head_B, *currentPtr, *newPtr;
	currentPtr = newPtr = malloc(sizeof(struct ListNode));
	while( p_A != NULL && p_B != NULL){ 
		if( p_A->data == p_B->data){				//若两链表数据有相同的值，则复制该节点并连接到C1链表后 
			if ( C == NULL)
				C = currentPtr;
			else
				currentPtr->next = newPtr;
			newPtr->data = p_A->data;
			currentPtr = newPtr;
			newPtr = malloc(sizeof(struct ListNode));
			currentPtr->next = NULL;
			p_A = p_A->next;
		}
		else if( p_A->data > p_B->data)				//若元素不同，则让数据较小的一个链表向前进一步，直到有一方走到头 
			p_B = p_B->next;
		else
			p_A = p_A->next;
	}
	return C;
}

struct ListNode *Disjunction( struct ListNode *head_A, struct ListNode *head_B)		//提取两个链表不同的元素 
{
	struct ListNode *C = NULL, *p_A = head_A, *p_B = head_B, *currentPtr, *newPtr;
	currentPtr = newPtr = malloc(sizeof(struct ListNode));
	while( p_A != NULL || p_B != NULL){
		if( p_A == NULL || p_A->data > p_B->data){			//以A链表为参照，当B比A小时或者A链表已经到头时，复制B的节点加到C2上 
			if ( C == NULL)
				C = currentPtr;
			else
				currentPtr->next = newPtr;
			newPtr->data = p_B->data;
			currentPtr = newPtr;
			newPtr = malloc(sizeof(struct ListNode));
			currentPtr->next = NULL;
			p_B = p_B->next;
		}
		else if( p_B == NULL || p_A->data < p_B->data){		//如果A链表数据的值小于B链表数据的值，则复制A链表的这个节点加到C2上 
			if ( C == NULL)
				C = currentPtr;
			else
				currentPtr->next = newPtr;
			newPtr->data = p_A->data;
			currentPtr = newPtr;
			newPtr = malloc(sizeof(struct ListNode));
			currentPtr->next = NULL;
			p_A = p_A->next;
		}
		else{												//当A和B链表的数据相等时，两个链表同时向前走一位，跳过这个值 
			p_A = p_A->next;
			p_B = p_B->next;
		}
	}
	return C;
}

int main()
{
	struct ListNode *head_A = NULL, *head_B = NULL, *head_C1, *head_C2;
	char C1[3] = "C1", C2[3] = "C2";
	
	head_A = create(head_A);
	head_B = create(head_B);
	head_C1 = Conjunction( head_A, head_B);
	head_C2 = Disjunction( head_A, head_B);

	print(head_C1, C1);
	print(head_C2, C2); 
	
	free(head_A);
	free(head_B);
	free(head_C1);
	free(head_C2);
}
