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
#include "sqlite3.h"

/**************************** 宏定义 ********************************/
#define SOCKET_PORT		8888				//端口号
#define MAXBACKLOG		50					//最大连接数量
#define SECRET_KEY		"Hello client"		//TCP 连接秘钥

#define ID_LEN			6					 //账号长度
#define NICKNAME_LEN	11					//昵称长度
#define PASSWORD_LEN	6					//密码长度
#define AGE_LEN			4					//年龄长度
#define SEX_LEN			2					//性别长度

/************************** 结构体声明 ******************************/
//用于接收线程
typedef struct info
{
	int tcp_connfd;							//连接描述符
	struct sockaddr_in tcp_cltaddr;			//客户端套接字地址
	pthread_t thread;						//线程号
	AGREEMENT dataPacket;					//数据包
} R_THREAD;

//用于TCP连接
typedef struct TcpInit
{
	struct sockaddr_in tcp_cltaddr;			//客户端套接字地址
	int tcp_socket;							//TCP 套接字描述符
	int tcp_connfd;							//TCP 连接描述符
	in_port_t port;							//设定端口号
} TCP_INIT;

//记录完成登录的客户信息
struct ClientInfo
{
	int  client_connfd;						//客户端连接描述符
	char id[ID_LEN];						//账号
	char nickname[NICKNAME_LEN];			//昵称
};

/*************************** 函数声明 *******************************/
//------------UDP---------------
int udp_server_init(int index);
//------------TCP---------------
int tcp_server_init(TCP_INIT *tcp_init);
int tcp_server_close(TCP_INIT *tcp_init);
//------------线程---------------
void *tcp_server_handle(void *arg);
int get_first();
//------------请求处理函数---------------
void handle_request(AGREEMENT *packet, int index);
void login_req(AGREEMENT *packet, int index);
void register_req(AGREEMENT *packet, int index);
void singleChat_req(AGREEMENT *packet, int index);
void addFriend_req(AGREEMENT *packet, int index);

/************************* 外部变量声明 *****************************/
extern R_THREAD infos[MAXBACKLOG];				//接收线程
extern TCP_INIT tcp_init;						//TCP连接
extern struct ClientInfo cliInfos[MAXBACKLOG];	//已登录的客户信息
extern pthread_mutex_t mutex;					//互斥锁;
extern int cliNum;								//客户端数


#endif
