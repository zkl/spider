#ifndef TO_DO_LIST
#define TO_DO_LIST

#include "modules/list.h"


/* 1.未下载图片文件列表 */
/* 2.已下载图片文件列表 */
/* 3.未下载URL文件列表  */
/* 4.已下载URL文件列表  */

typedef struct  
{
	linked_list_t * urls;
}todo_list_t;

void todo_create(todo_list_t ** list);
void todo_free(todo_list_t * list);

/* 插入数据 */
int  todo_insert(todo_list_t * list, const char * item);

/* 删除第一条数据 */
int  todo_remove(todo_list_t * list);

/* 保存到文件 */
void todo_save(todo_list_t * list, const char * file);
void todo_load(todo_list_t * list, const char * file);

/* 获取第一条数据 */
const char * todo_one(todo_list_t * list);

#endif
