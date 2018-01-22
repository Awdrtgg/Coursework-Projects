#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 35 

typedef struct
{
	int id;
	char name[MAX_NAME];
	int num;
	float price;
}COMMODITY;

int initial();			//创建一个100条空记录的文件
int get_record();        //输入商品记录
int update_record();		//更新商品记录
int clear();			//删除商品记录		
void print_all();		//输出商品记录

int main()
{
	
	int i = 1;
	while( i != 0){
		int s;
		printf("请输入您的选择：\n1--创建一个100条空记录的文件\n2--输入商品记录\n3--更新商品记录\n4--删除商品记录\n5--输出商品记录\n");
		scanf("%d", &s);
		switch(s)
		{
			case 1: i = initial(); break;
			case 2: i = get_record(); break;
			case 3: i = update_record(); break;
			case 4: i = clear(); break;
			case 5: print_all(); break;
			default: break;
		}
	}
}

int initial()
{
	FILE *fp;
	
	fp = fopen( "commodity.dat", "wb");
	COMMODITY commodity = {
		.id = 0,
		.name = '\0',
		.num = -1,
		.price = 0.0
	};
	
	for( commodity.id = 0; commodity.id < 100; commodity.id++)
	{
		fwrite( &commodity, sizeof(COMMODITY), 1, fp);
	}
	fclose(fp);
	printf("100条空记录创建完毕\n");
	return 1;
}

int get_record()
{
	FILE *fp;
	COMMODITY commodity;
	if( (fp = fopen("commodity.dat","rb+")) == NULL ){
		printf("无法打开文件\n");
		return 0;
	}
	
	printf("请输入商品信息[记录号为-1代表输入结束]：\n记录号：");
	scanf("%d", &commodity.id);
	while( commodity.id != -1){
		printf("商品名：");
		scanf("%s", commodity.name);
		printf("数量：");
		scanf("%d", &commodity.num);
		printf("价格：");
		scanf("%f", &commodity.price);
		
		fseek( fp, (commodity.id-1)*sizeof(COMMODITY), SEEK_SET);
		fwrite( &commodity, sizeof(commodity), 1, fp);
		
		printf("请输入商品信息[记录号为-1代表输入结束]：\n记录号：");
		scanf("%d", &commodity.id);
	}
	
	fseek( fp, 0, SEEK_SET);
	print_all(fp);
	
	fclose(fp);
	return 1;
}

int update_record()
{
	FILE *fp;
	COMMODITY commodity;
	if( (fp = fopen("commodity.dat","rb+")) == NULL ){
		printf("无法打开文件\n");
		exit(0);
	}
	
	printf("请输入待更新商品ID:\n");
	scanf("%d", &commodity.id);
	fseek( fp, sizeof(COMMODITY)*(commodity.id-1), SEEK_SET);
	fread( &commodity, sizeof(COMMODITY), 1, fp); 
	
	if( commodity.num == -1)
		printf("对不起，不存在该商品，无法进行更新操作\n");
	else{
		printf("原商品信息如下：\n记录号（商品ID）	商品名                               \t数量	价格\n");
		printf("%-16d\t%-35s\t%-7d%.2f\n", commodity.id, commodity.name, commodity.num, commodity.price);
		printf("请输入更新信息[如某项不更新请直接按回车键]：\n");
		printf("商品名：");
		
		char c;
		int temp;
		
		fflush(stdin);
		scanf("%c", &c);
		if( c != '\n')
		{
			int i = 0;
			commodity.name[i] = c;
			scanf("%c", &c);
			while( c != '\n' && i < 35){
				i++;
				commodity.name[i] = c;
				scanf("%c", &c);
			}
			commodity.name[i+1] = '\0';
		}
		
		printf("数量：");
		scanf("%c", &c);
		if( c != '\n')
		{
			commodity.num = c - '0';
			scanf("%c", &c);
			while( c != '\n'){
				commodity.num *= 10;
				commodity.num += c - '0';
				scanf("%c", &c);
			}	
		}

		printf("价格：");
		scanf("%c", &c);
		if( c != '\n')
		{
			commodity.price = 0.0;
			while( c != '.' && c != '\n'){
				commodity.price *= 10;
				commodity.price += (float)(c-'0');
				scanf("%c", &c);
			}
			if( c != '\n'){
				float decimal = 1.0;
				scanf("%c", &c);
					while( c != '\n'){
						decimal *= 0.1;
						commodity.price += (c-'0')*decimal;
						scanf("%c", &c);
					}
			}	
		}

		fseek( fp, (commodity.id-1)*sizeof(COMMODITY), SEEK_SET);
		fwrite( &commodity, sizeof(COMMODITY), 1, fp);
		
		printf("更新后商品信息如下：\n记录号（商品ID）	商品名                               \t数量	价格\n");
		printf("%-16d\t%-35s\t%-7d%.2f\n", commodity.id, commodity.name, commodity.num, commodity.price);
	}
	fclose(fp);
	return 1;	
}

int clear()
{
	FILE *fp;
	COMMODITY temp;
	if( (fp = fopen("commodity.dat","rb+")) == NULL ){
		printf("无法打开文件\n");
		return 0;
	}
	
	printf("请输入待删除商品ID:\n");
	scanf("%d", &temp.id);
	fseek( fp, (temp.id-1)*sizeof(COMMODITY), SEEK_SET);
	fread( &temp, sizeof(COMMODITY), 1, fp); 
	fseek( fp, (temp.id-1)*sizeof(COMMODITY), SEEK_SET);
	
	if( temp.num == -1)
		printf("对不起，不存在该商品，无法进行删除操作\n");
	else{
		printf("原商品信息如下：\n记录号（商品ID）	商品名                                 数量	价格\n");
		printf("%-16d\t%-35s\t%-7d%.2f\n", temp.id, temp.name, temp.num, temp.price);
		memset( temp.name, '\0', MAX_NAME*sizeof(char));
		temp.num = -1;
		temp.price = 0.0;
		fwrite( &temp, sizeof(COMMODITY), 1, fp);
		printf("删除成功\n");
	}
	
	fclose(fp);
	return 1;
}

void print_all()
{
	FILE *fp;
	if( (fp = fopen("commodity.dat","rb+")) == NULL ){
		printf("无法打开文件\n");
		exit(0);
	}
	
	COMMODITY commodity;
	int i = 0; 
	fseek( fp, 0, SEEK_SET);
	printf("商品信息如下：\n记录号（商品ID）	商品名                               \t数量	价格\n");
	while( i < 100 ){
		fread( &commodity, sizeof(COMMODITY), 1, fp);
		if( commodity.num > 0)
			printf("%-16d\t%-35s\t%-7d%.2f\n", commodity.id, commodity.name, commodity.num, commodity.price);
	i++;
	}
	
	fclose(fp);
}
