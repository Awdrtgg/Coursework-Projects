#include <stdio.h>
#include <stdlib.h>
#define START 0
#define GATE1 1
#define GATE2 2
#define GATE3 3
#define GATE4 4
#define GATE5 5
#define GATE6 6
#define GATE7 7
#define END 8

int main()
{
	char sourcefile[50], newfile[50];
	printf("Input the name of sourcefile:");
	scanf("%s", sourcefile);
	printf("Input the name of file which would copy to:");
	scanf("%s", newfile);
	
	FILE *fp_s, *fp_n;
	if ( ( fp_s = fopen( sourcefile, "r")) == NULL || (fp_n = fopen( newfile, "w")) == NULL)
	{
		printf("Open File Error\n");
		exit(0);
	}
	
	char s_char;
	int state = START;
	
	s_char = fgetc(fp_s);
	while( state != END){
	
		switch(state){
			case START:
				switch(s_char){
					case '/': 
						state = GATE1; 
						break;
					case '"': 
						fputc( '"', fp_n);
						state = GATE6; 
						break;
						
					case '\'':
						fputc( s_char, fp_n);
						state = GATE7;
						break;
					default:
						fputc( s_char, fp_n);
						state = GATE5;
						break;
				}
				break;
				
			case GATE1:
				switch(s_char){
					case '/': 
						state = GATE2; 
						break;
					case '*': 
						state = GATE3; 
						break;
					default: 
						fputc( '/', fp_n); 
						fputc( s_char, fp_n);
						state = GATE5;
						break;
				}
				break;
				
			case GATE2:
				if( s_char == '\n'){
					fputc( s_char, fp_n);
					state = GATE5;
				}
				else 
					state = GATE2; 
				break;
				
			case GATE3:
				if( s_char == '*')	
					state = GATE4;
				else	
					state = GATE3;
				break;
			
			case GATE4:
				switch(s_char){
					case '*': 
						state = GATE4; 
						break;
					case '/': 
						state = GATE5; 
						break;
					default: 
						state = GATE3; 
						break;
				}
				break;
				
			case GATE5:
				switch(s_char){
					case '/': 
						state = GATE1; 
						break;
					case '"': 
						fputc( '"', fp_n);
						state = GATE6; 
						break;
					case 39:
						fputc( s_char, fp_n);
						state = GATE7;
						break;
					default:
						fputc( s_char, fp_n);
						state = GATE5;
						break;
				}
				break;
			
			case GATE6:
				if( s_char == '"'){
					fputc( s_char, fp_n);
					state = GATE5;
				}
				else{
					fputc( s_char, fp_n);
					state = GATE6;
				}
				break;
				
			case GATE7:
				if( s_char == '\''){
					fputc( s_char, fp_n);
					state = GATE5;
				}
				else{
					fputc( s_char, fp_n);
					state = GATE7;
				}
				break;
		}
		
		if( (s_char = fgetc(fp_s)) == -1) 
			state = END;
	}
	
	fclose(fp_s);
	fclose(fp_n);
	return 0;
} 
