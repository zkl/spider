#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "todolist.h"

void todo_create(todo_list_t ** list)
{
	*list = (todo_list_t *)malloc(sizeof(todo_list_t));
	memset(*list, 0, sizeof(todo_list_t));
	linked_list_create(&(*list)->urls);
}

void todo_free(todo_list_t * list)
{
	linked_list_node_t * node;
	for(node = linked_list_first(list->urls); node; node = linked_list_next(node))
	{
		char * url = (char *)linked_list_data(node);
		free(url);
	}
	linked_list_delete(list->urls);
	free(list);
}

/* 插入数据 */
int  todo_insert(todo_list_t * list, const char * item)
{
	linked_list_node_t * node = linked_list_first(list->urls);
	
	for(node; node; node = linked_list_next(node))
	{
		char * val = (char *)linked_list_data(node);
		int diff = stricmp(val, item);

		if(diff == 0)
			return -1;

		if(diff > 0)
		{
			node = linked_list_previous(node);
			linked_list_insert(list->urls, node, strdup(item));
			return 0;
		}
	}
	
	node = linked_list_last(list->urls);
	linked_list_insert(list->urls, node, strdup(item));
	return 0;
}

/* 删除第一条数据 */
int  todo_remove(todo_list_t * list)
{
	if(linked_list_size(list->urls) == 0)
		return -1;
	
	linked_list_node_t * node;
	node = linked_list_first(list->urls);
	if(node == 0)
		return -1;

	void * val = linked_list_data(node);
	free(val);
	
	linked_list_remove(list->urls, node);
	return 0;
}

/* 保存到文件 */
void todo_save(todo_list_t * list, const char * file);
void todo_load(todo_list_t * list, const char * file);

/* 获取第一条数据 */
const char * todo_one(todo_list_t * list)
{
	linked_list_node_t * node;
	if(linked_list_size(list->urls) <= 0)
		return 0;

	node = linked_list_first(list->urls);
	if(node == 0)
		return 0;

	return (char *)linked_list_data(node);
}