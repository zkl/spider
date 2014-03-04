/*******************************************************************************
**                                                  
** 版    本:  v 1.0.0                                                  
** 功    能:  管理网络连接，主要用于网络客户端
** 创    期:  Wed. 26 Feb 2014   修改于 Wed. 26 Feb 2014 
** 版    权:  
** 作    者:  张昆伦 
** 备    注:                                              
*******************************************************************************/
#ifndef NETWORK_H
#define NETWORK_H

#include <windows.h>
#include "dns.h"
#include "queue.h"
#include "modules/list.h"

typedef struct _network_
{
	int dsp;
	dns_t * dns;
	linked_list_t * net_sockets;
}network_t;

typedef struct _net_socket_
{
	int dsp;
	int statu;
	network_t * network;
	queue_t rdque;
	queue_t wtque;
	linked_list_node_t * linked_node;
}net_socket_t;

void           network_create (network_t ** network); /* 初始化 网络模块 */
void           network_delete (network_t * network);
net_socket_t * network_connect(network_t * network, const char * host, int port);
int            network_procmsg(network_t * network); /* 处理网络消息 */


int    net_socket_size (net_socket_t * socket);    // 可读数据大小
char * net_socket_data (net_socket_t * socket);    // 数据地址指针
char * net_socket_pop  (net_socket_t * socket, int bytes); // 数据出队
int    net_socket_write(net_socket_t * socket, const char * buf, int size);
int    net_socket_statu(net_socket_t * socket);
void   net_socket_close(net_socket_t * socket);


#endif