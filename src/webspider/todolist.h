#ifndef TO_DO_LIST
#define TO_DO_LIST

#include "modules/list.h"


/* 1.δ����ͼƬ�ļ��б� */
/* 2.������ͼƬ�ļ��б� */
/* 3.δ����URL�ļ��б�  */
/* 4.������URL�ļ��б�  */

typedef struct  
{
	linked_list_t * urls;
}todo_list_t;

void todo_create(todo_list_t ** list);
void todo_free(todo_list_t * list);

/* �������� */
int  todo_insert(todo_list_t * list, const char * item);

/* ɾ����һ������ */
int  todo_remove(todo_list_t * list);

/* ���浽�ļ� */
void todo_save(todo_list_t * list, const char * file);
void todo_load(todo_list_t * list, const char * file);

/* ��ȡ��һ������ */
const char * todo_one(todo_list_t * list);

#endif
