#ifndef TO_DO_LIST
#define TO_DO_LIST

#include "modules/list.h"


/* 1.未下载图片文件列表 */
/* 2.已下载图片文件列表 */
/* 3.未下载URL文件列表  */
/* 4.已下载URL文件列表  */

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
