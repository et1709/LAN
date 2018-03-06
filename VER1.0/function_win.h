#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include<stdio.h>
#include"connect_server.h"
#include"protocol.h"
#include"show_menu.h"
#include<stdbool.h>


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



#endif


