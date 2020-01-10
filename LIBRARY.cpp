#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "YE_library.h"


static int MAXBKS = 100;
static BOOK * Library = NULL;
static int isManage = 0;
static char file_name[257] = {0};
static char Key[MAXKEY] = {0};


void Menu( char file_name[] , Tree *root , int *count );
void Manager( char file_name[] , Tree *root , int *count );
int Append( char file_name[] , Tree *root , int n_file );
int Delete( char file_name[] , Tree *root , int n_file );
void Modify( char file_name[] , Tree *root ,int n_file );
void Repertory( char file_name[] , Tree *root , int n_file );
void Find( char file_name[] ,Tree *root , int n_file);
void Rand( char file_name[] , int n_file );
void Value( char file_name[] ,int n_file );
void Title( char file_name[] ,int n_file );
void Writer( char file_name[] ,int n_file );
void ID_ord( char file_name[] ,int n_file );
void Show( BOOK library[] , int n_file );
void Change_max( int n_file );
int Load( char  file_name[] , Tree *root );
void Change_key( char key[] , int n );


int main(void)
{
	Node *root = NULL;
	int count = 0;
	char file_user[257] = {0};
	FILE * pbooks = NULL;
	Change_key( Key , MAXKEY );
	Change_max( count );
	count = Load( file_name , &root );
	Menu( file_name , &root ,&count );
	strncpy( file_user , file_name , strlen(file_name)-4 );
	strcat( file_user , "(文本).txt" );
	pbooks = fopen( file_user , "r" );
	if( pbooks==NULL )
	{
		pbooks = YE_fopen( file_user , "w+" );
		fprintf( pbooks , HEADER );
		Traverse( pbooks , &root , "w+" );
		YE_fclose(pbooks);
	}
	else
	{
		YE_fclose(pbooks);
		if( isManage )
		{
			pbooks = YE_fopen( file_user , "w+" );
			fprintf( pbooks , HEADER );
			Traverse( pbooks , &root , "w+" );
			YE_fclose(pbooks);
		}
	}
	FreeTree( &root );
	if( Library != NULL )
		free(Library);
	puts("此次文件操作结束");
	return 0;
}


//功能描述：普通用户有权限控制的主菜单，多个函数的入口
//输入参数：一个文件名，一个树的地址，一个书目数量值
void Menu( char file_name[] , Tree *root ,int *count )
{
	system("CLS");
	int quit = 0;
	char mode = 0;
	while( 1 )
	{
		puts("请输入对应的大写字母:");
		puts("[F]ind找书    [B]andR借还相关    [R]and随机推荐    [M]anager管理员窗口");
		puts("[Q]uit退出");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			else
				break;
		}
		if( strchr( "BRMQF",mode )&&YE_clean()==1 )
		{
			switch(mode)
			{
			case 'B':
				Repertory( file_name , root , *count );
				break;
			case 'R':
				Rand( file_name , *count );
				break;
			case 'M':
				Manager( file_name , root , count );
				break;
			case 'F':
				Find( file_name , root , *count );
				break;
			case 'Q':
				quit = 1;
			}
		}
		else
		{
			puts("请输入正确的选项");
			continue;
		}
		if( quit )
			break;
	}
}


//功能描述：管理员有权限控制的菜单，增删改查函数的入口
//输入参数：一个文件名，一个树的地址，一个书目数量值
void Manager( char file_name[] , Tree *root , int *count )
{
	system("CLS");
	int quit = 0;
	char password[MAXKEY] = {0};
	FILE * fp = NULL;
	puts("输入密码以验证身份(空行退出)");
	while( YE_gets(password , MAXKEY , stdin)&&password[0]!='\0' )
	{
		if( strcmp( password , Key )==0 )
		{
			system("CLS");
			isManage = 1;
			char mode = 0;
			puts("[V]alue价格排序   [T]itle按书名排序  [W]riter按作者名排序  [I]D按序号排序");
			puts("[D]elete删除      [A]ppend添加       [M]odify修改          [C]hange改容");
			puts("[K]ey改密码       [L]oad载入文件     [Q]uit返回主菜单");
			while( scanf_s("%c",&mode) )
			{
				if( isspace(mode) )
					continue;
				if( strchr("DAMCQVTWILK",mode)&&YE_clean()==1 )
				{
					switch(mode)
					{
					case 'V':
						Value( file_name , *count );
						break;
					case 'T':
						Title( file_name , *count );
						break;
					case 'W':
						Writer( file_name , *count );
						break;
					case 'I':
						ID_ord( file_name ,*count );
						break;
					case 'A':
						*count = Append( file_name , root , *count );
						break;
					case 'D':
						*count = Delete( file_name , root , *count );
						break;
					case 'M':
						Modify( file_name , root , *count );
						break;
					case 'C':
						Change_max( *count );
						break;
					case 'K':
						Change_key( Key , MAXKEY );
						break;
					case 'L':
						*count = Load( file_name , root );
						break;
					case 'Q':
						quit = 1;
					}
				}
				else
				{
					puts("请输入对应的字母");
					continue;
				}
				if( quit )
					break;
				puts("[V]alue价格排序   [T]itle按书名排序  [W]riter按作者名排序  [I]D按序号排序");
				puts("[D]elete删除      [A]ppend添加       [M]odify修改          [C]hange改容");
				puts("[K]ey改密码       [L]oad载入文件     [Q]uit返回主菜单");
			}
			if( quit )
			{
				quit = 0;
				break;
			}
		}
		else
		{
			puts("验证失败");
			puts("请重输密码，或空行退出");
		}
	}
	system("CLS");
}


//功能描述：随机显示文件中的一个书目信息
//输入参数：一个文件名，一个书目数量值
//其他说明：若书目数为0，则返回
void Rand( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return;
	}
	system("CLS");
	BOOK * BOOKS = NULL;
	FILE * fp = NULL;
	BOOKS = (BOOK *)YE_alloc( 1 , YE_size );
	fp = YE_fopen( file_name , "rb" );
	int item = 0;
	srand( (unsigned int)time(NULL) );
	item = rand() % n_file;
	fseek( fp , item*YE_size , SEEK_SET );
	fread( BOOKS , 1 , YE_size , fp );
	printf(HEADER);
	YE_print( *BOOKS );
	free(BOOKS);
	YE_fclose(fp);
	puts("回车键继续");
	getchar();
	system("CLS");
}


//功能描述：在文件中查找与用户输入的信息一致的内容
//输入参数：一个文件名，一个树的地址，一个书目数量值
//其他说明：若书目数为0，则返回
//          书名和作者名是关键字查找，书号是二叉树查找
void Find( char file_name[] , Tree *root , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return;
	}
	system("CLS");
	int quit = 0;
	int n_find = 0;
	int  * Find_i = NULL;
	FILE * fp = NULL;
	BOOK * library = NULL;
	Pair find;
	memset( &find , 0 , sizeof(Pair) );
	fp = YE_fopen( file_name , "rb" );
	Find_i=(int *)YE_alloc(n_file,sizeof(int));
	library = (BOOK *)YE_alloc(n_file , YE_size);
	fread( library , n_file , YE_size , fp );
	BOOK Temp;
	memset( &Temp , 0 , YE_size );
	while(1)
	{
		char mode = 0;
		puts("[T]itle按书名查找  [W]riter按作者名查找  [I]D按书号查找   [Q]uit退出");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			if( strchr("TWQI",mode)&&YE_clean()==1 )
				break;
			else
			{
				puts("请输入对应的字母");
				continue;
			}
		}
		switch(mode)
		{
		case 'T':
			puts("输入书名");
			while(YE_gets( Temp.title , MAXTITL , stdin ))
			{
				if( Temp.title[0]=='\0' )
					puts("该项不能为空");
				else
					break;
			}
			for( int i=0 ; i<n_file ; i++ )
			{
				if( strcmp(library[i].title , Temp.title)==0 )
					Find_i[n_find++] = i;
			}
			for( int i=0 ; i<n_file ; i++ )
			{
				if( strstr(library[i].title , Temp.title) )
				{
					if( strcmp(library[i].title , Temp.title)==0 )
						continue;
					Find_i[n_find++] = i;
				}
			}
			break;
		case 'W':
			puts("输入作者名");
			while(YE_gets( Temp.author , MAXAUTL , stdin ))
			{
				if( Temp.author[0]=='\0' )
					puts("该项不能为空");
				else
					break;
			}
			for( int i=0 ; i<n_file ; i++ )
			{
				if(strcmp(library[i].author , Temp.author)==0 )
					Find_i[n_find++] = i;
			}
			for( int i=0 ; i<n_file ; i++ )
			{
				if( strstr(library[i].author , Temp.author) )
				{
					if( strcmp(library[i].title , Temp.title)==0 )
						continue;
					Find_i[n_find++] = i;
				}
			}
			break;
		case 'I':
			puts("输入书号");
			while( YE_gets( Temp.ID , MAXID , stdin ) )
			{
				if( Temp.ID[0]=='\0' )
					puts("该项不能为空");
				else
					break;
			}
			find = SeekBook( Temp , root );
			if( find.child==NULL )
				puts("查无此书");
			else
			{
				printf(HEADER);
				YE_print( find.child->book );
			}
			break;
		case 'Q':
			quit = 1;
		}
		if( quit )
		{
			quit = 0;
			break;
		}
		if( n_find==0 && mode!='I' )
			puts("查无此书");
		else if( mode!='I' )
		{
			printf("找到%d本你可能要的\n",n_find);
			for( int i=0 ; i<n_find ; i++ )
			{
				if( i==0 )
					printf(HEADER);
				YE_print( library[Find_i[i]] );
			}
			n_find = 0;
		}
	}
	system("CLS");
	free(Find_i);
	free(library);
	YE_fclose(fp);
}


//功能描述：在二叉树中添加一个节点，并在文件末尾添加书目信息
//输入参数：一个文件名，一个树的地址，一个书目数量值
//返回值  ：书目种类的最终数量
//其他说明：若书目数达到上限，则返回。
int Append( char file_name[] , Tree *root , int n_file )
{
	system("CLS");
	int x = 0;
	int n_book = 0;
	char mode = 0;
	BOOK temp;
	memset( &temp , 0 , YE_size );
	Pair find;
	memset( &find , 0 , sizeof(Pair) );
	FILE *fp = NULL;
	Node * pnew = NULL;
	int filecount = n_file;
	if( n_file == MAXBKS )
	{
		fputs("文件已满.\n",stderr);
		return n_file;
	}
	fp = YE_fopen( file_name , "a+b" );
	puts("开始录入新数据:");
	printf("输入书的书号(限个%d字符)(空行结束):\n",x=MAXID-1);
	while( n_file<MAXBKS )
	{
		if(YE_gets(temp.ID , MAXID,stdin)!=NULL)
		{
			if( temp.ID[0]=='\0' )
				break;
		}
		find = SeekBook(temp,root);
		if( find.child !=NULL )
		{
			printf(HEADER);
			YE_print( find.child->book );
			puts("书号不能重复,请重输(空行退出)");
			continue;
		}
		printf("输入书名(限%d个字符):\n",x=MAXTITL-1);
		while( YE_gets(temp.title,MAXTITL,stdin ) )
		{
			if( temp.title[0]=='\0' )
				puts("该项不能为空");
			else
				break;
		}
		printf("输入作者名(限%d个字符):\n",x=MAXAUTL-1);
		while( YE_gets(temp.author,MAXAUTL,stdin) )
		{
			if( temp.author[0]=='\0' )
				puts("该项不能为空");
			else
				break;
		}
		puts("输入价格(元):");
		while( scanf_s("%f",&temp.value)!=1||temp.value<=0 )
		{
			YE_clean();
			puts("价格为正数");
			continue;
		}
		YE_clean();
		puts("输入馆藏:");
		while( scanf_s("%d",&temp.repertory)!=1||temp.repertory<0 )
		{
			YE_clean();
			puts("馆藏为自然数");
			continue;
		}
		YE_clean();
		printf(HEADER);
		YE_print( temp );
		puts("[Y]es确认\t[N]o取消");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			if( strchr("YN",mode)&&YE_clean()==1 )
				break;
			else
			{
				puts("请输入对应的字母");
					continue;
			}
		}
		if( mode=='Y' )
		{
			pnew = (Node *)YE_alloc( 1 , sizeof(Node) );
			Initialize( pnew );
			pnew->book = temp;
			AddNode( pnew , root );
			n_book++;
			n_file++;
			fwrite( &temp , YE_size , 1 , fp );
			if( n_file==MAXBKS )
			{
				puts("书目种类已达上限");
				break;
			}
		}
		if( n_file<MAXBKS )
			printf("下一个书号(限%d个字符)(空行退出):\n",x=MAXID-1);
	}
	system("CLS");
	YE_fclose(fp);
	if( n_book !=0 )
	{
		fp = YE_fopen( file_name , "rb" );
		if( Library!=NULL )
			free(Library);
		Library = (BOOK *)YE_alloc( n_file , YE_size );
		fread( Library , YE_size , n_file , fp );YE_fclose(fp);
	}
	return n_file;
}


//功能描述：在二叉树中删除一个节点，并重写文件
//输入参数：一个文件名，一个树的地址，一个书目数量值
//返回值  ：书目种类的最终数量
//其他说明：若书目数为0，则返回
int Delete( char file_name[] , Tree *root , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return n_file;
	}
	system("CLS");
	Pair find;
	memset( &find , 0 , sizeof(Pair) );
	BOOK Temp;
	memset( &Temp , 0 , YE_size );
	FILE * fp = NULL;
	char mode = 0;
	int num_dfile = 0;
	puts("输入书的书号(空行结束):");
	while( YE_gets(Temp.ID , MAXID,stdin) && Temp.ID[0]!='\0' )
	{
		find = SeekBook( Temp , root );
		if( find.child == NULL )
		{
			puts("查无此书，请重新输入书号(空行退出):");
			continue;
		}
		else
		{
			printf(HEADER);
			YE_print( find.child->book );
		}
		puts("[Y]es确认删除\t[N]o取消");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			YE_clean();
			if( strchr("YN",mode) )
				break;
			else
			{
				puts("请输入对应的字母");
					continue;
			}
		}
		if( mode=='Y' )
		{
			if( find.parent == NULL )
				DeleteNode( root );
			else if( find.parent->left==find.child )
				DeleteNode( &find.parent->left );
			else
				DeleteNode( &find.parent->right );
			num_dfile++;
			n_file--;
		}
		if( n_file==0 )
		{
			puts("书目已全部删除");
			break;
		}
		else
			puts("下一个书号(空行退出)");
	}
	system("CLS");
	if( num_dfile!=0 )
	{
		fp = YE_fopen( file_name , "w+b" );
		Traverse( fp , root , "w+b" );
		YE_fclose( fp );
		fp = YE_fopen( file_name , "rb" );
		if( Library!=NULL )
			free(Library);
		Library = (BOOK *)YE_alloc( n_file , YE_size );
		fread( Library , YE_size , n_file , fp );
		YE_fclose( fp );
	}
	return n_file;
}


//功能描述：修改二叉树节点的信息，并修改文件
//输入参数：一个文件名，一个树的地址，一个书目数量值
//其他说明：若书目数为0，则返回
void Modify( char file_name[] , Tree *root , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return;
	}
	system("CLS");
	int x = 0;
	int item = 0;
	FILE * fp = NULL;
	BOOK temp;
	memset( &temp , 0 , YE_size );
	Pair find;
	memset( &find , 0 , sizeof(Pair) );
	Pair find_i;
	memset( &find_i , 0 , sizeof(Pair) );
	Node *pnew = NULL;
	fp = YE_fopen(file_name , "r+b");
	char mode = 0;
	puts("输入书的书号(空行结束):");
	while( YE_gets(temp.ID,MAXID,stdin) && temp.ID[0]!='\0' )
	{
		find = SeekBook( temp , root );
		if( find.child==NULL )
		{
			puts("查无此书,下一个书号(空行退出):");
			continue;
		}
		else
		{
			for( int i=0 ; i<n_file ; i++ )
			{
				if( strcmp(temp.ID,Library[i].ID)==0 )
				{
					item = i;
					fseek( fp , i*YE_size , SEEK_SET );
					break;
				}
			}
		}
		int is_modify = 0;
		int all_modify = 0;
		printf(HEADER);
		YE_print( find.child->book );
		puts("[T]itle改书名       [W]riter改作者名    [V]alue改价格    [I]D改书号");
		puts("[R]epertory改馆藏   [A]ll全改           [Q]uit取消");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			if( strchr("RTWVQAI",mode)&&YE_clean()==1 )
				break;
			else
			{
				puts("请输入对应的字母");
				continue;
			}
		}
		temp = find.child->book;
		switch(mode)
		{
		case 'A':
			all_modify = 1;
		case 'I':
			printf("输入书号(限%d个字符):\n",x=MAXID-1);
			while( YE_gets( temp.ID,MAXID,stdin) )
			{
				find_i = SeekBook(temp,root);
				if( find_i.child!=NULL )
				{
					printf(HEADER);
					YE_print( find_i.child->book );
					puts("书号不能重复");
					continue;
				}
				else
					break;
			}
			if( !all_modify )
				break;

		case 'T':
			printf("输入书名(限%d个字符):\n",x=MAXTITL-1);
			while( YE_gets(temp.title,MAXTITL,stdin) )
			{
				if( temp.title[0]=='\0' )
					puts("该项不能为空");
				else
					break;
			}
			if( !all_modify )
				break;

		case 'W':
			printf("输入作者名(限%d个字符):\n",x=MAXAUTL-1);
			while( YE_gets(temp.author , MAXAUTL,stdin) )
			{
				if( temp.author[0]=='\0' )
					puts("该项不能为空");
				else
					break;
			}
			if( !all_modify )
				break;

		case 'V':
			puts("输入价格:");
			while( scanf_s("%f",&temp.value)!=1||temp.value<=0 )
			{
				YE_clean();
				puts("价格为正数");
				continue;
			}
			YE_clean();
			if( !all_modify )
				break;

		case 'R':
			puts("输入馆藏:");
			while( scanf_s("%d",&temp.repertory)!=1||temp.repertory<0 )
			{
				YE_clean();
				puts("馆藏为自然数");
				continue;
			}
			YE_clean();
			break;
		}
		if( mode=='I'||mode=='A' )
		{
			Library[item] = temp;
			pnew = (Node *)YE_alloc( 1,sizeof(Node) );
			Initialize( pnew );
			pnew->book = temp;
			if( find.parent == NULL )
				DeleteNode( root );
			else if( find.parent->left==find.child )
				DeleteNode( &find.parent->left );
			else
				DeleteNode( &find.parent->right );
			AddNode( pnew , root );
		}
		else
			find.child->book = temp;
		if( mode!='Q' )
		{
			puts("这是现在此书的信息");
			printf(HEADER);
			YE_print(temp);
			fwrite( &temp , YE_size , 1 , fp );
		}
		puts("下一个书号(空行退出)");
	}
	YE_fclose(fp);
	system("CLS");
}


//功能描述：修改二叉树节点的书目馆藏信息，并修改文件
//输入参数：一个文件名，一个树的地址，一个书目数量值
//其他说明：若书目数为0，则返回
void Repertory( char file_name[] , Tree *root , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return;
	}
	system("CLS");
	isManage = 1;
	int quit = 0;
	char sure = 0;
	char mode = 0;
	FILE * fp = NULL;
	Pair find;
	memset( &find , 0 , sizeof(Pair) );
	BOOK Temp;
	memset(&Temp , 0 , YE_size );
	fp = YE_fopen(file_name , "r+b");
	puts("[B]orrow借  [R]eback还  [Q]uit退出");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("BRQ",mode)&&YE_clean()==1 )
		{
			switch(mode)
			{
			case 'B':case 'R':
				puts("输入书号(空行结束)");
				while(YE_gets( Temp.ID , MAXID , stdin )&&Temp.ID[0]!='\0')
				{
					find = SeekBook( Temp , root );
					if( find.child==NULL )
					{
						puts("查无此书,请重新输入书号(空行退出)");
						continue;
					}
					else
					{
						for( int i=0 ; i<n_file ; i++ )
						{
							if( strcmp( Temp.ID,Library[i].ID )==0 )
							{
								fseek( fp , i*YE_size , SEEK_SET );
								break;
							}
						}
					}
					printf(HEADER);
					YE_print( find.child->book );
					puts("[Y]es确认\t[N]o取消");
					while( scanf_s("%c",&sure) )
					{
						if( isspace(sure) )
							continue;
						if( strchr("YN",sure)&&YE_clean()==1 )
							break;
						else
						{
							puts("请输入对应的字母");
							continue;
						}
					}
					if( mode=='B'&& sure=='Y' )
					{
						if( find.child->book.repertory>0 )
						{
							find.child->book.repertory--;
							puts("借书成功");
						}
						else
							puts("此书已借完");
					}
					else if( mode=='R'&&sure=='Y' )
					{
						find.child->book.repertory++;
						puts("还书成功");					
					}				
					puts("下一个书号(空行结束)");
					fwrite( &(find.child->book) , YE_size , 1 , fp );
				}
				break;
			case 'Q':
				quit = 1;
			}
			if( quit )
				break;
		}
		else
		{
			puts("请输入对应的字母");
			continue;
		}
		puts("[B]orrow借  [R]eback还  [Q]uit退出");
	}
	system("CLS");
	YE_fclose(fp);
}


//功能描述：按价格排序文件中的书目，并显示在屏幕上
//输入参数：一个文件名，一个书目数量值
//其他说明：若书目数为0，则返回
void Value( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return;
	}
	system("CLS");
	char mode = 0;
	FILE *fp = NULL;
	BOOK * BOOKS = NULL;
	fp = YE_fopen( file_name , "rb" );
	BOOKS = (BOOK *)YE_alloc( n_file , YE_size);
	fread( BOOKS , YE_size , n_file , fp );
	puts("[H]igh价格由高到低  [L]ow价格由低到高");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("HL",mode)&&YE_clean()==1 )
			break;
		else
		{
			puts("请输入对应的字母");
				continue;
		}
	}
	if( mode=='L' )
		qsort( BOOKS , n_file , YE_size , V_LowToHigh );
	else
		qsort( BOOKS , n_file , YE_size , V_HighToLow );
	Show( BOOKS , n_file );
	free(BOOKS);
	YE_fclose(fp);
	system("PAUSE");
	system("CLS");
}


//功能描述：按书名排序文件中的书目，并显示在屏幕上
//输入参数：一个文件名，一个书目数量值
//其他说明：若书目数为0，则返回
void Title( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return;
	}
	system("CLS");
	char mode = 0;
	FILE *fp = NULL;
	BOOK * BOOKS = NULL;
	fp = YE_fopen( file_name , "r+" );
	BOOKS = (BOOK *)YE_alloc( n_file , YE_size);
	fread( BOOKS , YE_size , n_file , fp );
	puts("[O]rder书名正序  [R]eversed书名逆序");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("OR",mode)&&YE_clean()==1 )
			break;
		else
		{
			puts("请输入对应的字母");
				continue;
		}
	}
	if( mode=='O' )
		qsort( BOOKS , n_file , YE_size , T_Order );
	else
		qsort( BOOKS , n_file , YE_size , T_Reversed );
	Show( BOOKS , n_file );
	free(BOOKS);
	YE_fclose(fp);
	system("PAUSE");
	system("CLS");
}


//功能描述：按作者名排序文件中的书目，并显示在屏幕上
//输入参数：一个文件名，一个书目数量值
//其他说明：若书目数为0，则返回
void Writer( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return;
	}
	system("CLS");
	char mode = 0;
	FILE *fp = NULL;
	BOOK * BOOKS = NULL;
	fp = YE_fopen( file_name , "rb" );
	BOOKS = (BOOK *)YE_alloc( n_file , YE_size);
	fread( BOOKS , YE_size , n_file , fp );
	puts("[O]rder作者名正序  [R]eversed作者名逆序");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("OR",mode)&&YE_clean()==1 )
			break;
		else
		{
			puts("请输入对应的字母");
				continue;
		}
	}
	if( mode=='O' )
		qsort( BOOKS , n_file , YE_size , W_Order );
	else
		qsort( BOOKS , n_file , YE_size , W_Reversed );
	Show( BOOKS , n_file );
	free(BOOKS);
	YE_fclose(fp);
	system("PAUSE");
	system("CLS");
}


//功能描述：按书号排序文件中的书目，并显示在屏幕上
//输入参数：一个文件名，一个书目数量值
//其他说明：若书目数为0，则返回
void ID_ord( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("没有书");
		return;
	}
	system("CLS");
	char mode = 0;
	FILE *fp = NULL;
	BOOK * BOOKS = NULL;
	fp = YE_fopen( file_name , "rb" );
	BOOKS = (BOOK *)YE_alloc( n_file , YE_size);
	fread( BOOKS , YE_size , n_file , fp );
	puts("[O]rder书号正序  [R]eversed书号逆序");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("OR",mode)&&YE_clean()==1 )
			break;
		else
		{
			puts("请输入对应的字母");
				continue;
		}
	}
	if( mode=='O' )
		qsort( BOOKS , n_file , YE_size , I_Order);
	else
		qsort( BOOKS , n_file , YE_size , I_Reversed);
	Show( BOOKS , n_file );
	free(BOOKS);
	YE_fclose(fp);
	system("PAUSE");
	system("CLS");
}


//功能描述：更改书目种类上限
//输入参数：一个书目数量值
void Change_max( int n_file )
{
	system("CLS");
	char mode = 0;
	do
	{
		printf("当前书目种类上限为%d\n",MAXBKS);
		puts("[Y]es确认不改   [N]o更改");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			if( strchr("YN",mode)&&YE_clean()==1 )
				break;
			else
			{
				puts("请输入对应的字母");
					continue;
			}
		}
		if( mode=='N' )
		{
			puts("输入新上限:");
			while(1)
			{
				if( scanf_s("%d",&MAXBKS)!=1||MAXBKS<=0 )
				{
					YE_clean();
					puts("上限需为正整数");
					continue;
				}
				else
				{
					if( MAXBKS<n_file )
					{
						printf("上限不能低于已知的%d\n",n_file);
						continue;
					}
					else 
						break;
				}
			}
		}
	}while(mode != 'Y');
	system("CLS");
}


//功能描述：更改管理员密码
//输入参数：一个存放密码的字符串，一个密码上限
void Change_key( char key[] , int n )
{
	system("CLS");
	char  * password = NULL;
	password = (char *)YE_alloc( n , sizeof(char) );
	while(1)
	{
		printf("设置管理员密码(限%d个字符)\n",n-1);
		YE_gets( password , n , stdin );
		puts("确认密码");
		YE_gets( key , n , stdin );
		if( strcmp(key , password)==0 )
			break;
		else
			puts("密码前后不一致!请重输。");
	}
	free(password);
	system("CLS");
}


//功能描述：更改本系统的二进制文件，并重建二叉树
//输入参数：一个存储文件名的字符串，一个树的地址
//返回值  ：节点的数量
int Load( char  file_name[] , Tree *root )
{
	if( *root != NULL )
		FreeTree( root );
	int count = 0;
	char mode = 0;
	FILE *pbooks = NULL;
	BOOK Temp;
	memset( &Temp , 0 , YE_size );
	Node *library = NULL;
	system("CLS");
	puts("请放入二进制文件");
	while( YE_gets( file_name , 250 , stdin ) )
	{
		if( file_name[0]=='\0' )
			puts("该项不能为空");
		else
			break;
	}
	pbooks=YE_fopen( file_name, "r+b" );
	rewind(pbooks);
	while( count < MAXBKS )
	{
		if( fread(&Temp , YE_size , 1 , pbooks )==1 )
		{
			library = (Node *)YE_alloc( 1 , sizeof(Node) );
			Initialize( library );
			library->book = Temp;
			AddNode( library , root );
		}
		else
			break;
		count++;
		if( count==MAXBKS )
		{
			puts("书目种类已达上限，是否更改？");
			puts("[Y]es更改   [N]o下一步");
			while( scanf_s("%c",&mode) )
			{
				if( isspace(mode) )
					continue;
				if( strchr("YN",mode)&&YE_clean()==1 )
					break;
				else
				{
					puts("请输入对应的字母");
					continue;
				}
			}
			if( mode=='Y' )
				Change_max( count );
		}
	}
	rewind(pbooks);
	if( Library!=NULL )
		free(Library);
	Library = (BOOK *)YE_alloc( count , YE_size );
	fread( Library , YE_size , count , pbooks );
	YE_fclose(pbooks);
	system("CLS");
	return count;
}


//功能描述：显示整个书目结构数组的信息，并统计种类数和书的总数
//输入参数：一个书目结构数组的地址，一个书目数量值
void Show( BOOK library[] , int n_file )
{
	int sum_book = 0;
	puts("这是现在的书册");
	for( int i=0 ; i<n_file ; i++ )
	{
		if( i==0 )
			printf(HEADER);
		YE_print( library[i] );
		sum_book += library[i].repertory;
	}
	printf("共%d种  %d本书\n",n_file,sum_book);
}