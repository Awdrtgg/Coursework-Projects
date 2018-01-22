%{
	#include<ctype.h>
	#include<stdio.h>
	#define YYSTYPE double
%}

%token NUM
%left '+' '-'
%left '*' '/'
%%

line:expr 			{printf("%lf\n",$1);}
	|line'\n'		{}
	;
expr:expr'+'expr	{$$=$1+$3;printf("E->E+E\n");}
	|expr'-'expr 	{$$=$1-$3;printf("E->E-E\n");}
	|temp 		 	{$$=$1;printf("E->T\n");}
	;
temp:temp'*'fact 	{$$=$1*$3;printf("T->T*F\n");}
	|temp'/'fact 	{$$=$1/$3;printf("T->T/F\n");}
	|fact			{$$=$1;printf("E->F\n");}
	;
fact:'('expr')'		{$$=$2;printf("T->(E)\n");}
	|NUM			{$$=$1;printf("T->NUM\n");}
	;
%%

int main(int argc,char *argv[])
{
	/*check the input*/
	if(argc!=2){
		printf("error usage\n");
		printf("right usage: program_name input_file_name  \n");
		return 0;
	}
	
	/* skip over program name */
	++argv, --argc; 
	
	/*input redirection*/
	if(freopen(argv[0],"r",stdin)==NULL){
		printf("error:open input_file failed\n");
		return 0;
	}
	
	//start
	printf("start\n");
	yyparse();
	
	fclose(stdin);   
}

int yylex()
{
	int c;
	if((c=getchar())!=EOF)
	{
		if(c=='.'||isdigit(c)){
			//read num
			ungetc(c,stdin);
			scanf("%lf",&yylval);
			return NUM;
		}
	}
	return c;
}

void yyerror(char *s){
	printf("%s\n",s);
}