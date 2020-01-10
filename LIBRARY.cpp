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
	strcat( file_user , "(�ı�).txt" );
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
	puts("�˴��ļ���������");
	return 0;
}


//������������ͨ�û���Ȩ�޿��Ƶ����˵���������������
//���������һ���ļ�����һ�����ĵ�ַ��һ����Ŀ����ֵ
void Menu( char file_name[] , Tree *root ,int *count )
{
	system("CLS");
	int quit = 0;
	char mode = 0;
	while( 1 )
	{
		puts("�������Ӧ�Ĵ�д��ĸ:");
		puts("[F]ind����    [B]andR�軹���    [R]and����Ƽ�    [M]anager����Ա����");
		puts("[Q]uit�˳�");
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
			puts("��������ȷ��ѡ��");
			continue;
		}
		if( quit )
			break;
	}
}


//��������������Ա��Ȩ�޿��ƵĲ˵�����ɾ�Ĳ麯�������
//���������һ���ļ�����һ�����ĵ�ַ��һ����Ŀ����ֵ
void Manager( char file_name[] , Tree *root , int *count )
{
	system("CLS");
	int quit = 0;
	char password[MAXKEY] = {0};
	FILE * fp = NULL;
	puts("������������֤���(�����˳�)");
	while( YE_gets(password , MAXKEY , stdin)&&password[0]!='\0' )
	{
		if( strcmp( password , Key )==0 )
		{
			system("CLS");
			isManage = 1;
			char mode = 0;
			puts("[V]alue�۸�����   [T]itle����������  [W]riter������������  [I]D���������");
			puts("[D]eleteɾ��      [A]ppend���       [M]odify�޸�          [C]hange����");
			puts("[K]ey������       [L]oad�����ļ�     [Q]uit�������˵�");
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
					puts("�������Ӧ����ĸ");
					continue;
				}
				if( quit )
					break;
				puts("[V]alue�۸�����   [T]itle����������  [W]riter������������  [I]D���������");
				puts("[D]eleteɾ��      [A]ppend���       [M]odify�޸�          [C]hange����");
				puts("[K]ey������       [L]oad�����ļ�     [Q]uit�������˵�");
			}
			if( quit )
			{
				quit = 0;
				break;
			}
		}
		else
		{
			puts("��֤ʧ��");
			puts("���������룬������˳�");
		}
	}
	system("CLS");
}


//���������������ʾ�ļ��е�һ����Ŀ��Ϣ
//���������һ���ļ�����һ����Ŀ����ֵ
//����˵��������Ŀ��Ϊ0���򷵻�
void Rand( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
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
	puts("�س�������");
	getchar();
	system("CLS");
}


//�������������ļ��в������û��������Ϣһ�µ�����
//���������һ���ļ�����һ�����ĵ�ַ��һ����Ŀ����ֵ
//����˵��������Ŀ��Ϊ0���򷵻�
//          �������������ǹؼ��ֲ��ң�����Ƕ���������
void Find( char file_name[] , Tree *root , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
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
		puts("[T]itle����������  [W]riter������������  [I]D����Ų���   [Q]uit�˳�");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			if( strchr("TWQI",mode)&&YE_clean()==1 )
				break;
			else
			{
				puts("�������Ӧ����ĸ");
				continue;
			}
		}
		switch(mode)
		{
		case 'T':
			puts("��������");
			while(YE_gets( Temp.title , MAXTITL , stdin ))
			{
				if( Temp.title[0]=='\0' )
					puts("�����Ϊ��");
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
			puts("����������");
			while(YE_gets( Temp.author , MAXAUTL , stdin ))
			{
				if( Temp.author[0]=='\0' )
					puts("�����Ϊ��");
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
			puts("�������");
			while( YE_gets( Temp.ID , MAXID , stdin ) )
			{
				if( Temp.ID[0]=='\0' )
					puts("�����Ϊ��");
				else
					break;
			}
			find = SeekBook( Temp , root );
			if( find.child==NULL )
				puts("���޴���");
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
			puts("���޴���");
		else if( mode!='I' )
		{
			printf("�ҵ�%d�������Ҫ��\n",n_find);
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


//�����������ڶ����������һ���ڵ㣬�����ļ�ĩβ�����Ŀ��Ϣ
//���������һ���ļ�����һ�����ĵ�ַ��һ����Ŀ����ֵ
//����ֵ  ����Ŀ�������������
//����˵��������Ŀ���ﵽ���ޣ��򷵻ء�
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
		fputs("�ļ�����.\n",stderr);
		return n_file;
	}
	fp = YE_fopen( file_name , "a+b" );
	puts("��ʼ¼��������:");
	printf("����������(�޸�%d�ַ�)(���н���):\n",x=MAXID-1);
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
			puts("��Ų����ظ�,������(�����˳�)");
			continue;
		}
		printf("��������(��%d���ַ�):\n",x=MAXTITL-1);
		while( YE_gets(temp.title,MAXTITL,stdin ) )
		{
			if( temp.title[0]=='\0' )
				puts("�����Ϊ��");
			else
				break;
		}
		printf("����������(��%d���ַ�):\n",x=MAXAUTL-1);
		while( YE_gets(temp.author,MAXAUTL,stdin) )
		{
			if( temp.author[0]=='\0' )
				puts("�����Ϊ��");
			else
				break;
		}
		puts("����۸�(Ԫ):");
		while( scanf_s("%f",&temp.value)!=1||temp.value<=0 )
		{
			YE_clean();
			puts("�۸�Ϊ����");
			continue;
		}
		YE_clean();
		puts("����ݲ�:");
		while( scanf_s("%d",&temp.repertory)!=1||temp.repertory<0 )
		{
			YE_clean();
			puts("�ݲ�Ϊ��Ȼ��");
			continue;
		}
		YE_clean();
		printf(HEADER);
		YE_print( temp );
		puts("[Y]esȷ��\t[N]oȡ��");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			if( strchr("YN",mode)&&YE_clean()==1 )
				break;
			else
			{
				puts("�������Ӧ����ĸ");
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
				puts("��Ŀ�����Ѵ�����");
				break;
			}
		}
		if( n_file<MAXBKS )
			printf("��һ�����(��%d���ַ�)(�����˳�):\n",x=MAXID-1);
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


//�����������ڶ�������ɾ��һ���ڵ㣬����д�ļ�
//���������һ���ļ�����һ�����ĵ�ַ��һ����Ŀ����ֵ
//����ֵ  ����Ŀ�������������
//����˵��������Ŀ��Ϊ0���򷵻�
int Delete( char file_name[] , Tree *root , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
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
	puts("����������(���н���):");
	while( YE_gets(Temp.ID , MAXID,stdin) && Temp.ID[0]!='\0' )
	{
		find = SeekBook( Temp , root );
		if( find.child == NULL )
		{
			puts("���޴��飬�������������(�����˳�):");
			continue;
		}
		else
		{
			printf(HEADER);
			YE_print( find.child->book );
		}
		puts("[Y]esȷ��ɾ��\t[N]oȡ��");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			YE_clean();
			if( strchr("YN",mode) )
				break;
			else
			{
				puts("�������Ӧ����ĸ");
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
			puts("��Ŀ��ȫ��ɾ��");
			break;
		}
		else
			puts("��һ�����(�����˳�)");
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


//�����������޸Ķ������ڵ����Ϣ�����޸��ļ�
//���������һ���ļ�����һ�����ĵ�ַ��һ����Ŀ����ֵ
//����˵��������Ŀ��Ϊ0���򷵻�
void Modify( char file_name[] , Tree *root , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
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
	puts("����������(���н���):");
	while( YE_gets(temp.ID,MAXID,stdin) && temp.ID[0]!='\0' )
	{
		find = SeekBook( temp , root );
		if( find.child==NULL )
		{
			puts("���޴���,��һ�����(�����˳�):");
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
		puts("[T]itle������       [W]riter��������    [V]alue�ļ۸�    [I]D�����");
		puts("[R]epertory�Ĺݲ�   [A]llȫ��           [Q]uitȡ��");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			if( strchr("RTWVQAI",mode)&&YE_clean()==1 )
				break;
			else
			{
				puts("�������Ӧ����ĸ");
				continue;
			}
		}
		temp = find.child->book;
		switch(mode)
		{
		case 'A':
			all_modify = 1;
		case 'I':
			printf("�������(��%d���ַ�):\n",x=MAXID-1);
			while( YE_gets( temp.ID,MAXID,stdin) )
			{
				find_i = SeekBook(temp,root);
				if( find_i.child!=NULL )
				{
					printf(HEADER);
					YE_print( find_i.child->book );
					puts("��Ų����ظ�");
					continue;
				}
				else
					break;
			}
			if( !all_modify )
				break;

		case 'T':
			printf("��������(��%d���ַ�):\n",x=MAXTITL-1);
			while( YE_gets(temp.title,MAXTITL,stdin) )
			{
				if( temp.title[0]=='\0' )
					puts("�����Ϊ��");
				else
					break;
			}
			if( !all_modify )
				break;

		case 'W':
			printf("����������(��%d���ַ�):\n",x=MAXAUTL-1);
			while( YE_gets(temp.author , MAXAUTL,stdin) )
			{
				if( temp.author[0]=='\0' )
					puts("�����Ϊ��");
				else
					break;
			}
			if( !all_modify )
				break;

		case 'V':
			puts("����۸�:");
			while( scanf_s("%f",&temp.value)!=1||temp.value<=0 )
			{
				YE_clean();
				puts("�۸�Ϊ����");
				continue;
			}
			YE_clean();
			if( !all_modify )
				break;

		case 'R':
			puts("����ݲ�:");
			while( scanf_s("%d",&temp.repertory)!=1||temp.repertory<0 )
			{
				YE_clean();
				puts("�ݲ�Ϊ��Ȼ��");
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
			puts("�������ڴ������Ϣ");
			printf(HEADER);
			YE_print(temp);
			fwrite( &temp , YE_size , 1 , fp );
		}
		puts("��һ�����(�����˳�)");
	}
	YE_fclose(fp);
	system("CLS");
}


//�����������޸Ķ������ڵ����Ŀ�ݲ���Ϣ�����޸��ļ�
//���������һ���ļ�����һ�����ĵ�ַ��һ����Ŀ����ֵ
//����˵��������Ŀ��Ϊ0���򷵻�
void Repertory( char file_name[] , Tree *root , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
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
	puts("[B]orrow��  [R]eback��  [Q]uit�˳�");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("BRQ",mode)&&YE_clean()==1 )
		{
			switch(mode)
			{
			case 'B':case 'R':
				puts("�������(���н���)");
				while(YE_gets( Temp.ID , MAXID , stdin )&&Temp.ID[0]!='\0')
				{
					find = SeekBook( Temp , root );
					if( find.child==NULL )
					{
						puts("���޴���,�������������(�����˳�)");
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
					puts("[Y]esȷ��\t[N]oȡ��");
					while( scanf_s("%c",&sure) )
					{
						if( isspace(sure) )
							continue;
						if( strchr("YN",sure)&&YE_clean()==1 )
							break;
						else
						{
							puts("�������Ӧ����ĸ");
							continue;
						}
					}
					if( mode=='B'&& sure=='Y' )
					{
						if( find.child->book.repertory>0 )
						{
							find.child->book.repertory--;
							puts("����ɹ�");
						}
						else
							puts("�����ѽ���");
					}
					else if( mode=='R'&&sure=='Y' )
					{
						find.child->book.repertory++;
						puts("����ɹ�");					
					}				
					puts("��һ�����(���н���)");
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
			puts("�������Ӧ����ĸ");
			continue;
		}
		puts("[B]orrow��  [R]eback��  [Q]uit�˳�");
	}
	system("CLS");
	YE_fclose(fp);
}


//�������������۸������ļ��е���Ŀ������ʾ����Ļ��
//���������һ���ļ�����һ����Ŀ����ֵ
//����˵��������Ŀ��Ϊ0���򷵻�
void Value( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
		return;
	}
	system("CLS");
	char mode = 0;
	FILE *fp = NULL;
	BOOK * BOOKS = NULL;
	fp = YE_fopen( file_name , "rb" );
	BOOKS = (BOOK *)YE_alloc( n_file , YE_size);
	fread( BOOKS , YE_size , n_file , fp );
	puts("[H]igh�۸��ɸߵ���  [L]ow�۸��ɵ͵���");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("HL",mode)&&YE_clean()==1 )
			break;
		else
		{
			puts("�������Ӧ����ĸ");
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


//���������������������ļ��е���Ŀ������ʾ����Ļ��
//���������һ���ļ�����һ����Ŀ����ֵ
//����˵��������Ŀ��Ϊ0���򷵻�
void Title( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
		return;
	}
	system("CLS");
	char mode = 0;
	FILE *fp = NULL;
	BOOK * BOOKS = NULL;
	fp = YE_fopen( file_name , "r+" );
	BOOKS = (BOOK *)YE_alloc( n_file , YE_size);
	fread( BOOKS , YE_size , n_file , fp );
	puts("[O]rder��������  [R]eversed��������");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("OR",mode)&&YE_clean()==1 )
			break;
		else
		{
			puts("�������Ӧ����ĸ");
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


//�����������������������ļ��е���Ŀ������ʾ����Ļ��
//���������һ���ļ�����һ����Ŀ����ֵ
//����˵��������Ŀ��Ϊ0���򷵻�
void Writer( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
		return;
	}
	system("CLS");
	char mode = 0;
	FILE *fp = NULL;
	BOOK * BOOKS = NULL;
	fp = YE_fopen( file_name , "rb" );
	BOOKS = (BOOK *)YE_alloc( n_file , YE_size);
	fread( BOOKS , YE_size , n_file , fp );
	puts("[O]rder����������  [R]eversed����������");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("OR",mode)&&YE_clean()==1 )
			break;
		else
		{
			puts("�������Ӧ����ĸ");
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


//��������������������ļ��е���Ŀ������ʾ����Ļ��
//���������һ���ļ�����һ����Ŀ����ֵ
//����˵��������Ŀ��Ϊ0���򷵻�
void ID_ord( char file_name[] , int n_file )
{
	if( n_file==0 )
	{
		puts("û����");
		return;
	}
	system("CLS");
	char mode = 0;
	FILE *fp = NULL;
	BOOK * BOOKS = NULL;
	fp = YE_fopen( file_name , "rb" );
	BOOKS = (BOOK *)YE_alloc( n_file , YE_size);
	fread( BOOKS , YE_size , n_file , fp );
	puts("[O]rder�������  [R]eversed�������");
	while( scanf_s("%c",&mode) )
	{
		if( isspace(mode) )
			continue;
		if( strchr("OR",mode)&&YE_clean()==1 )
			break;
		else
		{
			puts("�������Ӧ����ĸ");
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


//����������������Ŀ��������
//���������һ����Ŀ����ֵ
void Change_max( int n_file )
{
	system("CLS");
	char mode = 0;
	do
	{
		printf("��ǰ��Ŀ��������Ϊ%d\n",MAXBKS);
		puts("[Y]esȷ�ϲ���   [N]o����");
		while( scanf_s("%c",&mode) )
		{
			if( isspace(mode) )
				continue;
			if( strchr("YN",mode)&&YE_clean()==1 )
				break;
			else
			{
				puts("�������Ӧ����ĸ");
					continue;
			}
		}
		if( mode=='N' )
		{
			puts("����������:");
			while(1)
			{
				if( scanf_s("%d",&MAXBKS)!=1||MAXBKS<=0 )
				{
					YE_clean();
					puts("������Ϊ������");
					continue;
				}
				else
				{
					if( MAXBKS<n_file )
					{
						printf("���޲��ܵ�����֪��%d\n",n_file);
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


//�������������Ĺ���Ա����
//���������һ�����������ַ�����һ����������
void Change_key( char key[] , int n )
{
	system("CLS");
	char  * password = NULL;
	password = (char *)YE_alloc( n , sizeof(char) );
	while(1)
	{
		printf("���ù���Ա����(��%d���ַ�)\n",n-1);
		YE_gets( password , n , stdin );
		puts("ȷ������");
		YE_gets( key , n , stdin );
		if( strcmp(key , password)==0 )
			break;
		else
			puts("����ǰ��һ��!�����䡣");
	}
	free(password);
	system("CLS");
}


//�������������ı�ϵͳ�Ķ������ļ������ؽ�������
//���������һ���洢�ļ������ַ�����һ�����ĵ�ַ
//����ֵ  ���ڵ������
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
	puts("�����������ļ�");
	while( YE_gets( file_name , 250 , stdin ) )
	{
		if( file_name[0]=='\0' )
			puts("�����Ϊ��");
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
			puts("��Ŀ�����Ѵ����ޣ��Ƿ���ģ�");
			puts("[Y]es����   [N]o��һ��");
			while( scanf_s("%c",&mode) )
			{
				if( isspace(mode) )
					continue;
				if( strchr("YN",mode)&&YE_clean()==1 )
					break;
				else
				{
					puts("�������Ӧ����ĸ");
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


//������������ʾ������Ŀ�ṹ�������Ϣ����ͳ�����������������
//���������һ����Ŀ�ṹ����ĵ�ַ��һ����Ŀ����ֵ
void Show( BOOK library[] , int n_file )
{
	int sum_book = 0;
	puts("�������ڵ����");
	for( int i=0 ; i<n_file ; i++ )
	{
		if( i==0 )
			printf(HEADER);
		YE_print( library[i] );
		sum_book += library[i].repertory;
	}
	printf("��%d��  %d����\n",n_file,sum_book);
}