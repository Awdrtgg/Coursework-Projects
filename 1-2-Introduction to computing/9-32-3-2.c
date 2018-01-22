#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define START 0
#define GATE1 1
#define GATE2 2
#define GATE3 3
#define GATE4 4
#define GATE5 5
#define FAIL 6
#define SUCCESS 7
#define END 8

int main()
{
	char int_[4], c;
	int state = START;
	
	printf("Please input a string:");
	while( state != END){
		switch(state){
			case START:
				scanf("%3s", int_);
				if( strcmp( int_, "int") == 0)
					state = GATE1;
				else
					state = FAIL;
				break;
							
			case GATE1:
				if( ( c >= 'a' && c <= 'z') || ( c >= 'A' && c <= 'Z') || c == '_')
					state = GATE2;
				else	state = FAIL;
				break;
				
			case GATE2:
				if( c == '[')	state = GATE3;
				else if( ( c >= 'a' && c <= 'z') || ( c >= 'A' && c <= 'Z') || c == '_' || ( c >= '0' && c <= '9'))
					state = GATE2;
				else	state = FAIL;
				break;
				
			case GATE3:
				if( c >= '0' && c <= '9')		state = GATE4;
				else	state = FAIL;
				break;
				
			case GATE4:
				if( c >= '0' && c <= '9')		state = GATE4;
				else if( c == ']')	state = GATE5;
				else	state = FAIL;
				break;
					
			case GATE5:
				switch(c){
					case ',': state = GATE1;	break;
					case ';': state = SUCCESS;	break;
					default: state = FAIL;	break;
				}
				break;
				
			case FAIL:
				printf("The result is: NO\n");
				exit(0);
				
			case SUCCESS:
				printf("The result is: YES\n");
				exit(1);
		}
		c = getchar();
		while( c == ' ')			//处理多余的空格，更接近于普遍C编译器的效果 
			c = getchar();
	}
	
}
 
