#ifndef LIBRARY_H_
#define LIBRARY_H_


#define MAXID   11//这三者关系到读写文件
#define MAXTITL 21//尽量不要修改
#define MAXAUTL 21//若必须修改，需注意表格表头要连同修改
#define MAXKEY  11
#define HEADER "|书号------|书名----------------|作者----------------|价格----|馆藏--\n"//规定的表头
#define YE_size  sizeof(struct book)//书目的大小


//书目的信息
typedef struct book{
	char ID[MAXID];       //书号
	char title[MAXTITL];  //书名
	char author[MAXAUTL]; //作者名
	float value;          //价格
	int repertory;        //馆藏
}BOOK;


//节点是书目信息的载体
typedef struct node{
	BOOK book;           //书目信息
	struct node * left;  //指向左节点的指针
	struct node * right; //指向右节点的指针
}Node;


//成员为父子节点地址信息的结构，用于二叉树查找
typedef struct pair{
	struct node * parent;//指向父节点的指针
	struct node * child; //指向子节点的指针
}Pair;


//根的地址在此系统定义为树
typedef Node * Tree;


void Fopen_Failure( void );
FILE * YE_fopen( const char file_name[] , const char *mode );


void Fclose_Failure( void );
int YE_fclose( FILE * fp );


void Alloc_Failure( void );
void *YE_alloc( unsigned int count , unsigned int size );

int YE_clean(void);

char * YE_gets( char st[] , int n , FILE *fp );


void YE_print( struct book library );


void YE_fprint( FILE * fp , struct book library );


void Initialize( Node * library );


int ToLeft( const BOOK parent , const BOOK child );


int ToRight( const BOOK parent , const BOOK child );


void Traverse( FILE * fp , Tree *root , char *mode);
void InOrder( FILE * fp , Node * root , char *mode);


Pair SeekBook( const BOOK pride , const Tree *root );


void AddNode( Node *pnew , Tree *root );


void DeleteNode( Node ** ptr );


void FreeTree( Tree * ptree );
void FreeAllNodes( Node * root );


int V_HighToLow( const void* p1 , const void* p2 );
int V_LowToHigh( const void* p1 , const void* p2 );


int I_Order( const void* p1 , const void* p2 );
int I_Reversed( const void* p1 , const void* p2 );


int W_Order( const void* p1 , const void* p2 );
int W_Reversed( const void* p1 , const void* p2 );


int T_Order( const void* p1 , const void* p2 );
int T_Reversed( const void* p1 , const void* p2 );



#endif