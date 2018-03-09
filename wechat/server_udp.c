#include "server.h"


/*-------------------------------------------------------
|	函数名：udp_server_init
|	功能  ：使用UICI接口进行UDP监听
|	输入  ：struct TcpInit *tcp_init
|	输出  ：-1——失败； 0——成功
|	说明  ：
-------------------------------------------------------*/
int udp_server_init(int index)
{
	int udp_socket;						//创建 UDP 套接字描述符
	in_port_t port = SOCKET_PORT;		//设定端口号为8080
	struct sockaddr_in udp_cltaddr;		//客户端 UDP 的IPv4地址
	socklen_t cltlen = sizeof(struct sockaddr_in);
	char buf_r[128] = "";				//创建读缓存区
	char buf_w[128] = "Hello client";	//创建写缓存区
	int on = 1;
	
	udp_socket = uin_open_udp(port);	//建立一个socket通信
	if(udp_socket == -1)
	{
		printf("创建UDP套接字失败\n");
		return -1;
	}
	else
	{
		printf("Socket success!\n");
		printf("Bind success!\n");
		printf("Listening......\n");
	}

	//接收客户端数据
	uin_recvfrom(udp_socket, buf_r, sizeof buf_r, &udp_cltaddr);
	printf("收到%s(%d)的消息：%s\n", inet_ntoa(udp_cltaddr.sin_addr),
			ntohs(udp_cltaddr.sin_port), buf_r);
			
	//判断消息后应答
	if(strcmp(buf_r, "Hello server") == 0)
	{
		uin_sendto(udp_socket, buf_w, strlen(buf_w), &udp_cltaddr);
	}

	//// 设置广播选项
	//if (-1 == setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, 
	//				&on, sizeof on))
	//{
	//	perror("设置广播失败");
	//	close(udp_socket);
	//}

	////判断消息后应答
	//if(strcmp(buf_r, "Hello server") == 0)
	//{
	//	//将新上线的客户发送给在线的客户端
	//	memset(buf_w, 0 ,sizeof buf_w);//清空缓冲区
	//	sprintf(buf_w, "%s(%d)", inet_ntoa(infos[index].tcp_cltaddr.sin_addr)
	//								,ntohs(infos[index].tcp_cltaddr.sin_port));
	//	strcat(buf_w, " 已上线...\n");
	//	uin_sendto(udp_socket, buf_w, strlen(buf_w), &(infos[index].tcp_cltaddr));
	//}
	
	//关闭套接字
	while((close(udp_socket)&&(errno = EINTR) == -1));
	
	return 0;
}




