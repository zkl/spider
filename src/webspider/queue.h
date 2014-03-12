#ifndef QUEUE_H
#define QUEUE_H

typedef struct _queue_
{
	int grow_step;
	int max_size;
	char * head;
	char * rear;
}queue_t;

int queue_init(queue_t * q, int maxsize, int grow_step);
int queue_destroy(queue_t * q);

int queue_enqueue(queue_t * q, const char * buf, int size);
int queue_dequeue(queue_t * q, int size);

int queue_size(queue_t * q);
int queue_left(queue_t * q);

char * queue_data(queue_t * q);
char * queue_last(queue_t * q);

#endif
