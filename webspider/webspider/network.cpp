#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "network.h"


/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 创建网络模块
** 入  参： 
** 返回值：       
** 备  注： 
*******************************************************************************/
void network_create(network_t ** network)
{
	*network = (network_t *)malloc(sizeof(network_t));
	memset(*network, 0, sizeof(network_t));

	dns_create(&(*network)->dns, 20);
	linked_list_create(&(*network)->net_sockets);

	(*network)->dsp = 0;
}

void network_delete(network_t * network)
{
	linked_list_node_t * list_node;
	net_socket_t * socket;
	for(list_node = linked_list_first(network->net_sockets); list_node; 
		list_node = linked_list_next(list_node))
	{
		socket = (net_socket_t *)linked_list_data(list_node);

		queue_destroy(&socket->rdque);
		queue_destroy(&socket->wtque);

		free(socket);
	}

	dns_delete(network->dns);
	linked_list_delete(network->net_sockets);

	free(network);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 连接到主机
** 入  参： network - 网络模块指针
            host    - 网络地址IP或者是主机名
			port    - 主机端口号

** 返回值： socket模块指针                                  
** 备  注： 不需要使用delete删除对象   
*******************************************************************************/
net_socket_t * network_connect(network_t * network, const char * host, int port)
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = dns_host_ip(network->dns, host);
	addr.sin_port = htons(port);

	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(fd == INVALID_SOCKET)
		return 0;

	if( connect(fd, (struct sockaddr *)&addr, sizeof(addr)) )
		return 0;

	unsigned long argp = 1;
	if(ioctlsocket(fd, FIONBIO, &argp))
		printf("ioctlsocket error\n");
	
	net_socket_t * net_socket = (net_socket_t *)malloc(sizeof(net_socket_t));

	net_socket->dsp = fd;
	net_socket->statu = 1;
	net_socket->network = network;
	net_socket->linked_node = linked_list_insert(network->net_sockets, 0, net_socket);

	queue_init(&net_socket->rdque, 1024, 1024);
	queue_init(&net_socket->wtque, 1024, 1024);

	return net_socket;
}

int net_socket_size(net_socket_t * socket)
{
	return queue_size(&socket->rdque);
}

char * net_socket_data(net_socket_t * socket)
{
	return queue_data(&socket->rdque);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 删除数据队首数据
** 入  参： network - 网络模块指针
            num     - 字节
** 返回值： 队首指针                                  
** 备  注： 不需要使用delete删除对象   
*******************************************************************************/
char * net_socket_pop (net_socket_t * socket, int bytes)
{
	bytes = bytes<=queue_size(&socket->rdque)?bytes:queue_size(&socket->rdque);
	queue_dequeue(&socket->rdque, bytes);
	return queue_data(&socket->rdque);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 向网络模块写入数据
** 入  参： socket - 网络模块指针
			buf    - 数据首地址
			size   - 数据大小
** 返回值： 失败 返回-1， 成功 返回写入的数据大小
** 备  注： 
*******************************************************************************/
int net_socket_write(net_socket_t * socket, const char * buf, int size)
{
	queue_t * write_queue = &socket->wtque;

	queue_enqueue(write_queue, buf, size);

	if(queue_size(write_queue) > 0)
	{
		int ret = send(socket->dsp, queue_data(write_queue), 
			queue_size(write_queue), 0);

		if(ret > 0)
		{	
			queue_dequeue(write_queue, ret);
		}
		else
		{
			socket->statu = ret;
			return -1;
		}
	}

	return size;
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 获取网络模块的连接状态
** 入  参： socket - 网络模块指针

** 返回值： -1  - 网络被断开
             0  - 网络连接被关闭
			 1  - 网络连接正常
** 备  注： 
*******************************************************************************/
int net_socket_statu(net_socket_t * socket)
{
	return socket->statu;
}


/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 关闭网络连接，并且释放网络资源
** 入  参： socket - 网络模块指针

** 返回值： void
** 备  注： 网络模块被关闭后，指向网络模块的指针将指向未知数据
*******************************************************************************/
void net_socket_close(net_socket_t * socket)
{
	closesocket(socket->dsp);
	queue_destroy(&socket->rdque);
	queue_destroy(&socket->wtque);
	linked_list_remove(socket->network->net_sockets, socket->linked_node);
	free(socket);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 处理网络消息（接收，发送等）
** 入  参： network - 网络模块指针
           
** 返回值： 成功返回0， 失败返回 SOCKET_ERROR                             
** 备  注： 主要函数
*******************************************************************************/
int network_procmsg(network_t * network)
{
	struct timeval val;
	val.tv_sec = 1;
	val.tv_usec= 0;

	fd_set fd_read;
	FD_ZERO(&fd_read);

	net_socket_t * socket;
	linked_list_node_t * list_node;

	for(list_node = linked_list_first(network->net_sockets); list_node; 
		list_node = linked_list_next(list_node))
	
	{
		socket = (net_socket_t *)linked_list_data(list_node);
		FD_SET(socket->dsp, &fd_read);
	}

	int ret = select(0, &fd_read, NULL, NULL, &val);

	if(ret <= 0)
		return ret;

	for(list_node = linked_list_first(network->net_sockets); list_node; 
		list_node = linked_list_next(list_node))
	{
		socket = (net_socket_t *)linked_list_data(list_node);
	
		if(FD_ISSET(socket->dsp, &fd_read))
		{
			ret = recv(socket->dsp, queue_last(&socket->rdque), 
				queue_left(&socket->rdque), 0);

			printf("recv buf: %s\n", queue_last(&socket->rdque));

			if(ret > 0)
				queue_enqueue(&socket->rdque, 0, ret);

			if(ret <= 0)
				socket->statu = ret;
		}

		net_socket_write(socket, 0, 0);
	}

	return 0;
}