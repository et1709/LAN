#include"function.h"

INFORMATION_QUEUE* pSend_queue;
INFORMATION_QUEUE* receive_queue;

extern int udp_socket;


//����ѡ��
void function(int sockfd)
{
	int rt;
	pthread_t send_thread, receive_thread;
	pthread_t handler_receive_thread, handler_send_thread;
	
	pSend_queue = init_queue();
	receive_queue = init_queue();

	/*
	PARAMETER  parameter;
	parameter.callBack = handler_receive;
	parameter.sockfd = sockfd;
	*/
	
	pthread_create(&send_thread, NULL, _send, (void *)sockfd);
	pthread_create(&receive_thread, NULL, receive, (void *)sockfd);
	pthread_create(&send_thread, NULL, handler_receive, NULL);
	
	//��ʾע���½�˵�
	login_menu();
	while (-1 == (rt = log_in_menu(sockfd)));

	//��ʾ���˵�
	while(1)
	{
		main_menu();
		choose_function();
	}	
}

/*���������Ϣ����:
* 11:ע��ɹ�,  10:ע��ʧ��.   22:��½�ɹ�,  20:��½ʧ��
* 33:�յ�������Ϣ,  44:���ҵ�����  40:���Һ���ʧ��
*/
int handler_receive(void)
{
	AGREEMENT receive_data;

	while(1)
	{		
		if(receive_queue->front != receive_queue->rear)
		{						
			memset(&receive_data, 0, sizeof(AGREEMENT));
			if(false == dequeue(receive_queue, &receive_data))
			{
				printf("��Ϣ���Ӷ�ʧ��!");
				return -1;
			}
			switch(receive_data.order)
			{
				case 11:
					printf("   ע��ɹ�, �����˺���: %d\n", receive_data.friend_id);
					break;
				case 10:
					printf("   ע��ʧ�ܣ�������ע��!\n");
					break;
				case 22:
					printf("   ��½�ɹ�!\n");
					break;
				case 20:
					printf("   ��½ʧ��! �����µ�½!\n");
					break;
				case 33:
					printf("   �յ�%s(%d)��������Ϣ: %s\n",
						  	receive_data.friend_nickname, receive_data.friend_id,
						  	receive_data.information);
					break;
				case 44:
					//printf("   �����б�:\n");
					
					
					break;
				case 40:

					break;
			}
		}
	}
}


//�����߳�
void *_send(void * arg)
{
	int cnt;
	int sockfd = (int)arg;
	fd_set wset;
	struct timeval timevalue = {1, 0};
	AGREEMENT send_data;
	int send_data_len = sizeof(AGREEMENT);

	while(1)
	{
		if(pSend_queue->front != pSend_queue->rear)
		{			
			// �����������	
			FD_ZERO(&wset);		

			// ��������������������
			FD_SET(sockfd, &wset);
			
			//����Ƿ�����Ϣ�ɶ�
			while( (-1 == (cnt = select(sockfd+1, NULL, &wset,
					NULL, &timevalue))) && (EINTR == errno));
			if(-1 == cnt)
			{
				perror("����д��Ϣʧ��!");
				sleep(1);
				continue;
			}

			memset(&send_data, 0, send_data_len);

			//��Ϣ����
			if(false == dequeue(pSend_queue, &send_data))
			{
				printf("��Ϣ���Ӷ�ʧ��!");
				continue;
			}		
			cnt = recv(sockfd, &send_data, send_data_len, 0);
			if(-1 == cnt)
			{
				printf("������Ϣʧ��!\n");
			}
			else if(cnt > 0)
			{
				printf("��������(%d)�ɹ�!\n", send_data.order);
			}
			else{
				printf("������0����Ϣ!\n");
			}		
		}
	}	
}

//�����߳�
void *receive(void *arg)
{
	int tcp_cnt, udp_cnt, num = 3;
	int sockfd = (int)arg;
	fd_set tcp_rset, udp_rset;
	struct timeval timevalue = {1, 0};
	AGREEMENT recv_data, temp_buf;
	char udp_rdbuf[20];
	int recv_data_len = sizeof(AGREEMENT);
	
	while(1)
	{
		// �����������	
		FD_ZERO(&tcp_rset);
		FD_ZERO(&udp_rset);
		
		// ��������������������
		FD_SET(sockfd, &tcp_rset);
		FD_SET(udp_socket, &udp_rset);
		
		//����Ƿ�����Ϣ�ɶ�
		while( (-1 == (tcp_cnt = select(sockfd+1, &tcp_rset, NULL,
				 NULL, &timevalue))) && (EINTR == errno));
		
		while( (-1 == (udp_cnt = select(udp_socket+1, &udp_rset, NULL,
				 NULL, &timevalue))) && (EINTR == errno));
			
		if(-1 == tcp_cnt)
		{
			perror("���TCP�ɶ���Ϣʧ��!");
			sleep(1);
			continue;
		}
		if(-1 == udp_cnt)
		{
			perror("���UDP�ɶ���Ϣʧ��!");
			sleep(1);
			continue;
		}

		if (FD_ISSET(udp_socket, &udp_rset))
		{
			memset(udp_rdbuf, 0, sizeof(udp_rdbuf));
			udp_cnt = recv(sockfd, udp_rdbuf, sizeof(udp_rdbuf), 0);
			if(udp_cnt > 0)
			{
				printf("���յ�UDP��������Ϣ: %s\n", udp_rdbuf);
			}			
		}

		if (FD_ISSET(sockfd, &tcp_rset))
		{		
			memset(&recv_data, 0, recv_data_len);		
			tcp_cnt = recv(sockfd, &recv_data, recv_data_len, 0);
	
			if(-1 == tcp_cnt)
			{
				printf("����Ϣʧ��!\n");
				continue;
			}
			else if(tcp_cnt > 0)
			{
				/*
				//������
				{
					printf("���յ���������Ϣ(��Ӧ����%d): %s\n", recv_data.order,
							recv_data.information);
					
					temp_buf.information = CLT_TOKEN;
					tcp_cnt = send(sockfd, &temp_buf, sizeof(temp_buf), 0);
					if(-1 == tcp_cnt)
					{
						printf("ת����������Ϣʧ��!\n");
					}
					else if(tcp_cnt > 0)
					{
						printf("ת����������Ϣ (%s) �ɹ�!\n", recv_data.information);
					}
				}
				*/
				//��Ϣ���
				if(false == enqueue(receive_queue, recv_data))
				{
					printf("��Ϣ���ʧ��!");
					continue;
				}			
			}
		}
	}
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
	pQueue->front = pQueue->rear = (QUEUE_NODE *)malloc(sizeof(QUEUE_NODE));
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
	*pData = pTmp->data;

	pQueue->front->pNext = pTmp->pNext;
	if(NULL == pQueue->front->pNext)
	{
		pQueue->rear = pQueue->front;
	}
	free(pTmp);
	return true;
}

/*����ѡ��
* 1:����        2:Ⱥ��       3:���Һ���
* 4:��Ӻ���    5:����Ⱥ     6:����Ⱥ  
* 7:���Ⱥ      8:�����ļ�   0:�˳�
*/
int choose_function(void)
{
	int num;
	char ch;
	while(1)
	{		
		while(scanf("%d", &num) <= 0)
		{
			printf("�������, ����������!\n");
			while((ch = getchar()) != '\n' && ch != EOF);
		}
		if(num > 8 || num < 0)
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
	AGREEMENT data;

	memset(&data, 0, sizeof(data));
	printf("������Ҫ����λ��������? ");
	scanf("%d", &data.mine_id);
	
}

int log_in_menu(int sockfd)
{
	int num;
	int rt;
	while(1)
	{		
		while(scanf("%d", &num) <= 0)
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
			_register(sockfd);
			if(-1 == rt)
			{
				return -1;
			}
			usleep(8000);
			break;
		case 2:
			//��½
			log_in(sockfd);
			if(-1 == rt)
			{
				return -1;
			}
			usleep(8000);
			break;
	}
}


//ע��
int _register(int sockfd)
{
	AGREEMENT data;
	int cnt;
	memset(&data, 0, sizeof(data));
	data.order = 1;

	strcpy(data.information, "����ע��");
	
	while((-1 == (cnt = send(sockfd, (void*)&data, sizeof(data), 0))) 
			   && (EINTR == errno));
	if(-1 == cnt)
	{
		perror("��������ע��ʧ��!");
		return -1;
	}
	else if(cnt > 0)
	{
		printf("�ɹ��������� %d ��Ϣ\n", data.order);
	}
	
	return 0;
}


//��½
int log_in(int sockfd)
{
	AGREEMENT data;
	int cnt;
	memset(&data, 0, sizeof(data));
	data.order = 2;
	strcpy(data.information, "�����½");

	while((-1 == (cnt = send(sockfd, (void*)&data, sizeof(data), 0))) 
			   && (EINTR == errno));
	if(-1 == cnt)
	{
		perror("���������½ʧ��!");
		return -1;
	}
	else if(cnt > 0)
	{
		printf("�ɹ��������� %d ��Ϣ\n", data.order);
	}

	
	return 0;
}


