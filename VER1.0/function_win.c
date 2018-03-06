#include"function.h"

INFORMATION_QUEUE* pSend_queue;
INFORMATION_QUEUE* receive_queue;


//����ѡ��
void function(void)
{
	int rt;
	
	pSend_queue = init_queue();
	receive_queue = init_queue();
	
	//��ʾע���½�˵�
	while (-1 == (rt = log_in()))
	{
		login_menu();
	}
	if(-1 == rt)
	{
		return -1;
	}
	//��ʾ���˵�
	main_menu();
	choose_function();
}

//��ʼ������
INFORMATION_QUEUE *init_queue(void)
{
	INFORMATION_QUEUE *pQueue = (INFORMATION_QUEUE *)malloc(sizeof(INFORMATION_QUEUE));
	if(NULL == pQueue)
	{
		perror("�������ͷ�ڴ�ʧ��!\n");
		return NULL;
	}
	pQueue->front = pQueue->rear = (INFORMATION_QUEUE *)malloc(sizeof(INFORMATION_QUEUE));
	if(NULL == pQueue->rear)
	{
		perror("�������ͷ�ڵ��ڴ�ʧ��!\n");
		return NULL;
	}
	memset(&pQueue->front->data, 0, sizeof(AGREEMENT));
	pQueue->rear->pNext = NULL;
	
	return pQueue;
}
//���
bool enqueue(INFORMATION_QUEUE *pQueue, AGREEMENT data)
{
	QUEUE_NODE *pNew = (QUEUE_NODE *)malloc(sizeof(QUEUE_NODE));
	if(NULL == pNew)
	{
		perror("����ڵ��ڴ�ʧ��!\n");
		return false;
	}
	pNew->data = data;
	pNew->pNext = NULL;
	pQueue->rear->pNext = pNew;
	pQueue->rear = pNew;

	return true;
}

//����
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
* 1:����        2:Ⱥ��       3:���Һ���
* 4:��Ӻ���    5:����Ⱥ     6:����Ⱥ  
* 7:���Ⱥ      8:�����ļ�   0:�˳�
*/
int choose_function(void)
{
	int num;
	while(1)
	{		
		while(scanf(&num) <= 0)
		{
			printf("�������, ����������!\n");
		}
		if(num > 9 || num < 0)
		{
			printf("��Ǹ,û�����ѡ��, ����������!\n");
		}
		else {
			break;
		}
	}
	switch(num)
	{
		case 0:
			printf("��лʹ��, 88~~");
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
			printf("��Ǹ,û�����ѡ��\n");
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
			printf("�������, ����������!\n");
		}
		if(num > 3 || num < 0)
		{
			printf("��Ǹ,û�����ѡ��, ����������!\n");
		}
		else {
			break;
		}
	}
	switch(num)
	{
		case 0: 
			printf("��лʹ��, 88~~\n");
			return -1;
		case 1:
			//ע���˺�

			if(-1 == rt)
			{
				return -1;
			}
			break;
		case 2:
			//��½
			if(-1 == rt)
			{
				return -1;
			}
			break;
	}
}

