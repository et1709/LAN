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
//���
bool enqueue(INFORMATION_QUEUE *pQueue, AGREEMENT data);
//����
bool dequeue(INFORMATION_QUEUE *pQueue, AGREEMENT *pData);

/*����ѡ��
* 1:����        2:Ⱥ��       3:���Һ���
* 4:��Ӻ���    5:����Ⱥ     6:����Ⱥ  
* 7:���Ⱥ      8:�����ļ�   0:�˳�
*/
int choose_function(void);
int log_in_menu(int sockfd);
//ע��
int _register(int sockfd);
//��½
int log_in(int sockfd);
int single_chat(void);







#endif


