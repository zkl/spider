#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "network.h"

static void net_socket_free(net_socket_t * socket);
static int  net_socket_check_valid(net_socket_t * socket);


/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 判断网络模块是否有效，如果网络莫无效后应该被删除掉，而且不能引用网
				络模块的用户数据
** 入  参： 
** 返回值： 1 有效,  0 无效  
** 备  注： 需要调用net_socket_free进行释放
*******************************************************************************/
int net_socket_check_valid(net_socket_t * socket)
{
	return socket->valid;
}
/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 创建网络模块
** 入  参： 
** 返回值：       
** 备  注： 创建的网络模块使用完后需要调用network_free进行释放
*******************************************************************************/
network_t * network_create()
{
	network_t * network = (network_t *)malloc(sizeof(network_t));
	memset(network, 0, sizeof(network_t));

	dns_create(&(network)->dns, 20);
	linked_list_create(&(network)->net_sockets);

	network->dsp = 0;

	return network;
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 释放网络模块数据和资源
** 入  参： network - 网络模块指针
** 返回值：                             
** 备  注： 
*******************************************************************************/
void network_free(network_t * network)
{
	linked_list_node_t * list_node;
	net_socket_t * socket;
	for(list_node = linked_list_first(network->net_sockets); list_node; 
		list_node = linked_list_next(list_node))
	{
		socket = (net_socket_t *)linked_list_data(list_node);

		queue_free(socket->rdque);
		queue_free(socket->wtque);

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

	/* connect error */
	if(connect(fd, (struct sockaddr *)&addr, sizeof(addr)))
		return 0;

	unsigned long argp = 1;
	if(ioctlsocket(fd, FIONBIO, &argp))
		printf("ioctlsocket error\n");
	
	net_socket_t * net_socket = (net_socket_t *)malloc(sizeof(net_socket_t));

	net_socket->dsp = fd;
	net_socket->statu = 1;
	net_socket->valid = 1;
	net_socket->network = network;

	net_socket->writeed_callback = 0; 
	net_socket->arrived_callback = 0;
	net_socket->error_callback   = 0;

	net_socket->rdque = queue_create(8192, 1024);
	net_socket->wtque = queue_create(8192, 1024);

	return net_socket;
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 获取当前网络数据队列中数据的大小
** 入  参： 
** 返回值：                             
** 备  注： 
*******************************************************************************/
int net_socket_size(net_socket_t * socket)
{
	return queue_size(socket->rdque);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 获取数据队列首地址
** 入  参： 
** 返回值：                             
** 备  注： 
*******************************************************************************/
char * net_socket_data(net_socket_t * socket)
{
	return queue_data(socket->rdque);
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
	bytes = bytes<=queue_size(socket->rdque)?bytes:queue_size(socket->rdque);
	queue_dequeue(socket->rdque, bytes);

	return queue_data(socket->rdque);
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
	queue_t * write_queue = socket->wtque;

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
	socket->valid = 0;
	closesocket(socket->dsp);
}

void net_socket_free(net_socket_t * socket)
{
	queue_free(socket->rdque);
	queue_free(socket->wtque);
	free(socket);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 绑定用户数据
** 入  参： 
** 返回值：                             
** 备  注： 
*******************************************************************************/
void net_socket_set_user_data(net_socket_t * socket, void * d)
{
	socket->user_data = d;
}


/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 获取用户数据
** 入  参： 
** 返回值：                             
** 备  注： 
*******************************************************************************/
void * net_socket_get_user_data(net_socket_t * socket)
{
	return socket->user_data;
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
	val.tv_sec = 0;
	val.tv_usec= 100;

	fd_set fd_read;
	FD_ZERO(&fd_read);

	net_socket_t * socket;
	linked_list_node_t * list_node;

	for(list_node = linked_list_first(network->net_sockets); list_node; )
	{
		socket = (net_socket_t *)linked_list_data(list_node);
		if(net_socket_check_valid(socket) == 0)
		{
			net_socket_free(socket);
			list_node = linked_list_remove(network->net_sockets, list_node);
			continue;
		}

		FD_SET(socket->dsp, &fd_read);
		list_node = linked_list_next(list_node);
	}

	int ret = select(0, &fd_read, NULL, NULL, &val);

	if(ret <= 0)
		return ret;

	for(list_node = linked_list_first(network->net_sockets); list_node;
		list_node = linked_list_next(list_node))
	{
		socket = (net_socket_t *)linked_list_data(list_node);

		/* 判断网络socket是否有效，需要在每一个用户操作后进行判断
		   如果无效，需要及时进行内存释放和清理 */
		if(FD_ISSET(socket->dsp, &fd_read))
		{
			ret = recv(socket->dsp, queue_last(socket->rdque), 
				queue_left(socket->rdque), 0);

			if(ret > 0)
				queue_enqueue(socket->rdque, 0, ret);

			if(ret <= 0)
			{
				socket->statu = ret;
				if(socket->error_callback)
					((network_event_t)socket->error_callback)(network, socket);

				if(net_socket_check_valid(socket) == 0)
					continue;
			}

			if(socket->arrived_callback)
				((network_event_t)socket->arrived_callback)(network, socket);
		
			if(net_socket_check_valid(socket) == 0)
				continue;
		}

		net_socket_write(socket, 0, 0);
		if(socket->writeed_callback && net_socket_size(socket) == 0)
			((network_event_t)socket->writeed_callback)(network, socket);
	}
	return 0;
}

void   net_socket_config(net_socket_t * network, net_socket_config_t cmd, 
			void * parm)
{
	switch(cmd)
	{
	case NET_SET_RECV_EVENT:
		network->arrived_callback = (network_event_t)parm;
		break;
	case NET_SET_SEND_EVENT:
		network->writeed_callback = (network_event_t)parm;
		break;
	case NET_SET_ERROR_EVENT:
		network->error_callback = (network_event_t)parm;
		break;
	}
}
