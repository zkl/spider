#include <malloc.h>
#include <string.h>
#include "list.h"

/************************************************************************/
/* 功  能：比较两个DATA结构体是否相等                                   */
/* 返回值：相等返回0，否者返回非0                                       */
/* 日  期：2012年10月11日                                               */
/* 版  本：v1.0                                                         */
/* 备  注：                                                             */
/************************************************************************/
int list_datacmp(DATA des, DATA src)
{
	return 1;
}

/************************************************************************/
/* 功  能：创建空的链表表                                               */
/* 返回值：返回空的链表的结构体                                         */
/* 日  期：2012年03月14日                                               */
/* 版  本：v1.1                                                         */
/* 备  注：                                                             */
/************************************************************************/
LIST * list_new(void)
{
	LIST * list = (LIST *)malloc(sizeof(LIST));
	memset(list, 0, sizeof(LIST));
	return list;
}

/************************************************************************/
/* 功  能：销毁链表，释放链表中所有的节点，并对每个data指针回调         */
/*         callback函数                                                 */
/* 返回值：                                                             */
/* 日  期：2012年03月14日                                               */
/* 版  本：v1.1                                                         */
/* 备  注：                                                             */
/************************************************************************/
void list_delete(LIST * list, DATA_CBK data_free, void * parm)
{
	LIST_NODE * node = list->head;
	int id = 0;
	while(list->head)
	{
		if(data_free)
			data_free(id, node, parm);
		
		node = node->next;
		free(list->head);
		list->head = node;
		id++;
	}
	free(list);
}

/************************************************************************/
/* 功  能：在链表list中新增一个节点，data指向节点数据项                 */
/* 返回值：                                                             */
/* 日  期：2012年03月14日                                               */
/* 版  本：v1.0                                                         */
/* 备  注：                                                             */
/************************************************************************/
void list_append(LIST * list, DATA data)
{
	LIST_NODE *node = (LIST_NODE *)malloc(sizeof(LIST_NODE));
	memset(node, 0, sizeof(LIST_NODE));
	node->d = data;
	list->size++;
	
	if(!list->head)
		list->head = node;

	node->prev = list->last;

	if(list->last)
		list->last->next = node;

	list->last = node;
}

void list_insertl2 (LIST * list, DATA data, int position)
{
	LNDE prnode = list_nodeat(list, position-1);
	if(position >= list->size)
		list_append(list, data);
	else
		list_insert(list, prnode, data);
}

void list_insert (LIST * list, LNDE prnode, DATA data)
{
	LNDE node = (LNDE)malloc(sizeof(LIST_NODE));
	node->d = data;

	if(prnode == 0)
	{
		node->next = list->head;

		if(list->head)
			list->head->prev = node;
		else
			list->last = node;

		node->prev = 0;
		list->head = node;
	}
	else
	{
		node->next = prnode->next;
		prnode->next = node;
		node->prev = prnode;

		if(node->next)
			node->next->prev = node;
		else
			list->last = node;
	}
	list->size++;
}

/************************************************************************/
/* 功  能：在list中删除node指向的节点                                   */
/* 返回值：                                                             */
/* 日  期：2012年03月14日                                               */
/* 版  本：v1.2                                                         */
/* 备  注：该函数并不会释放node和data指针指向的内存，调用前需要手动释放 */
/*         data指向的内存空间                                           */
/************************************************************************/
LNDE list_removel2(LIST * list, LNDE node)
{
	LIST_NODE * tnode = list->head;

	if(!list || !node || !tnode)
		return 0;

	/* 去掉错误验证，增加风险*/
	/* 在LIST中查找node节点如果找到则继续执行，找不到直接返回 */
	//for(; tnode->next; tnode = tnode->next)
	//{
	//	if(node == tnode)
	//		break;
	//}

	//if(tnode != node)
	//	return ;


	if(node == list->last)
		list->last = node->prev;

	if(node == list->head)
		list->head = node->next;

	if(node->prev)
		node->prev->next = node->next;
	
	if(node->next)
		node->next->prev = node->prev;

	list->size--;

	node->prev = 0;
	node->next = 0;

	return node;
}

void list_remove (LIST * list, int position)
{
	LNDE node = list_nodeat(list, position);
	list_removel2(list, node);
}

/************************************************************************/
/* 功  能：返回list链表中第i个节点的指针                                */
/* 返回值：如果存在第i个节点，则返回其指针，否则返回NULL                */
/* 日  期：2012年03月14日                                               */
/* 版  本：v1.1                                                         */
/* 备  注：下标从0开始                                                  */
/************************************************************************/
LNDE list_nodeat (LIST * list, int position)
{
	LIST_NODE *node = 0;
	
	if(list->size <= 0 || position >= list->size || position < 0)
		return 0;

	if(position > list->size/2)  // 如果i位于链表的后半部分
	{
		node = list->last;

		for(int j=list->size-1; j>position; j--)
		{
			if(node)
				node = node->prev;
		}
	}
	else                 // i位于链表的前半部分
	{
		node = list->head;
		for(int j=0; j<position; j++)
		{
			if(node)
				node = node->next;
		}
	}
	return node;
}


DATA list_dataat (LIST * list, int position)
{
	LNDE node = list_nodeat(list, position);
	return node->d;
}

/************************************************************************/
/* 函数名：ListNode *FirstNode(const List *list, void *data)            */
/* 功  能：在list链表查找第一个数据域指针等于data的节点                 */
/* 返回值：如果找到返回找到的节点的指针，否则返回NULL                   */
/* 日  期：2012年03月14日                                               */
/* 版  本：v1.0                                                         */
/* 备  注：                                                             */
/************************************************************************/
LNDE list_firstd (LIST * list, DATA data)
{
	LIST_NODE *node = list->head;
	
	if(list->size <= 0)
		return 0;

	for(int i=0; i<list->size; i++)
	{
		if(node->d == data)
			return node;
		node = node->next;
	}
	return 0;
}


int  list_firstl2(LIST * list, DATA data)
{
	LIST_NODE *node = list->head;

	if(list->size <= 0)
		return -1;

	for(int i=0; i<list->size; i++)
	{
		if(node->d == data)
			return i;
		node = node->next;
	}
	return -1;
}

int list_position(LIST * list, LNDE node)
{
	LIST_NODE *tnode = list->head;

	if(list->size <= 0)
		return -1;

	for(int i=0; i<list->size; i++)
	{
		if(tnode == node)
			return i;
		tnode = tnode->next;
	}
	return -1;
}

/************************************************************************/
/* 功  能：枚举列表                                                     */
/* 返回值：返回结束后节点的位置                                         */
/* 日  期：2012年10月11日                                               */
/* 版  本：v1.2                                                         */
/* 备  注：                                                             */
/************************************************************************/
LNDE list_prview(LIST * list, DATA_CBK data_view, void * parm)
{
	LIST_NODE * node = list->head;

	for(int id = 0; node; node = node->next, id++)
	{
		if(!data_view(id, node, parm))
			return node;
	}
	return 0;
}

void list_delnde (LNDE node, DATA_CBK data_free, void * parm)
{
	if(data_free)
		data_free(-1, node, parm);

	memset(node, 0, sizeof(LIST_NODE));
	free(node);
}

/* v1.1 */
void list_exchange(LIST * list, LNDE enode, LNDE node)
{
	/* 不加错误验证，相信参数都是正确的 */
	//if(!enode || !node)
	//	return ;

	DATA edata = enode->d;
	enode->d = node->d;
	node->d  = edata;
}



/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 创建链表
** 入  参： 
** 返回值：                                   
** 备  注： 需要调用 linked_list_delete 删除对象
*******************************************************************************/
void linked_list_create(linked_list_t ** list)
{
	*list = (linked_list_t *)malloc(sizeof(linked_list_t));
	memset(*list, 0, sizeof(linked_list_t));
}

/* 删除列表 */
void linked_list_delete(linked_list_t * list)
{
	linked_list_node_t * node = list->head;
	while(list->head)
	{
		node = node->next;
		free(list->head);
		list->head = node;
	}
	free(list);
}

/* 获取数据 */
void * linked_list_data(linked_list_node_t * node)
{
	return node->d;
}

int linked_list_size(linked_list_t * list)
{
	return list->size;
}

/* 第一个位置 */
linked_list_node_t * linked_list_first(linked_list_t * list)
{
	return list->head;
}

/* 最后一个位置 */
linked_list_node_t * linked_list_last (linked_list_t * list)
{
	return list->last;
}

/* 下一个一个位置 */
linked_list_node_t * linked_list_next (linked_list_node_t * node)
{
	return node? node->next: 0;
}

/* 上一个一个位置 */
linked_list_node_t * linked_list_previous(linked_list_node_t * node)
{
	return node? node->prev: 0;
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 删除一个节点
** 入  参：
** 返回值： 返回下一个节点                      
** 备  注：   
*******************************************************************************/
linked_list_node_t * linked_list_remove(linked_list_t * list, 
	linked_list_node_t * node)
{
	linked_list_node_t * next_node = node->next;

	if(node == list->last)
		list->last = node->prev;

	if(node == list->head)
		list->head = node->next;

	if(node->prev)
		node->prev->next = node->next;

	if(node->next)
		node->next->prev = node->prev;

	list->size--;

	free(node);

	return next_node;
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 插入数据
** 入  参：
** 返回值： 返回新插入的节点                                
** 备  注：   
*******************************************************************************/
linked_list_node_t * linked_list_insert(linked_list_t * list, 
	linked_list_node_t * node, void * data)
{
	linked_list_node_t * new_node = (linked_list_node_t *)malloc(
		sizeof(linked_list_node_t));

	new_node->d = data;
	new_node->prev = 0;
	new_node->next = 0;

	if(node == 0)
	{
		new_node->prev = 0;
		new_node->next = list->head;

		if(list->head)
		{	
			list->head->prev = new_node;
		}

		list->head = new_node;

		if(list->last == 0)
			list->last = new_node;
	}
	else
	{
		new_node->next = node->next;
		new_node->prev = node;

		node->next = new_node;

		if(new_node->next) // 存在下一个节点
			new_node->next->prev = new_node;
		else
			list->last = new_node;
	}

	list->size++;

	return new_node;
}
