#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "YE_library.h"


//功能描述：打开一个文件，并判断是否打开成功
//输入参数：接收一个文件名，和一种打开模式
//返回值  ：若打开成功则返回一个文件指针
//其他说明：打开失败则执行函数(Fopen_Failure)，并中断程序
void Fopen_Failure( void )
{
	fputs("打开文件失败\n",stderr);
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


//功能描述：关闭一个文件，并判断是否打开成功
//输入参数：接收一个已指向打开文件的文件指针
//返回值  ：若关闭成功则返回整型数字0
//其他说明：关闭失败则执行函数(Fclose_Failure)，并中断程序
void Fclose_Failure( void )
{
	fputs("关闭文件失败\n",stderr);
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


//功能描述：分配指定大小的内存，并检查是否分配成功
//输入参数：接收分配内存的数量和大小
//返回值  ：若分配成功，则返回指向该内存的指针
//其他说明：分配失败则执行函数(Alloc_Failure)，并中断程序
void Alloc_Failure( void )
{
	fputs("分配内存失败\n",stderr);
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

//功能描述：清理缓冲区，并计算被清理的字符数
//返回值  ：清理的字符数
int YE_clean(void)
{
	int i = 1;
	while( getchar()!='\n' )
		i++;
	return i;
}


//功能描述：按指定字符数，和规定文件输入一个字符，并检查是否有截断
//输入参数：接收一个字符串地址，一个指定字符数，一个文件指针
//返回值  ：字符串得到输入流，函数返回指向此字符串地址的指针
//其他说明：若有换行符则以'\0'代替，否则输出截断警告或失败警告
//          并让用户重输
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
				fputs("\a*****输入被截断!请重输!*****\n",stderr);
				YE_clean();
				continue;
			}
			strncpy( st , temp , n );
			st[n-1] = '\0';
			break;
		}
		else
			fputs("\a*****输入失败!请重输!*****\n",stderr);
	}
	ret_val = st;
	free(temp);
	return ret_val;
}


//功能描述：按规定框架在屏幕上打印book变量
//输入参数：接收一个BOOK变量
void YE_print( BOOK library )
{
	printf("|%-10s|%-20s|%-20s|%-8.2f|%-6d\n",
		library.ID , library.title , library.author,
		library.value ,library.repertory);
}


//功能描述：按规定框架在指定文件上打印book变量
//输入参数：接收一个已指向文件的文件指针，和一个BOOK变量
void YE_fprint( FILE * fp , BOOK library )
{
	fprintf(fp , "|%-10s|%-20s|%-20s|%-8.2f|%-6d\n",
		library.ID , library.title , library.author,
		library.value ,library.repertory);
}


//功能描述：初始化一个节点的指针成员
//输入参数：接收一个节点的地址
void Initialize( Node * library )
{
	library->left = NULL;
	library->right = NULL;
}


//功能描述：判断子节点与父节点规定项的大小
//输入参数：接收两个BOOK变量
//返回值  ：若前者的指定成员大于后者的，返回1，
//          否则返回0
int ToLeft( const BOOK parent , const BOOK child )
{
	if( strcmp(parent.ID,child.ID)>0 )
		return 1;
	else
		return 0;
}


//功能描述：判断子节点与父节点规定项的大小
//输入参数：接收两个BOOK变量
//返回值  ：若前者的指定成员小于后者的，返回1，
//          否则返回0
int ToRight( const BOOK parent , const BOOK child )
{
	if( strcmp(parent.ID,child.ID)<0 )
		return 1;
	else
		return 0;
}


//功能描述：往指定文件按指定模式处理整个二叉树
//输入参数：接收一个已指向文件的文件指针，一个树的地址，
//          和一个指令字符串
//其他说明："w+"指令->往目标文件按文本模式打印二叉树，
//          "w+b"指令->往目标文件按二进制模式打印二叉树
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


//功能描述：遍历树寻找规定信息与目标节点相同的节点
//输入参数：接收一个BOOK变量，和一个树的地址
//返回值  ：若在树中找到与目标变量的规定信息相同的节点，
//          则返回这个节点和此父节点的地址，否则返回NULL
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


//功能描述：往树中添加一个节点
//输入参数：接收一个节点的地址，和一个树的地址
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
			fputs("添加时不应含有相同书号的项!\n",stderr);
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


//功能描述：删除一个节点，并重连树
//输入参数：接收一个指向节点的指针的地址
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


//功能描述：释放整个二叉树的内存
//输入参数：接收一个树的地址
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


//功能描述：价格排序函数qsort的最后一个参数
//其他说明：前者为由低到高，后者为由高到低
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


//功能描述：书号排序函数qsort的最后一个参数
//其他说明：前者为顺序，后者为逆序
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


//功能描述：书名排序函数qsort的最后一个参数
//其他说明：前者为顺序，后者为逆序
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


//功能描述：作者名排序函数qsort的最后一个参数
//其他说明：前者为顺序，后者为逆序
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