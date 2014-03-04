#include <malloc.h>
#include <string.h>
#include "list.h"

/************************************************************************/
/* ��  �ܣ��Ƚ�����DATA�ṹ���Ƿ����                                   */
/* ����ֵ����ȷ���0�����߷��ط�0                                       */
/* ��  �ڣ�2012��10��11��                                               */
/* ��  ����v1.0                                                         */
/* ��  ע��                                                             */
/************************************************************************/
int list_datacmp(DATA des, DATA src)
{
	return 1;
}

/************************************************************************/
/* ��  �ܣ������յ������                                               */
/* ����ֵ�����ؿյ�����Ľṹ��                                         */
/* ��  �ڣ�2012��03��14��                                               */
/* ��  ����v1.1                                                         */
/* ��  ע��                                                             */
/************************************************************************/
LIST * list_new(void)
{
	LIST * list = (LIST *)malloc(sizeof(LIST));
	memset(list, 0, sizeof(LIST));
	return list;
}

/************************************************************************/
/* ��  �ܣ����������ͷ����������еĽڵ㣬����ÿ��dataָ��ص�         */
/*         callback����                                                 */
/* ����ֵ��                                                             */
/* ��  �ڣ�2012��03��14��                                               */
/* ��  ����v1.1                                                         */
/* ��  ע��                                                             */
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
/* ��  �ܣ�������list������һ���ڵ㣬dataָ��ڵ�������                 */
/* ����ֵ��                                                             */
/* ��  �ڣ�2012��03��14��                                               */
/* ��  ����v1.0                                                         */
/* ��  ע��                                                             */
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
/* ��  �ܣ���list��ɾ��nodeָ��Ľڵ�                                   */
/* ����ֵ��                                                             */
/* ��  �ڣ�2012��03��14��                                               */
/* ��  ����v1.2                                                         */
/* ��  ע���ú����������ͷ�node��dataָ��ָ����ڴ棬����ǰ��Ҫ�ֶ��ͷ� */
/*         dataָ����ڴ�ռ�                                           */
/************************************************************************/
LNDE list_removel2(LIST * list, LNDE node)
{
	LIST_NODE * tnode = list->head;

	if(!list || !node || !tnode)
		return 0;

	/* ȥ��������֤�����ӷ���*/
	/* ��LIST�в���node�ڵ�����ҵ������ִ�У��Ҳ���ֱ�ӷ��� */
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
/* ��  �ܣ�����list�����е�i���ڵ��ָ��                                */
/* ����ֵ��������ڵ�i���ڵ㣬�򷵻���ָ�룬���򷵻�NULL                */
/* ��  �ڣ�2012��03��14��                                               */
/* ��  ����v1.1                                                         */
/* ��  ע���±��0��ʼ                                                  */
/************************************************************************/
LNDE list_nodeat (LIST * list, int position)
{
	LIST_NODE *node = 0;
	
	if(list->size <= 0 || position >= list->size || position < 0)
		return 0;

	if(position > list->size/2)  // ���iλ������ĺ�벿��
	{
		node = list->last;

		for(int j=list->size-1; j>position; j--)
		{
			if(node)
				node = node->prev;
		}
	}
	else                 // iλ�������ǰ�벿��
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
/* ��������ListNode *FirstNode(const List *list, void *data)            */
/* ��  �ܣ���list������ҵ�һ��������ָ�����data�Ľڵ�                 */
/* ����ֵ������ҵ������ҵ��Ľڵ��ָ�룬���򷵻�NULL                   */
/* ��  �ڣ�2012��03��14��                                               */
/* ��  ����v1.0                                                         */
/* ��  ע��                                                             */
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
/* ��  �ܣ�ö���б�                                                     */
/* ����ֵ�����ؽ�����ڵ��λ��                                         */
/* ��  �ڣ�2012��10��11��                                               */
/* ��  ����v1.2                                                         */
/* ��  ע��                                                             */
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
	/* ���Ӵ�����֤�����Ų���������ȷ�� */
	//if(!enode || !node)
	//	return ;

	DATA edata = enode->d;
	enode->d = node->d;
	node->d  = edata;
}



/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ��������
** ��  �Σ� 
** ����ֵ��                                   
** ��  ע�� ��Ҫ���� linked_list_delete ɾ������
*******************************************************************************/
void linked_list_create(linked_list_t ** list)
{
	*list = (linked_list_t *)malloc(sizeof(linked_list_t));
	memset(*list, 0, sizeof(linked_list_t));
}

/* ɾ���б� */
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

/* ��ȡ���� */
void * linked_list_data(linked_list_node_t * node)
{
	return node->d;
}

int linked_list_size(linked_list_t * list)
{
	return list->size;
}

/* ��һ��λ�� */
linked_list_node_t * linked_list_first(linked_list_t * list)
{
	return list->head;
}

/* ���һ��λ�� */
linked_list_node_t * linked_list_last (linked_list_t * list)
{
	return list->last;
}

/* ��һ��һ��λ�� */
linked_list_node_t * linked_list_next (linked_list_node_t * node)
{
	return node->next;
}

/* ��һ��һ��λ�� */
linked_list_node_t * linked_list_previous(linked_list_node_t * node)
{
	return node->prev;
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ɾ��һ���ڵ�
** ��  �Σ�
** ����ֵ�� ������һ���ڵ�                      
** ��  ע��   
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
** ��  ���� v 1.1     
** ��  �ܣ� ��������
** ��  �Σ�
** ����ֵ�� �����²���Ľڵ�                                
** ��  ע��   
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
		new_node->next = list->head;

		if(list->head)
			list->head->prev = new_node;

		list->head = new_node;
	}
	else
	{
		new_node->next = node->next;
		new_node->prev = node;

		node->next = new_node;

		if(new_node->next)
			new_node->next->prev = new_node;

	}

	list->size++;

	return new_node;
}