/*******************************************************************************
**                                                  
** 版    本:  v 1.3.0                                                  
** 功    能:  定义链表的数据结构
** 创    期:  Sat. 3 Mar 2012  修改于 Wed. 26 Feb 2014 
** 版    权:  
** 作    者:  张昆伦 
** 备    注:                                              
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

/* 创建链表 */
linked_list_t * list_new  (void);

/*******************************************************************************
**                      Wed. 26 Feb 2014  更新                                */

/* 创建列表 */
void   linked_list_create(linked_list_t ** list);
void   linked_list_delete(linked_list_t * list);
int    linked_list_size(linked_list_t * list);

/* 获取数据 */
void * linked_list_data(linked_list_node_t * node);

/* 第一个位置 */
linked_list_node_t * linked_list_first(linked_list_t * list);

/* 最后一个位置 */
linked_list_node_t * linked_list_last (linked_list_t * list);

/* 下一个一个位置 */
linked_list_node_t * linked_list_next (linked_list_node_t * node);

/* 上一个一个位置 */
linked_list_node_t * linked_list_previous(linked_list_node_t * node);

/* 删除节点 */
linked_list_node_t * linked_list_remove(linked_list_t * list, 
	linked_list_node_t * node);

/* 插入节点 */
linked_list_node_t * linked_list_insert(linked_list_t * list, linked_list_node_t * node, 
	void * data);

/******************************************************************************/



linked_list_t * list_new  (void);
/* 在末端添加节点 */
void list_append (LIST * list, DATA data);

/* 查找节点第一次出现的位置 */
LNDE list_firstd (LIST * list, DATA data);
int  list_firstl2(LIST * list, DATA data);
int  list_position(LIST * list, LNDE node);

/* 移除指定位置的节点 */
void list_remove (LIST * list, int position);
LNDE list_removel2(LIST * list, LNDE node);

/* 获取指定位置的数据 */
DATA list_dataat (LIST * list, int position);
LNDE list_nodeat (LIST * list, int position);

/* 插入节点 */
void list_insert(LIST * list, LNDE prnode, DATA data);
void list_insertl2(LIST * list, DATA data, int position);
void list_exchange(LIST * list, LNDE enode, LNDE node);

/* 销毁链表 */
void list_delete (LIST * list, DATA_CBK data_free, void * parm);

/* 遍历列表 */
LNDE list_prview (LIST * list, DATA_CBK data_view, void * parm);
void list_delnde (LNDE node, DATA_CBK data_free, void * parm);

#endif