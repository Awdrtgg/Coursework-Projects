#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 1024
#define MAX_LINE 65535
#define MAX_TXT_LEN 1024
#define WORD_TYPE 5

#define BLANK -1
#define ID 0
#define NUM 1
#define STRING 2
#define PUNC 3
#define ERROR 4

int state = 0, yytext_count = 0;
char c = '\0';
char yytext[MAX_TXT_LEN];

int count_line = 0, count_word[WORD_TYPE] = {0}, count_char = 0;
int line_count_char[MAX_LINE];

FILE *p_in, *p_out;

int isLetter(char cc) {
    return ((cc >= 'a') && (cc <= 'z')) || ((cc >= 'A') && (cc <= 'Z'));
}

int isDigit(char nn) {
    return (nn >= '0') && (nn <= '9');
}

void record(int type, int retreat) {
    yytext[yytext_count] = '\0';
    yytext_count = 0;
    int col = count_char - strlen(yytext) - line_count_char[count_line];
    fprintf(p_out, "%d\t%d\t", count_line+1, col);
    switch(type) {
        case ERROR: strcpy(yytext, "error"); break;
        case ID: fprintf(p_out, "id\t"); break;
        case NUM: fprintf(p_out, "num\t"); break;
        case STRING: fprintf(p_out, "str\t"); break;
        case PUNC: fprintf(p_out, "punc\t"); break;
        default: printf("Warning!"); break;
    }
    fprintf(p_out, yytext);
    fprintf(p_out, "\n");
    if (fseek(p_in, -retreat, SEEK_CUR) == -1) {
    	printf("Input file error");
    	exit(0);
	}
    state = 0;
    count_char -= retreat;
    count_word[type] += 1;
}

void error(int retreat) {
    record(ERROR, retreat);
}

int main(int argc, char *argv[]) {
    char f_in[MAX_PATH] = {0};
    char f_out[MAX_PATH] = {0};

    if (argc != 3) {
        printf("Input Error");
        return -1;
    }
    else {
        strcat(f_in, argv[1]);
        strcat(f_out, argv[2]);
        p_in = fopen(f_in, "r");
        p_out = fopen(f_out, "a+");
        fprintf(p_out, "line\tcolumn\tattr\tproperty\n");
    }

    line_count_char[count_line] = count_char;
    c = fgetc(p_in);
    while (!feof(p_in)) {
        count_char++;
        printf("Ln=%d, col=%d, count_word=%d, ch=%c, state=%d, yytext_count=%d\n", count_line+1, count_char-line_count_char[count_line], count_char, c, state, yytext_count);
        //getchar();
        switch (state) {
            case 0:
                if (isLetter(c)) {
                    yytext[yytext_count++] = c;
                    state = 1;
                }
                else if (isDigit(c)) {
                    yytext[yytext_count++] = c;
                    state = 2;
                }
                else 
                    switch(c) {
                        case '\n': count_line++; line_count_char[count_line] = count_char;
                        case ' ':
                        case '\v':
                        case '\f': count_char++; yytext_count = 0; break;
                        
                        case '/': yytext[yytext_count++] = c; state = 8; break;
                        
                        case '\'': yytext[yytext_count++] = c; state = 12; break;
                        case '\"': yytext[yytext_count++] = c; state = 14; break;

                        case '^': 
                        case '!':
                        case '*':
                        case '=':
                        case '%': yytext[yytext_count++] = c; state = 16; break;

                        case '-': yytext[yytext_count++] = c; state = 17; break;

                        case '+': yytext[yytext_count++] = c; state = 18; break;
                        case '&': yytext[yytext_count++] = c; state = 19; break;
                        case '|': yytext[yytext_count++] = c; state = 20; break;
                        case '<': yytext[yytext_count++] = c; state = 21; break;
                        case '>': yytext[yytext_count++] = c; state = 23; break;

                        case ';':
                        case ':':
                        case '(':
                        case ')':
                        case '[':
                        case ']':
                        case '{':
                        case '}':
                        case '.':
                        case '~':
                        case '\\':
                        case '#':
                        case ',':
                        case '?': yytext[yytext_count++] = c; record(PUNC, 0); break;
                        default: error(0); break;
                    };
            break;

            case 1:
                if (isLetter(c) || isDigit(c) || c == '_')
                    yytext[yytext_count++] = c;
                else record(ID, 1);
            break;

            case 2:
                if (isDigit(c))
                    yytext[yytext_count++] = c;
                else if (c == '.') {
                    yytext[yytext_count++] = c;
                    state = 3;
                }
                else if (c == 'E' || c == 'e') {
                    yytext[yytext_count++] = c;
                    state = 5;
                }
                else record(NUM, 1);
            break;

            case 3:
                if (isDigit(c)) {
                    yytext[yytext_count++] = c;
                    state = 4;
                }
                else error(0);
            break;

            case 4:
                if (isDigit(c)) 
                    yytext[yytext_count++] = c;
                else if (c == 'E' || c == 'e') {
                    yytext[yytext_count++] = c;
                    state = 5;
                }
                else record(NUM, 1);
            break;

            case 5:
                if (isDigit(c)) {
                    yytext[yytext_count++] = c;
                    state = 7;
                }
                else if (c == '-' || c == '+') {
                    yytext[yytext_count++] = c;
                    state = 6;
                }
                else error(0);
            break;

            case 6:
                if (isDigit(c)) {
                    yytext[yytext_count++] = c;
                    state = 7;
                }
                else error(0);
            break;

            case 7:
                if (isDigit(c)) {
                    yytext[yytext_count++] = c;
                    state = 7;
                }
                else record(NUM, 1);
            
            case 8:
                switch (c) {
                    case '/': state = 9; break;
                    case '*': state = 10; break;
                    case '=': yytext[yytext_count++] = c;
                    default: record(PUNC, 0); break;
                }
            break;

            case 9:
                if (c == '\n') {
                    count_line++;
                    line_count_char[count_line] = count_char;
                    state = 0;
                }
                else
                    ;
            break;

            case 10:
                if (c == '*')
                    state = 11;
                else if (c == '\n') {
                    count_line++;
                    line_count_char[count_line] = count_char;
                } 
                else
                    ;
            break;

            case 11:
                if (c == '*');
                else {
                    if (c == '\n') {
                    	count_line++;
                        line_count_char[count_line] = count_char;   
                    }
                    state = 10;
                }
            break;

            case 12:
                if (c == '\'') {
                	yytext[yytext_count++] = c;
                    record(STRING, 0);
				}
				else if (c == '\\') {
					yytext[yytext_count++] = c;
					state = 13;
				}
                else {
                    if (c == '\n') {
                    	count_line++;
                        line_count_char[count_line] = count_char;
                    }
                    else
                        yytext[yytext_count++] = c;
                }
            break;
            
            case 13:
            	yytext[yytext_count++] = c;
            	state = 12;
            break;

            case 14:
                if (c == '\"')  {
                	yytext[yytext_count++] = c;
                    record(STRING, 0);
				}
				else if (c == '\\') {
					yytext[yytext_count++] = c;
					state = 15;
				}
                else {
                    if (c == '\n') {
                    	count_line++;
                        line_count_char[count_line] = count_char;
                    }
                    else
                        yytext[yytext_count++] = c;
                }
            break;
            
            case 15:
            	yytext[yytext_count++] = c;
            	state = 14;
            break;

            case 16:
                if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;

            case 17:
                if (c == '-') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else if (c == '>') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;

            case 18:
                if (c == '+') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;

            case 19:
                if (c == '&') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;

            case 20:
                if (c == '|') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;
            
            case 21:
                if (c == '<') {
                    yytext[yytext_count++] = c;
                    state = 22;
                }
                else if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;

            case 22:
                if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;

            case 23:
                if (c == '>') {
                    yytext[yytext_count++] = c;
                    state = 24;
                }
                else if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;
            
            case 24:
                if (c == '=') {
                    yytext[yytext_count++] = c;
                    record(PUNC, 0);
                }
                else 
                    record(PUNC, 1);
            break;
                    
                    

            default:
            break;
        };
        c = fgetc(p_in);
    }
    fprintf(p_out, "\nSummary: %d lines, %d characters.\n", count_line, count_char);
    fprintf(p_out, "Statistics: %d id(s), %d number(s), %d string(s), %d error(s).\n", count_word[ID], count_word[NUM], count_word[STRING], count_word[ERROR]);

    fclose(p_in);
    fclose(p_out);
    return 0;
}

