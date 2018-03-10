#include"function.h"

INFORMATION_QUEUE* pSend_queue;
INFORMATION_QUEUE* receive_queue;

extern int udp_socket;
int LoginFlag = 0;
int RegisterFlag = 0;
char my_account[6];


pthread_mutex_t LoginMutex; //= PTHREAD_MUTEX_INITIALIZER;    //互斥锁
pthread_cond_t LoginCond;      //条件变量


//功能选择
void function(int sockfd)
{
	int rt;
	pthread_t send_thread, receive_thread;
	pthread_t handler_receive_thread, handler_send_thread;

	//初始化队列
	pSend_queue = init_queue();
	receive_queue = init_queue();

	// 初始化互斥锁和条件变量
	pthread_mutex_init(&LoginMutex, NULL);
	pthread_cond_init(&LoginCond, NULL);
	
	pthread_create(&send_thread, NULL, _send, (void *)sockfd);
	pthread_create(&receive_thread, NULL, receive, (void *)sockfd);
	pthread_create(&send_thread, NULL, handler_receive, (void *)sockfd);
	
	//显示注册登陆菜单	
	while (1)
	{
		login_menu();
		log_in_menu(sockfd);
		//usleep(300000);

		if(RegisterFlag)
		{
			RegisterFlag = 0;
			usleep(300000);
			continue;
		}

		// 加互斥锁	
		pthread_mutex_lock(&LoginMutex);
		while(0 == LoginFlag)
		{
			pthread_cond_wait(&LoginCond, &LoginMutex);  //等待条件成立
		}
		pthread_mutex_unlock(&LoginMutex);
		//printf("LoginFlag = %d\n", LoginFlag);
		break;
	}
	
	while(1)
	{		
		//进入主菜单
		main_menu();
	}	
}

//主菜单
void main_menu(void)
{
	show_main_menu();
	choose_function();
}

/*
*    处理接收消息函数:
* 11:注册成功,          10:注册失败.   
* 22:登陆成功,          20:登陆失败
* 33:收到单聊信息,      
* 44:查找到好友成功     40:查找好友失败
* 66:添加好友成功,      60:添加好友失败
* 77:查找好友列表成功   70:查找好友列表失败
*/
void * handler_receive(void *pSockfd)
{
	AGREEMENT receive_data;
	int sockfd = (int)pSockfd;
	while(1)
	{
		if(receive_queue->front != receive_queue->rear)
		{
			memset(&receive_data, 0, sizeof(AGREEMENT));
			if(false == dequeue(receive_queue, &receive_data))
			{
				printf("消息出队队失败!");
				return;
			}
			switch(receive_data.order)
			{
				case 11:
					printf("   注册成功, 您的账号是: %s\n", receive_data.mine_id);
					//receive_data.order = -1;
					break;
				case 10:
					printf("   注册失败，失败原因: %s\n", receive_data.information);
					printf("   请重新注册!\n");
					//receive_data.order = -1;
					break;
				case 22:
					printf("   登陆成功!\n");

					// 加互斥锁	
					pthread_mutex_lock(&LoginMutex);
					
					LoginFlag = 1;

					//发通知给等待条件成立的线程
					pthread_cond_signal(&LoginCond);
					// 解锁	
					pthread_mutex_unlock(&LoginMutex);
					//receive_data.order = -1;
					break;
				case 20:
					printf("   登陆失败! 失败原因: %s\n", receive_data.information);
					printf("   请重新登陆!\n");
					//log_in(sockfd);
					//receive_data.order = -1;
					break;
				case 33:
					printf("   收到(%s)(%s)发来的消息: %s\n",
						  	receive_data.friend_nickname, receive_data.friend_id,
						  	receive_data.information);
					//receive_data.order = -1;
					break;
				case 44:
					printf("   查找好友成功, 好友信息:\n");
					printf("   昵称:%s(账号:%s)", receive_data.friend_nickname,
							receive_data.friend_id);
					printf("   性别:%s, 年龄: %s", receive_data.sex,
							receive_data.age);
					break;
				case 40:
					printf("   查找好友失败! 原因: %s\n", receive_data.information);
					//receive_data.order = -1;
					break;
				case 66:
					printf("   添加好友成功!\n");
					//receive_data.order = -1;
					break;
				case 60:
					printf("   添加好友失败! 原因: %s\n", receive_data.information);
					//receive_data.order = -1;
					break;
				case 77:
					printf("   查找好友列表成功! 好友列表是:\n");
					int i;
					for(i = 0; i < receive_data.friend_num; i++)
					{
						printf("   %s(%s)\n", receive_data.friend_list.friend_nickname[i],
						   		receive_data.friend_list.friend_id[i]);
					}
					//receive_data.order = -1;
					break;
				case 70:
					printf("   查找好友列表失败! 原因: %s\n", receive_data.information);
					//receive_data.order = -1;
					break;				
			}
		}
	}
}


//发送线程
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
			// 清空描述符集
			FD_ZERO(&wset);

			// 设置描述符到描述符集
			FD_SET(sockfd, &wset);
			
			//检测是否有消息可写
			while( (-1 == (cnt = select(sockfd+1, NULL, &wset,
					NULL, &timevalue))) && (EINTR == errno));
			if(-1 == cnt)
			{
				perror("监测可写消息失败!");
				sleep(1);
				continue;
			}

			if (FD_ISSET(sockfd, &wset))
			{			
				memset(&send_data, 0, send_data_len);
				
				//消息出队
				if(false == dequeue(pSend_queue, &send_data))
				{
					printf("消息出队失败!");
					continue;
				}		
				cnt = send(sockfd, &send_data, send_data_len, 0);
				if(-1 == cnt)
				{
					printf("发送消息失败!\n");
				}
				else
				{
					printf("成功发送了%d个字节消息!\n", cnt);
					printf("   消息包内容是:\n");
					printf("   order:%d, information: %s\n",
						    send_data.order, send_data.information);
					
					printf("   nickname:%s, mine_id: %s\n",
						    send_data.nickname, send_data.mine_id);
					
					printf("   age:%s, sex: %s\n",
						    send_data.age, send_data.sex);
					
					printf("   friend_nickname:%s, friend_id: %s\n",
						    send_data.friend_nickname, send_data.friend_id);
					
					printf("   send_msg_time:%s\n", send_data.send_msg_time);			
				}
			}
		}
	}	
}

//接收线程
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
		// 清空描述符集	
		FD_ZERO(&tcp_rset);
		FD_ZERO(&udp_rset);
		
		// 设置描述符到描述符集
		FD_SET(sockfd, &tcp_rset);
		FD_SET(udp_socket, &udp_rset);
		
		//检测是否有消息可读
		while( (-1 == (tcp_cnt = select(sockfd+1, &tcp_rset, NULL,
				 NULL, &timevalue))) && (EINTR == errno));
		
		while( (-1 == (udp_cnt = select(udp_socket+1, &udp_rset, NULL,
				 NULL, &timevalue))) && (EINTR == errno));
			
		if(-1 == tcp_cnt)
		{
			perror("监测TCP可读消息失败!");
			sleep(1);
			continue;
		}
		if(-1 == udp_cnt)
		{
			perror("监测UDP可读消息失败!");
			sleep(1);
			continue;
		}
/*
		if (FD_ISSET(udp_socket, &udp_rset))
		{
			memset(udp_rdbuf, 0, sizeof(udp_rdbuf));
			udp_cnt = recv(sockfd, udp_rdbuf, sizeof(udp_rdbuf), 0);
			if(udp_cnt > 0)
			{
				printf("接收到UDP服务器消息: %s\n", udp_rdbuf);
			}			
		}
*/
		if (FD_ISSET(sockfd, &tcp_rset))
		{		
			memset(&recv_data, 0, recv_data_len);		
			tcp_cnt = recv(sockfd, &recv_data, recv_data_len, 0);
	
			if(-1 == tcp_cnt)
			{
				printf("读消息失败!\n");
				continue;
			}
			else if(tcp_cnt > 0)
			{
				//消息入队
				if(false == enqueue(receive_queue, recv_data))
				{
					printf("消息入队失败!");
					continue;
				}			
			}
		}
	}
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
	pQueue->front = pQueue->rear = (QUEUE_NODE *)malloc(sizeof(QUEUE_NODE));
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
	*pData = pTmp->data;

	pQueue->front->pNext = pTmp->pNext;
	if(NULL == pQueue->front->pNext)
	{
		pQueue->rear = pQueue->front;
	}
	free(pTmp);
	return true;
}

/*功能选择
* 1:单聊        2:群聊       3:查找好友
* 4:添加好友    5:查找群     6:创建群  
* 7:添加群      8:发送文件   9:查看好友列表   0:退出
*/
int choose_function(void)
{
	int num;

	while(1)
	{		
		num = get_integer();
		if(num > 10 || num < 0)
		{
			printf("Sorry, without this option, please retype!\n");
			continue;
		}
		break;
	}
	switch(num)
	{
		case 0:
			printf("感谢使用, 88~~\n");
			exit(0);
		case 1:
			single_chat();               // 单聊
			break;
		case 2:							 // 群聊
			
			break;
		case 3:
			find_friends();              // 查看好友
			break;
		case 4:
			add_friend();                // 添加好友
			break;
		case 5:                          // 查找群
			
			break;
		case 6:                          //创建群
			
			break;
		case 7:							 // 添加群
			
			break;
		case 8:							 //发送文件 
			
			break;
		case 9:							 //查看好友列表
			find_friend_list();
			break;
		
	}
}

//添加好友
void add_friend(void)
{
	AGREEMENT data;
	char ch;
	memset(&data, 0, sizeof(data));
	data.order = 6;
	
	printf("请输入您好友的账号:\n");
	get_size_string(data.friend_id, 5);
	
	strcpy(data.information, "添加好友");
	strcpy(data.mine_id, my_account);
	while(1)
	{
		//消息入队
		if(false == enqueue(pSend_queue, data))
		{
			printf("消息入队失败!");
			continue;
		}
		else {
			break;
		}
	}
}

//查找好友
int find_friends(void)
{
	AGREEMENT data;
	char ch;

	memset(&data, 0, sizeof(data));
	data.order = 4;
	
	printf("请输入您好友的账号(5位数):\n");
	get_size_string(data.friend_id, 5);
	
	strcpy(data.information, "查看好友信息");
	strcpy(data.mine_id, my_account);
	
	while(1)
	{
		//消息入队
		if(false == enqueue(pSend_queue, data))
		{
			printf("消息入队失败!");
			continue;
		}
		else {
			break;
		}
	}	
	return 0;
}

//查看好友列表
void find_friend_list(void)
{
	AGREEMENT data;
	memset(&data, 0, sizeof(data));
	data.order = 7;
	strcpy(data.information, "查看好友列表");
	while(1)
	{
		//消息入队
		if(false == enqueue(pSend_queue, data))
		{
			printf("消息入队失败!");
			continue;
		}
		else {
			break;
		}
	}
}

//单聊
int single_chat(void)
{
	AGREEMENT data;
	int cnt, flag;
	char ch;
	char temp_buf[6] = "";
	int buflen = 0;
	printf("进入单聊模式...\n");
	find_friend_list();
	usleep(300000);
	while(1)
	{		
		printf("(按 0 退出单聊模式, 按 1~9 任意数继续聊天模式)\n");
		cnt = get_integer();
		if(0 == cnt)
		{
			break;
		}
		flag = 0;
		memset(&data, 0, sizeof(data));
		data.order = 3;
		while(1)
		{			
			if(0 == buflen)
			{
				printf("   请问你要跟哪位好友(好友ID)聊天? (按0退出与当前好友的聊天)\n");
				get_string(temp_buf, 5);
				if('0' == temp_buf[0])
				{
					memset(temp_buf, 0, sizeof(temp_buf));
					buflen = 0;
					break;
				}
				buflen = sizeof(temp_buf);
				strcpy(data.friend_id, temp_buf);
				printf("friend_id = %s\n", data.friend_id);
			}
			
			printf("   请输入你要发送的消息内容(50字以内):\n");
			printf("   (按 0 退出与好友 %s 的聊天)\n", temp_buf);
			get_string(data.information, 100);
			if('0' == data.information[0])
			{
				memset(temp_buf, 0, sizeof(temp_buf));
				buflen = 0;
				break;
			}		
			while(1)
			{
				//消息入队
				if(false == enqueue(pSend_queue, data))
				{
					printf("消息入队失败!");
					continue;
				}
				else {
					memset(data.information, '\0', sizeof(data.information));
					break;
				}
			}		
		}		
	}
}

//注册登陆菜单
int log_in_menu(int sockfd)
{
	int num;
	int rt, ch;
	while(1)
	{		
		num = get_integer();
		if(num > 3 || num < 0)
		{
			printf("Sorry, without this option, please retype!\n");
			continue;
		}
		break;
	}
	switch(num)
	{
		case 0: 
			printf("感谢使用, 88~~\n");
			exit(0);
		case 1:			
			_register(sockfd);     //注册账号	
			usleep(8000);
			break;
		case 2:
			
			log_in(sockfd);        //登陆
			usleep(8000);
			break;
	}
}


//注册
int _register(int sockfd)
{
	AGREEMENT data;
	int cnt;
	struct information input_data;

	memset(&input_data, 0, sizeof(input_data));
	memset(&data, 0, sizeof(data));

	register_func(&input_data);
	
	data.order = 1;
	//strcpy(data.mine_id, input_data.id);
	strcpy(data.nickname, input_data.nickname);
	strcpy(data.password, input_data.password);
	strcpy(data.age, input_data.age);
	strcpy(data.sex, input_data.sex);
	strcpy(data.information, "请求注册");
	
	while(1)
	{
		//消息入队
		if(false == enqueue(pSend_queue, data))
		{
			printf("消息入队失败!");
			continue;
		}
		else {
			RegisterFlag = 1;
			break;
		}
	}

	return 0;
}

//登陆
int log_in(int sockfd)
{
	//printf("进入log_in函数\n");
	AGREEMENT data;
	int cnt;
	struct information input_data;
	memset(&input_data, 0, sizeof(input_data));
	memset(&data, 0, sizeof(data));
	
	login_func( &input_data);

	data.order = 2;
	strcpy(data.mine_id, input_data.id);
	strcpy(data.password, input_data.password);
	strcpy(data.information, "请求登陆");
	
	strcpy(my_account, data.mine_id);
	
	while(1)
	{
		//消息入队
		if(false == enqueue(pSend_queue, data))
		{
			printf("消息入队失败!");
			continue;
		}
		else {
			break;
		}
	}
	
	return 0;
}


