/*******************************************************************************
**                                                  
** ��    ��:  v 1.3.0                                                  
** ��    ��:  ������������ݽṹ
** ��    ��:  Sat. 3 Mar 2012  �޸��� Wed. 26 Feb 2014 
** ��    Ȩ:  
** ��    ��:  ������ 
** ��    ע:                                              
*******************************************************************************/

#ifndef LIST_H
#define LIST_H

typedef void * DATA;

typedef struct _linked_list_node_t_
{
	DATA d;
	struct _linked_list_node_t_ * prev;
	struct _linked_list_node_t_ * next;
}LIST_NODE, * LNDE, linked_list_node_t;

typedef struct _linked_list_
{
	int size;
	struct _linked_list_node_t_ * head;
	struct _linked_list_node_t_ * last;
}LIST, linked_list_t;

typedef int (*DATA_CBK)(int id, LNDE node, void * parm);

/* �������� */
linked_list_t * list_new  (void);

/*******************************************************************************
**                      Wed. 26 Feb 2014  ����                                */

/* �����б� */
void   linked_list_create(linked_list_t ** list);
void   linked_list_delete(linked_list_t * list);
int    linked_list_size(linked_list_t * list);

/* ��ȡ���� */
void * linked_list_data(linked_list_node_t * node);

/* ��һ��λ�� */
linked_list_node_t * linked_list_first(linked_list_t * list);

/* ���һ��λ�� */
linked_list_node_t * linked_list_last (linked_list_t * list);

/* ��һ��һ��λ�� */
linked_list_node_t * linked_list_next (linked_list_node_t * node);

/* ��һ��һ��λ�� */
linked_list_node_t * linked_list_previous(linked_list_node_t * node);

/* ɾ���ڵ� */
linked_list_node_t * linked_list_remove(linked_list_t * list, 
	linked_list_node_t * node);

/* ����ڵ� */
linked_list_node_t * linked_list_insert(linked_list_t * list, linked_list_node_t * node, 
	void * data);

/******************************************************************************/



linked_list_t * list_new  (void);
/* ��ĩ����ӽڵ� */
void list_append (LIST * list, DATA data);

/* ���ҽڵ��һ�γ��ֵ�λ�� */
LNDE list_firstd (LIST * list, DATA data);
int  list_firstl2(LIST * list, DATA data);
int  list_position(LIST * list, LNDE node);

/* �Ƴ�ָ��λ�õĽڵ� */
void list_remove (LIST * list, int position);
LNDE list_removel2(LIST * list, LNDE node);

/* ��ȡָ��λ�õ����� */
DATA list_dataat (LIST * list, int position);
LNDE list_nodeat (LIST * list, int position);

/* ����ڵ� */
void list_insert(LIST * list, LNDE prnode, DATA data);
void list_insertl2(LIST * list, DATA data, int position);
void list_exchange(LIST * list, LNDE enode, LNDE node);

/* �������� */
void list_delete (LIST * list, DATA_CBK data_free, void * parm);

/* �����б� */
LNDE list_prview (LIST * list, DATA_CBK data_view, void * parm);
void list_delnde (LNDE node, DATA_CBK data_free, void * parm);

#endif