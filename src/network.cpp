#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "network.h"

static void net_socket_free(net_socket_t * socket);
static int  net_socket_check_valid(net_socket_t * socket);


/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� �ж�����ģ���Ƿ���Ч���������Ī��Ч��Ӧ�ñ�ɾ���������Ҳ���������
				��ģ����û�����
** ��  �Σ� 
** ����ֵ�� 1 ��Ч,  0 ��Ч  
** ��  ע�� ��Ҫ����net_socket_free�����ͷ�
*******************************************************************************/
int net_socket_check_valid(net_socket_t * socket)
{
	return socket->valid;
}
/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ��������ģ��
** ��  �Σ� 
** ����ֵ��       
** ��  ע�� ����������ģ��ʹ�������Ҫ����network_free�����ͷ�
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
** ��  ���� v 1.1     
** ��  �ܣ� �ͷ�����ģ�����ݺ���Դ
** ��  �Σ� network - ����ģ��ָ��
** ����ֵ��                             
** ��  ע�� 
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
** ��  ���� v 1.1     
** ��  �ܣ� ���ӵ�����
** ��  �Σ� network - ����ģ��ָ��
            host    - �����ַIP������������
			port    - �����˿ں�

** ����ֵ�� socketģ��ָ��                                  
** ��  ע�� ����Ҫʹ��deleteɾ������   
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
** ��  ���� v 1.1     
** ��  �ܣ� ��ȡ��ǰ�������ݶ��������ݵĴ�С
** ��  �Σ� 
** ����ֵ��                             
** ��  ע�� 
*******************************************************************************/
int net_socket_size(net_socket_t * socket)
{
	return queue_size(socket->rdque);
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ��ȡ���ݶ����׵�ַ
** ��  �Σ� 
** ����ֵ��                             
** ��  ע�� 
*******************************************************************************/
char * net_socket_data(net_socket_t * socket)
{
	return queue_data(socket->rdque);
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ɾ�����ݶ�������
** ��  �Σ� network - ����ģ��ָ��
            num     - �ֽ�
** ����ֵ�� ����ָ��                                  
** ��  ע�� ����Ҫʹ��deleteɾ������   
*******************************************************************************/
char * net_socket_pop (net_socket_t * socket, int bytes)
{
	bytes = bytes<=queue_size(socket->rdque)?bytes:queue_size(socket->rdque);
	queue_dequeue(socket->rdque, bytes);

	return queue_data(socket->rdque);
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ������ģ��д������
** ��  �Σ� socket - ����ģ��ָ��
			buf    - �����׵�ַ
			size   - ���ݴ�С
** ����ֵ�� ʧ�� ����-1�� �ɹ� ����д������ݴ�С
** ��  ע�� 
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
** ��  ���� v 1.1     
** ��  �ܣ� ��ȡ����ģ�������״̬
** ��  �Σ� socket - ����ģ��ָ��

** ����ֵ�� -1  - ���类�Ͽ�
             0  - �������ӱ��ر�
			 1  - ������������
** ��  ע�� 
*******************************************************************************/
int net_socket_statu(net_socket_t * socket)
{
	return socket->statu;
}


/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� �ر��������ӣ������ͷ�������Դ
** ��  �Σ� socket - ����ģ��ָ��
** ����ֵ�� void
** ��  ע�� ����ģ�鱻�رպ�ָ������ģ���ָ�뽫ָ��δ֪����
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
** ��  ���� v 1.1     
** ��  �ܣ� ���û�����
** ��  �Σ� 
** ����ֵ��                             
** ��  ע�� 
*******************************************************************************/
void net_socket_set_user_data(net_socket_t * socket, void * d)
{
	socket->user_data = d;
}


/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ��ȡ�û�����
** ��  �Σ� 
** ����ֵ��                             
** ��  ע�� 
*******************************************************************************/
void * net_socket_get_user_data(net_socket_t * socket)
{
	return socket->user_data;
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ����������Ϣ�����գ����͵ȣ�
** ��  �Σ� network - ����ģ��ָ��
           
** ����ֵ�� �ɹ�����0�� ʧ�ܷ��� SOCKET_ERROR                             
** ��  ע�� ��Ҫ����
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

		/* �ж�����socket�Ƿ���Ч����Ҫ��ÿһ���û�����������ж�
		   �����Ч����Ҫ��ʱ�����ڴ��ͷź����� */
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
