#include <stdio.h>
#include <stdlib.h>

void copy( char *file_a, char *file_b)
{
	FILE *fp_a, *fp_b;
	char ch;
	if( (fp_a = fopen( file_a, "a+")) == NULL){
		printf("open file %s error\n", file_a);
		exit(0);
	}
	if( (fp_b = fopen( file_b, "a+"))== NULL){
		printf("open file %s error\n", file_a);
		exit(0);
	}
	while( (ch = fgetc(fp_a)) != EOF){
		if( ch == '"' || ch == 39){											//当"或'出现，后面的任何符号都复制 
			char temp = ch;
			fputc( temp, fp_b);
			while( (ch = fgetc(fp_a)) != EOF && !(ch == temp) )
				fputc( ch, fp_b);
			fputc( temp, fp_b);
		}
		else{
			if( ch == '/'){										//当 / 出现，判断注释类型 
				if( (ch = fgetc(fp_a)) == EOF)
					exit(0);
				if(ch == '/')									// 即为 //, 忽略换行前的内容 
				{
					fputc( '\n', fp_b);
					while( (ch = fgetc(fp_a)) != EOF){
						if( ch == '\n')
							break;
					}
				}
				else if( ch == '*')								// 即为 /* ,忽略 */之前的内容 
				{
					while( (ch = fgetc(fp_a)) != EOF){
						if( ch == '*')
							if( (ch = fgetc(fp_a)) != EOF && ch == '/')
								break;
						}
				}
				else{
					fputc( '/', fp_b);
					fputc( ch, fp_b);
				}
			}
			else
				fputc( ch, fp_b);
		}
	}
	fclose(fp_b);
	fclose(fp_a);
}

int main()
{
	char file_a[50], file_b[50];
	scanf("%s", file_a);
	scanf("%s", file_b);
	copy( file_a, file_b);
}
