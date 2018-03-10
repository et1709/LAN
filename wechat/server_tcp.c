#include "server.h"


/*-------------------------------------------------------
|	函数名：tcp_server_init
|	功能  ：使用UICI接口进行TCP套接字创建
|	输入  ：struct TcpInit *tcp_init
|	输出  ：-1——失败； 0——成功
|	说明  ：
-------------------------------------------------------*/
int tcp_server_init(TCP_INIT *tcp_init)
{
	tcp_init->port = SOCKET_PORT;
	
	tcp_init->tcp_socket = uin_open_tcp(tcp_init->port); //建立一个socket通信
	if(tcp_init->tcp_socket == -1)
	{
		printf("创建套接字失败\n");
		return -1;
	}
	return 0;
}


/*-------------------------------------------------------
|	函数名：tcp_server_handle
|	功能  ：处理客户端的请求
|	输入  ：线程下标index
|	输出  ：
|	说明  ：处理请求的线程
-------------------------------------------------------*/
void *tcp_server_handle(void *arg)
{
	char *cltip = NULL;				//建立客户端端 IP
	in_port_t cltport;				//建立客户端端口号
	char buf_r[128] = "";
	char buf_w[128] = SECRET_KEY;
	int connect_status;
	int index = (int)arg;
	int connfd = infos[index].tcp_connfd;
	struct sockaddr_in target_cltaddr;
	AGREEMENT *packet = &(infos[index].dataPacket);
	int read_len = 0;
	
	//提取客户端IP和端口
	cltip = inet_ntoa(infos[index].tcp_cltaddr.sin_addr);
	cltport = ntohs(infos[index].tcp_cltaddr.sin_port);
	
	while(1)
	{
		memset(buf_r, 0 ,sizeof buf_r);		//清空缓冲区

		connect_status = r_recv(connfd, buf_r, sizeof buf_r);
		printf("\n接收到客户端TCP消息：%s\n", buf_r);
		if(connect_status == -1)
		{
			printf("读取消息失败\n");
			goto _out;
		}
		else if(connect_status == 0)
		{
			printf("客户端断开连接\n");
			goto _out;
		}
		else
		{
			printf("\n已连接客户端%s(%d)\n", cltip, cltport);
			if(strcmp(buf_r, "over") == 0)
			{
				printf("通信结束\n");
				break;
			}
		}
		
		//判断客户端发来的口令
		if(strcmp(buf_r, "Hello server") == 0)
		{
			connect_status = r_send(infos[index].tcp_connfd, buf_w, strlen(buf_w));
			if (connect_status == -1)
			{
				printf("发送失败\n");
				goto _out;
			}
			else
			{
				printf("发送成功：%s\n", buf_w);
			}
		}	

		while(1)
		{
			//接收客户端发送过来的数据包
			if((read_len = r_recv(connfd, (void *)packet, sizeof (AGREEMENT))) > 0)
			{
				printf("接收到（%s）的消息：%d %s %s\n", inet_ntoa(infos[index].tcp_cltaddr.sin_addr), 
					packet->order, packet->information, packet->mine_id);

				handle_request(packet, index);			//解析数据包，处理客户端不同的请求
			}
		}
	}

_out:
	// 释放线程信息空间
	memset(&infos[index], 0, sizeof (struct info));
	// 关闭连接描述符
	if (connfd != -1)
	{
		while ((-1 == close(connfd)) && (EINTR == errno));
	}
}


/*-------------------------------------------------------
|	函数名：get_first
|	功能  ：在 infos 数组中查找空位
|	输入  ：
|	输出  ：
|	说明  ：i——成功  -1——失败
-------------------------------------------------------*/
int get_first()
{
	int i;
	for(i = 0; i < MAXBACKLOG; i++)
	{
		if(infos[i].tcp_connfd == 0)
		{
			return i;
		}
	}
	return -1;
}
