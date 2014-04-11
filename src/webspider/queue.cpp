#include <string.h>
#include <malloc.h>
#include "queue.h"

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ��������
** ��  �Σ� maxsize		- ���е����ֵ
			grow_step	- �Զ������Ĳ���
** ����ֵ�� ���е�ָ��                            
** ��  ע�� 
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
** ��  ���� v 1.1     
** ��  �ܣ� �ͷŶ��� 
** ��  �Σ� 
** ����ֵ�� ���е�ָ��                            
** ��  ע�� 
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
** ��  ���� v 1.1     
** ��  �ܣ� ����
** ��  �Σ� 
** ����ֵ��                            
** ��  ע�� 
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
		
		q->rear = q->rear - q->head + st; //����βָ��
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
** ��  ���� v 1.1     
** ��  �ܣ� ����
** ��  �Σ� 
** ����ֵ��                            
** ��  ע�� 
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
** ��  ���� v 1.1     
** ��  �ܣ� ��ȡ���д�С
** ��  �Σ� 
** ����ֵ��                            
** ��  ע�� 
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
