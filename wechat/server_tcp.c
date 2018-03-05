#include "server.h"

extern struct info infos[MAXBACKLOG];

//UICI TCP 服务器初始化
int tcp_server_init(struct TcpInit *tcp_init)
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


//TCP 服务器处理客户端连接请求的线程
void *tcp_server_handle(void *arg)
{
	char *cltip = NULL;           //建立客户端端 IP
	in_port_t cltport;            //建立客户端端口号
	char buf_r[128] = "";
	char buf_w[128] = "长江长江我是黄河";
	int connect_status;
	int index = (int)arg;
	int connfd = infos[index].tcp_connfd;
	
	//提取客户端IP和端口
	cltip = inet_ntoa(infos[index].tcp_cltaddr.sin_addr);
	cltport = ntohs(infos[index].tcp_cltaddr.sin_port);
	
	while(1)
	{
		memset(buf_r, 0 ,sizeof buf_r);//清空缓冲区
		connect_status = r_recv(connfd, buf_r, sizeof buf_r);
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
			printf("收到客户端%s(%d)的消息：%s\n", cltip, cltport, buf_r);
			if(strcmp(buf_r, "over") == 0)
			{
				printf("通信结束\n");
				break;
			}
		}
		
		// 应答
		connect_status = n_send(connfd, buf_w, strlen(buf_w));
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
	_out:
	// 释放线程信息空间
	memset(&infos[index], 0, sizeof (struct info));
	// 关闭连接描述符
	if (connfd != -1)
	{
		while ((-1 == close(connfd)) && (EINTR == errno));
	}
}


//TCP 服务器关闭连接
int tcp_server_close(struct TcpInit *tcp_init)
{
	if(tcp_init->tcp_connfd != -1)          //关闭连接描述符
	{
		while ((-1 == close(tcp_init->tcp_connfd)) && (EINTR == errno));
	}
	// 关闭套接字
	while ((-1 == close(tcp_init->tcp_socket)) && (EINTR == errno));
	
	return 0;
}


// 在 infos 数组中查找空位
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
