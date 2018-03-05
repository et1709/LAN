#include "server.h"


//UICI UDP 服务器初始化
int udp_server_init()
{
	int udp_socket;                   //创建 UDP 套接字描述符
	in_port_t port = SOCKET_PORT;     //设定端口号为8080
	struct sockaddr_in udp_cltaddr;   //创建客户端 UDP 的IPv4地址结构体
	socklen_t cltlen = sizeof(struct sockaddr_in);
	char buf_r[128] = "";             //创建读缓存区
	char buf_w[128] = "Hello client";//创建写缓存区
	
	udp_socket = uin_open_udp(port);  //建立一个socket通信
	if(udp_socket == -1)
	{
		printf("创建UDP套接字失败\n");
		return -1;
	}
	else
	{
		printf("Socket successful!\n");
		printf("Bind successful!\n");
	}
	
	printf("Listening......\n");
	//接收客户端数据
	uin_recvfrom(udp_socket, buf_r, sizeof buf_r, &udp_cltaddr);
	printf("收到%s(%d)的消息：%s\n", inet_ntoa(udp_cltaddr.sin_addr),
			ntohs(udp_cltaddr.sin_port), buf_r);
			
	//判断消息后应答
	if(0 == strcmp(buf_r, "Hello server"))
	{
		uin_sendto(udp_socket, buf_w, strlen(buf_w), &udp_cltaddr);
	}	
	
	//关闭套接字
	while((close(udp_socket)&&(errno = EINTR) == -1));
	
	return 0;
}




