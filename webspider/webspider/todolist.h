#ifndef TO_DO_LIST
#define TO_DO_LIST

#include "modules/list.h"


/* 1.δ����ͼƬ�ļ��б� */
/* 2.������ͼƬ�ļ��б� */
/* 3.δ����URL�ļ��б�  */
/* 4.������URL�ļ��б�  */

typedef struct  
{
	LIST * l;
}TO_LIST;

void todo_init(TO_LIST * list);
void todo_uninit(TO_LIST * list);

LNDE todo_mknode(char * item);
LNDE todo_first (TO_LIST * list);
LNDE todo_exisit(TO_LIST * list, char * item);
void todo_remove(TO_LIST * list, LNDE node);
int  todo_insert(TO_LIST * list, char * item);
int  todo_freedata(int id, LNDE node, void * parm);

#endif
