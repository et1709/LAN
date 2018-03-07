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
#include <stdlib.h>

#include "uinsock.h"
#include "protocol.h"
#include "database.h"

/************宏定义************/
#define SOCKET_PORT 8888 //端口号
#define MAXBACKLOG  50   //最大连接数量
#define SIZE_SHMADD 2048 //共享内存的大小

/************结构体声明************/
struct info
{
	int tcp_connfd;                 //连接描述符
	struct sockaddr_in tcp_cltaddr; //客户端套接字地址
	pthread_t thread;               //线程号
	AGREEMENT dataPacket;           //数据包
};

struct TcpInit
{
	struct sockaddr_in tcp_cltaddr; //客户端套接字地址
	int tcp_socket;                 //TCP 套接字描述符
	int tcp_connfd;                 //TCP 连接描述符
	in_port_t port;                 //设定端口号
};

struct ClientInfo
{
	struct sockaddr_in tcp_cltaddr; //客户端套接字地址
	int id;
	char nickname[20];
};

/************函数声明************/
//**********UDP**********
int udp_server_init(int index);
//**********TCP**********
int tcp_server_init(struct TcpInit *tcp_init);
int tcp_server_close(struct TcpInit *tcp_init);
//**********线程**********
void *tcp_server_handle(void *arg);
void *receive_msg(void *arg);
void *send_msg(void *arg);
int get_first();
//**********Handle_request**********
struct sockaddr_in handle_request(AGREEMENT *packet, int index);
void login_req(AGREEMENT *packet, int index);
struct sockaddr_in singleChat_req(AGREEMENT *packet);

/************外部变量声明************/
extern struct info infos[MAXBACKLOG];
extern struct ClientInfo cliInfos[MAXBACKLOG]; //已登录的客户信息
extern pthread_mutex_t mutex;  //互斥锁;
extern int cliNum;             //客户端数


#endif