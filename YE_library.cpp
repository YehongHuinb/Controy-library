#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "YE_library.h"


//������������һ���ļ������ж��Ƿ�򿪳ɹ�
//�������������һ���ļ�������һ�ִ�ģʽ
//����ֵ  �����򿪳ɹ��򷵻�һ���ļ�ָ��
//����˵������ʧ����ִ�к���(Fopen_Failure)�����жϳ���
void Fopen_Failure( void )
{
	fputs("���ļ�ʧ��\n",stderr);
}
FILE * YE_fopen( const char file_name[] , const char *mode )
{
	FILE * fp = NULL;
	fp = fopen( file_name , mode );
	if( fp == NULL )
	{
		atexit(Fopen_Failure);
		exit(EXIT_SUCCESS);
	}
	return fp;
}


//�����������ر�һ���ļ������ж��Ƿ�򿪳ɹ�
//�������������һ����ָ����ļ����ļ�ָ��
//����ֵ  �����رճɹ��򷵻���������0
//����˵�����ر�ʧ����ִ�к���(Fclose_Failure)�����жϳ���
void Fclose_Failure( void )
{
	fputs("�ر��ļ�ʧ��\n",stderr);
}
int YE_fclose( FILE * fp )
{
	int check = 0;
	check = fclose( fp );
	if( check!=0 )
	{
		atexit(Fclose_Failure);
		exit(EXIT_SUCCESS);
	}
	return check;
}


//��������������ָ����С���ڴ棬������Ƿ����ɹ�
//������������շ����ڴ�������ʹ�С
//����ֵ  ��������ɹ����򷵻�ָ����ڴ��ָ��
//����˵��������ʧ����ִ�к���(Alloc_Failure)�����жϳ���
void Alloc_Failure( void )
{
	fputs("�����ڴ�ʧ��\n",stderr);
}
void *YE_alloc( unsigned int count , unsigned int size )
{
	void *Temp = NULL;
	Temp = (void*)calloc( count , size );
	if( Temp == NULL )
	{
		atexit(Alloc_Failure);
		exit(EXIT_SUCCESS);
	}
	return Temp;
}

//�������������������������㱻������ַ���
//����ֵ  ��������ַ���
int YE_clean(void)
{
	int i = 1;
	while( getchar()!='\n' )
		i++;
	return i;
}


//������������ָ���ַ������͹涨�ļ�����һ���ַ���������Ƿ��нض�
//�������������һ���ַ�����ַ��һ��ָ���ַ�����һ���ļ�ָ��
//����ֵ  ���ַ����õ�����������������ָ����ַ�����ַ��ָ��
//����˵�������л��з�����'\0'���棬��������ضϾ����ʧ�ܾ���
//          �����û�����
char * YE_gets(char st[] , int n , FILE *fp)
{
	char * ret_val = NULL;
	char * temp = NULL;
	char * find = NULL;
	temp = (char *)YE_alloc( n+1 , sizeof(char) );
	while(1)
	{
		ret_val = fgets(temp , n+1 , fp);
		if( ret_val )
		{
			find = strchr(temp , '\n');
			if( find )
				*find = '\0';
			else
			{
				fputs("\a*****���뱻�ض�!������!*****\n",stderr);
				YE_clean();
				continue;
			}
			strncpy( st , temp , n );
			st[n-1] = '\0';
			break;
		}
		else
			fputs("\a*****����ʧ��!������!*****\n",stderr);
	}
	ret_val = st;
	free(temp);
	return ret_val;
}


//�������������涨�������Ļ�ϴ�ӡbook����
//�������������һ��BOOK����
void YE_print( BOOK library )
{
	printf("|%-10s|%-20s|%-20s|%-8.2f|%-6d\n",
		library.ID , library.title , library.author,
		library.value ,library.repertory);
}


//�������������涨�����ָ���ļ��ϴ�ӡbook����
//�������������һ����ָ���ļ����ļ�ָ�룬��һ��BOOK����
void YE_fprint( FILE * fp , BOOK library )
{
	fprintf(fp , "|%-10s|%-20s|%-20s|%-8.2f|%-6d\n",
		library.ID , library.title , library.author,
		library.value ,library.repertory);
}


//������������ʼ��һ���ڵ��ָ���Ա
//�������������һ���ڵ�ĵ�ַ
void Initialize( Node * library )
{
	library->left = NULL;
	library->right = NULL;
}


//�����������ж��ӽڵ��븸�ڵ�涨��Ĵ�С
//�����������������BOOK����
//����ֵ  ����ǰ�ߵ�ָ����Ա���ں��ߵģ�����1��
//          ���򷵻�0
int ToLeft( const BOOK parent , const BOOK child )
{
	if( strcmp(parent.ID,child.ID)>0 )
		return 1;
	else
		return 0;
}


//�����������ж��ӽڵ��븸�ڵ�涨��Ĵ�С
//�����������������BOOK����
//����ֵ  ����ǰ�ߵ�ָ����ԱС�ں��ߵģ�����1��
//          ���򷵻�0
int ToRight( const BOOK parent , const BOOK child )
{
	if( strcmp(parent.ID,child.ID)<0 )
		return 1;
	else
		return 0;
}


//������������ָ���ļ���ָ��ģʽ��������������
//�������������һ����ָ���ļ����ļ�ָ�룬һ�����ĵ�ַ��
//          ��һ��ָ���ַ���
//����˵����"w+"ָ��->��Ŀ���ļ����ı�ģʽ��ӡ��������
//          "w+b"ָ��->��Ŀ���ļ���������ģʽ��ӡ������
void Traverse( FILE * fp , Tree *ptree , char *mode )
{
	if( ptree != NULL )
		InOrder( fp , *ptree ,mode);
}
void InOrder( FILE * fp , Node * root , char *mode)
{
	if( root != NULL )
	{
		InOrder( fp , root->left ,mode);
		if( strcmp(mode,"w+")==0 )
			YE_fprint(fp, root->book );
		else if( strcmp(mode,"w+b")==0 )
			fwrite(&(root->book),YE_size,1,fp);
		InOrder( fp , root->right ,mode);
	}
}


//����������������Ѱ�ҹ涨��Ϣ��Ŀ��ڵ���ͬ�Ľڵ�
//�������������һ��BOOK��������һ�����ĵ�ַ
//����ֵ  �����������ҵ���Ŀ������Ĺ涨��Ϣ��ͬ�Ľڵ㣬
//          �򷵻�����ڵ�ʹ˸��ڵ�ĵ�ַ�����򷵻�NULL
Pair SeekBook( const BOOK pride , const Tree *root )
{
	Pair find;
	find.parent = NULL;
	find.child = *root;
	while( find.child != NULL )
	{
		if( ToLeft(find.child->book,pride) )
		{
			find.parent = find.child;
			find.child = find.child->left;
		}
		else if( ToRight(find.child->book,pride) )
		{
			find.parent = find.child;
			find.child = find.child->right;
		}
		else
			break;
	}
	return find;
}


//�������������������һ���ڵ�
//�������������һ���ڵ�ĵ�ַ����һ�����ĵ�ַ
void AddNode( Node *pnew , Tree *root )
{
	Node * temp = NULL;
	temp = *root;
	if( *root==NULL )
	{
		*root = pnew;
		return;
	}
	while(1)
	{
		if( strcmp(temp->book.ID,pnew->book.ID)==0 )
		{
			fputs("���ʱ��Ӧ������ͬ��ŵ���!\n",stderr);
			exit(EXIT_SUCCESS);
		}

		if( ToLeft(temp->book,pnew->book) )
		{
			if( temp->left==NULL )
			{
				temp->left = pnew;
				break;
			}
			else
			{
				temp = temp->left;
				continue;
			}
		}
		else if( ToRight(temp->book,pnew->book) )
		{
			if( temp->right==NULL )
			{
				temp->right = pnew;
				break;
			}
			else
			{
				temp = temp->right;
				continue;
			}
		}
	}
}


//����������ɾ��һ���ڵ㣬��������
//�������������һ��ָ��ڵ��ָ��ĵ�ַ
void DeleteNode( Node ** ptr )
{
	Node * temp = NULL;
	if( (*ptr)->left==NULL )
	{
		temp = *ptr;
		*ptr = (*ptr)->right;
	}
	else if( (*ptr)->right==NULL )
	{
		temp = *ptr;
		*ptr = (*ptr)->left;
	}
	else
	{
		for( temp=(*ptr)->left ; temp->right!=NULL ; temp=temp->right )
			continue;
		temp->right = (*ptr)->right;
		temp = *ptr;
		*ptr = (*ptr)->left;
	}
	free(temp);
}


//�����������ͷ��������������ڴ�
//�������������һ�����ĵ�ַ
void FreeTree( Tree * ptree )
{
	if( ptree!=NULL )
		FreeAllNodes( *ptree );
	*ptree = NULL;
}
void FreeAllNodes( Node * root )
{
	Node * prev = NULL;
	if( root!=NULL )
	{
		prev = root->right;
		FreeAllNodes( root->left );
		free( root );
		FreeAllNodes( prev );
	}
}


//�����������۸�������qsort�����һ������
//����˵����ǰ��Ϊ�ɵ͵��ߣ�����Ϊ�ɸߵ���
int V_LowToHigh( const void* p1 , const void* p2 )
{
	const struct book *a1 = (const struct book *)p1;
	const struct book *a2 = (const struct book *)p2;
	if( a1->value<a2->value )
		return -1;
	else if( a1->value==a2->value )
		return 0;
	else
		return 1;
}
int V_HighToLow( const void* p1 , const void* p2 )
{
	const struct book *a1 = (const struct book *)p1;
	const struct book *a2 = (const struct book *)p2;
	if( a1->value>a2->value )
		return -1;
	else if( a1->value==a2->value )
		return 0;
	else
		return 1;
}


//�������������������qsort�����һ������
//����˵����ǰ��Ϊ˳�򣬺���Ϊ����
int I_Order( const void* p1 , const void* p2 )
{
	const struct book* a1 = (const struct book*)p1;
	const struct book* a2 = (const struct book*)p2;
	if( strcmp(a1->ID,a2->ID)<0 )
		return -1;
	else if( strcmp(a1->ID,a2->ID)==0 )
		return 0;
	else
		return 1;
}
int I_Reversed( const void* p1 , const void* p2 )
{
	const struct book* a1 = (const struct book*)p1;
	const struct book* a2 = (const struct book*)p2;
	if( strcmp(a1->ID,a2->ID)>0 )
		return -1;
	else if( strcmp(a1->ID,a2->ID)==0 )
		return 0;
	else
		return 1;
}


//��������������������qsort�����һ������
//����˵����ǰ��Ϊ˳�򣬺���Ϊ����
int T_Order( const void* p1 , const void* p2 )
{
	const struct book* a1 = (const struct book*)p1;
	const struct book* a2 = (const struct book*)p2;
	if( strcmp(a1->title,a2->title)<0 )
		return -1;
	else if( strcmp(a1->title,a2->title)==0 )
		return 0;
	else
		return 1;
}
int T_Reversed( const void* p1 , const void* p2 )
{
	const struct book* a1 = (const struct book*)p1;
	const struct book* a2 = (const struct book*)p2;
	if( strcmp(a1->title,a2->title)>0 )
		return -1;
	else if( strcmp(a1->title,a2->title)==0 )
		return 0;
	else
		return 1;
}


//����������������������qsort�����һ������
//����˵����ǰ��Ϊ˳�򣬺���Ϊ����
int W_Order( const void* p1 , const void* p2 )
{
	const struct book* a1 = (const struct book*)p1;
	const struct book* a2 = (const struct book*)p2;
	if( strcmp(a1->author,a2->author)<0 )
		return -1;
	else if( strcmp(a1->author,a2->author)==0 )
		return 0;
	else
		return 1;
}
int W_Reversed( const void* p1 , const void* p2 )
{
	const struct book* a1 = (const struct book*)p1;
	const struct book* a2 = (const struct book*)p2;
	if( strcmp(a1->author,a2->author)>0 )
		return -1;
	else if( strcmp(a1->author,a2->author)==0 )
		return 0;
	else
		return 1;
}