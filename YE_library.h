#ifndef LIBRARY_H_
#define LIBRARY_H_


#define MAXID   11//�����߹�ϵ����д�ļ�
#define MAXTITL 21//������Ҫ�޸�
#define MAXAUTL 21//�������޸ģ���ע�����ͷҪ��ͬ�޸�
#define MAXKEY  11
#define HEADER "|���------|����----------------|����----------------|�۸�----|�ݲ�--\n"//�涨�ı�ͷ
#define YE_size  sizeof(struct book)//��Ŀ�Ĵ�С


//��Ŀ����Ϣ
typedef struct book{
	char ID[MAXID];       //���
	char title[MAXTITL];  //����
	char author[MAXAUTL]; //������
	float value;          //�۸�
	int repertory;        //�ݲ�
}BOOK;


//�ڵ�����Ŀ��Ϣ������
typedef struct node{
	BOOK book;           //��Ŀ��Ϣ
	struct node * left;  //ָ����ڵ��ָ��
	struct node * right; //ָ���ҽڵ��ָ��
}Node;


//��ԱΪ���ӽڵ��ַ��Ϣ�Ľṹ�����ڶ���������
typedef struct pair{
	struct node * parent;//ָ�򸸽ڵ��ָ��
	struct node * child; //ָ���ӽڵ��ָ��
}Pair;


//���ĵ�ַ�ڴ�ϵͳ����Ϊ��
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