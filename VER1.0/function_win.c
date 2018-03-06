#include"function.h"

INFORMATION_QUEUE* pSend_queue;
INFORMATION_QUEUE* receive_queue;


//功能选择
void function(void)
{
	int rt;
	
	pSend_queue = init_queue();
	receive_queue = init_queue();
	
	//显示注册登陆菜单
	while (-1 == (rt = log_in()))
	{
		login_menu();
	}
	if(-1 == rt)
	{
		return -1;
	}
	//显示主菜单
	main_menu();
	choose_function();
}

//初始化队列
INFORMATION_QUEUE *init_queue(void)
{
	INFORMATION_QUEUE *pQueue = (INFORMATION_QUEUE *)malloc(sizeof(INFORMATION_QUEUE));
	if(NULL == pQueue)
	{
		perror("分配队列头内存失败!\n");
		return NULL;
	}
	pQueue->front = pQueue->rear = (INFORMATION_QUEUE *)malloc(sizeof(INFORMATION_QUEUE));
	if(NULL == pQueue->rear)
	{
		perror("分配队列头节点内存失败!\n");
		return NULL;
	}
	memset(&pQueue->front->data, 0, sizeof(AGREEMENT));
	pQueue->rear->pNext = NULL;
	
	return pQueue;
}
//入队
bool enqueue(INFORMATION_QUEUE *pQueue, AGREEMENT data)
{
	QUEUE_NODE *pNew = (QUEUE_NODE *)malloc(sizeof(QUEUE_NODE));
	if(NULL == pNew)
	{
		perror("分配节点内存失败!\n");
		return false;
	}
	pNew->data = data;
	pNew->pNext = NULL;
	pQueue->rear->pNext = pNew;
	pQueue->rear = pNew;

	return true;
}

//出队
bool dequeue(INFORMATION_QUEUE *pQueue, AGREEMENT *pData)
{
	if(pQueue->rear == pQueue->front)
	{
		return false;
	}
	QUEUE_NODE *pTmp = pQueue->front->pNext;
	*pNum = pTmp->data;

	pQueue->front->pNext = pTmp->pNext;
	if(NULL == pQueue->front->pNext)
	{
		pQueue->rear = pQueue->front;
	}
	free(pTmp);
	return true;
}



/*
* 1:单聊        2:群聊       3:查找好友
* 4:添加好友    5:查找群     6:创建群  
* 7:添加群      8:发送文件   0:退出
*/
int choose_function(void)
{
	int num;
	while(1)
	{		
		while(scanf(&num) <= 0)
		{
			printf("输入错误, 请重新输入!\n");
		}
		if(num > 9 || num < 0)
		{
			printf("抱歉,没有这个选项, 请重新输入!\n");
		}
		else {
			break;
		}
	}
	switch(num)
	{
		case 0:
			printf("感谢使用, 88~~");
			exit(0);
		case 1:
			single_chat();
			break;
		case 2:
			
			break;
		case 3:
			
			break;
		case 4:
			
			break;
		case 5:
			
			break;
		case 6:
			
			break;
		case 7:
			
			break;
		case 8:
			
			break;
		default:
			printf("抱歉,没有这个选项\n");
			break;
	}
}

int single_chat(void)
{
	
}


int log_in(void)
{
	int num;
	int rt;
	while(1)
	{		
		while(scanf(&num) <= 0)
		{
			printf("输入错误, 请重新输入!\n");
		}
		if(num > 3 || num < 0)
		{
			printf("抱歉,没有这个选项, 请重新输入!\n");
		}
		else {
			break;
		}
	}
	switch(num)
	{
		case 0: 
			printf("感谢使用, 88~~\n");
			return -1;
		case 1:
			//注册账号

			if(-1 == rt)
			{
				return -1;
			}
			break;
		case 2:
			//登陆
			if(-1 == rt)
			{
				return -1;
			}
			break;
	}
}

