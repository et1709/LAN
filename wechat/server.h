#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/shm.h>

#include "uinsock.h"

/************宏定义************/
#define SOCKET_PORT 8080 //端口号
#define MAXBACKLOG  50   //最大连接数量
#define SIZE_SHMADD 2048 //共享内存的大小

/************结构体声明************/
struct info
{
	int tcp_connfd;                 //连接描述符
	struct sockaddr_in tcp_cltaddr; //客户端套接字地址
	pthread_t thread;               //线程号
};

struct TcpInit
{
	struct sockaddr_in tcp_cltaddr; //客户端套接字地址
	int tcp_socket;                 //TCP 套接字描述符
	int tcp_connfd;                 //TCP 连接描述符
	in_port_t port;                 //设定端口号
};

/************函数声明************/
int udp_server_init();
int tcp_server_init(struct TcpInit *tcp_init);
void *tcp_server_handle(void *arg);
int tcp_server_close(struct TcpInit *tcp_init);
int get_first();
extern struct info infos[MAXBACKLOG];
int shmid_create();

#endif