#include <string.h>
#include <malloc.h>
#include "queue.h"

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 创建队列
** 入  参： maxsize		- 队列的最大值
			grow_step	- 自动增长的步长
** 返回值： 队列的指针                            
** 备  注： 
*******************************************************************************/
queue_t * queue_create(int maxsize, int grow_step)
{
	queue_t * q = (queue_t *)malloc(sizeof(queue_t));
	q->grow_step= grow_step;
	q->max_size = maxsize;
	q->head = (char *)malloc(sizeof(char) * maxsize + sizeof(int));
	q->rear = q->head;

	return q;
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 释放队列 
** 入  参： 
** 返回值： 队列的指针                            
** 备  注： 
*******************************************************************************/
int queue_free(queue_t * q)
{
	free(q->head);
	q->max_size = 0;
	q->head = 0;
	q->rear = 0;

	return 0;
}


/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 进队
** 入  参： 
** 返回值：                            
** 备  注： 
*******************************************************************************/
int queue_enqueue(queue_t * q, const char * buf, int size)
{
	if(size <= 0)
		return 0;

	int left = queue_left(q);
	int grow = 0;

	while(left+grow <= size)
		grow += q->grow_step;

	if(grow > 0)
	{
		char * st = (char *)realloc(q->head, q->max_size + grow);
		
		q->rear = q->rear - q->head + st; //重置尾指针
		q->head = st;
		q->max_size += grow;
	}

	if(buf != 0)
		strncpy(q->rear, buf, size);

	q->rear += size;
	*q->rear = 0;

	return size;
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 出队
** 入  参： 
** 返回值：                            
** 备  注： 
*******************************************************************************/
int queue_dequeue(queue_t * q, int size)
{
	if(queue_size(q) < size || size <= 0)
		return -1;

	memcpy(q->head, q->head+size, (q->rear - q->head) - size);
	q->rear -= size;
	*(q->rear) = 0; 
	return size;
}


/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 获取队列大小
** 入  参： 
** 返回值：                            
** 备  注： 
*******************************************************************************/
int queue_size(queue_t * q)
{
	return q->rear - q->head;
}

int queue_left(queue_t * q)
{
	return q->max_size - (q->rear - q->head);
}

char * queue_data(queue_t * q)
{
	return q->head;
}

char * queue_last(queue_t * q)
{
	return q->rear;
}
