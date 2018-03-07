#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "registered.h"
#include "connect_server.h"
#include "protocol.h"
#include "show_menu.h"




/*
typedef struct  par
{
	int (*callBack)(void);
	int sockfd;
}PARAMETER;
*/

typedef struct node
{
	AGREEMENT data;
	struct node *pNext;
}QUEUE_NODE;

typedef struct head
{
	QUEUE_NODE* front;
	QUEUE_NODE* rear;
}INFORMATION_QUEUE;

void function(int sockfd);
int handler_receive(void);
void *_send(void *arg);
void *receive(void *arg);
INFORMATION_QUEUE *init_queue(void);
//入队
bool enqueue(INFORMATION_QUEUE *pQueue, AGREEMENT data);
//出队
bool dequeue(INFORMATION_QUEUE *pQueue, AGREEMENT *pData);

/*功能选择
* 1:单聊        2:群聊       3:查找好友
* 4:添加好友    5:查找群     6:创建群  
* 7:添加群      8:发送文件   0:退出
*/
int choose_function(void);
int log_in_menu(int sockfd);
//注册
int _register(int sockfd);
//登陆
int log_in(int sockfd);
int single_chat(void);







#endif


