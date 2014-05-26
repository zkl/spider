/*******************************************************************************
**                                                  
** 版    本:  v 1.1.0                                                  
** 功    能:  管理网络连接，主要用于网络客户端
** 创    期:  Wed. 26 Feb 2014   修改于 Thur 3 Mar 2014 
** 版    权:  
** 作    者:  张昆伦 
** 备    注:          
** 更    新:  3 Mar 2014 - 增加事件处理机制       
             19 Mar 2014 - 增加注释和说明
*******************************************************************************/

/*
void           network_create (network_t ** network); 
void           network_delete (network_t * network);
net_socket_t * network_connect(network_t * network, const char * host, int port);

处理网络消息，主要是负责接收和发送网络消息，并且回调网络事件
int network_procmsg(network_t * network); 

配置网络模块
void network_config (network_t * network, network_config_t cmd, void * parm);
参数说明：
network:
	网络模块指针
cmd
	配置命令字，有以下几种可选值
	NET_SET_RECV_EVENT 
		设置网络接收时的回调函数，parm指向 void function(network_t * network, 
		net_socket_t * netsocket) 类型函数指针。
	NET_SET_SEND_EVENT
		设置网络发送时的回调函数，parm指向 void function(network_t * network, 
		net_socket_t * netsocket) 类型函数指针。
parm 
	根据命令字指向相应的参数


获取网络消息队列的大小
int net_socket_size (net_socket_t * socket); 

获取网络消息队列数据的首地址，该地址可以访问不超过数据队列大小的数据
char * net_socket_data (net_socket_t * socket);

删除网络消息队列队首数据，但是网络消息队列的首地址不变
char * net_socket_pop  (net_socket_t * socket, int bytes);

将数据写入到网络消息队列中并行发送数据，如果buf是空指针（buf == NULL）则只将网络
消息队列中的数据发送出去
int    net_socket_write(net_socket_t * socket, const char * buf, int size);

获取当前网络状态，0 网络被关闭 -1 网络被断开 1 网络正常
int    net_socket_statu(net_socket_t * socket);

关闭网络连接并且释放网络模块占用的资源
void   net_socket_close(net_socket_t * socket);

网络模块提供一个可以绑定用户数据的接口net_socket_set_user_data，用户可以使用该接口快速定位到相
应的数据，方便对于网络事件的处理
void   net_socket_set_user_data(net_socket_t * socket, void * d);
void * net_socket_get_user_data(net_socket_t * socket);

*/

#ifndef NETWORK_H
#define NETWORK_H

#include <WINDOWS.H>
#include "dns.h"
#include "queue.h"
#include "modules/list.h"

struct _network_;
struct _net_socket_;

typedef void (*network_event_t)(struct _network_ * network, 
	struct _net_socket_ * netsocket);

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
	int valid;
	network_t * network;
	queue_t * rdque;
	queue_t * wtque;
	linked_list_node_t * linked_node;
	void * user_data;
	network_event_t arrived_callback;
	network_event_t writeed_callback;
	network_event_t error_callback;
}net_socket_t;

enum net_socket_config_t
{
	NET_SET_RECV_EVENT,
	NET_SET_SEND_EVENT,
	NET_SET_ERROR_EVENT,
};

network_t *    network_create (); 
void           network_free   (network_t * network);
net_socket_t * network_connect(network_t * network, const char * host,
					int port);
int            network_procmsg(network_t * network);

int    net_socket_size (net_socket_t * socket);
char * net_socket_data (net_socket_t * socket);
char * net_socket_pop  (net_socket_t * socket, int bytes);
int    net_socket_write(net_socket_t * socket, const char * buf, int size);
int    net_socket_statu(net_socket_t * socket);
void   net_socket_close(net_socket_t * socket);
void   net_socket_config(net_socket_t * network, net_socket_config_t cmd, 
			void * parm);

void   net_socket_set_user_data(net_socket_t * socket, void * d);
void * net_socket_get_user_data(net_socket_t * socket);

#endif
