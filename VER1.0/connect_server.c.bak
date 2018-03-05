#include"connect_server.h"

int connect_server(char *ip, unsigned short port)
{     
	int rt;	
    struct sockaddr_in svr_addr;
	//1:用UDP查找服务器
	rt = udp_broadcast(&svr_addr, ip, port);
	if(-1 == rt)
	{
		return -1;
	}
	
	//2,用TCP连接服务器
	//tcp_connect(&svr_addr);
	
	return 0;
}

//用TCP连接服务器
//int tcp_connect(struct sockaddr_in* pSvr_addr)
//{
	




	
//}

//用UDP查找服务器
int udp_broadcast(struct sockaddr_in* pSvr_addr, char *ip, unsigned short port)
{
    int udp_socket;
    int reuse = 1;
	fd_set rset;
	char *bcast_ip = ip;
	unsigned short bcast_port = port;
	
	printf("bcast_ip = %s\n", bcast_ip);
	printf("bcast_port = %d\n", bcast_port);
	
	struct timeval timevalue = {2, 0};
	int ret;
    struct sockaddr_in bcast_addr;
    int bcast_addr_len = sizeof(struct sockaddr_in);
	int svr_len = sizeof(struct sockaddr_in);
	char recv_buf[128];
	
	//1.创建UDP套接字
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == udp_socket)
    {
        perror("创建UDP套接字失败!");
        return -1;
    }
    
	//2,设置广播选项
    if(-1 == setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, 
        &reuse, sizeof(reuse)))
    {
        perror("设置地址可重用失败!");
        close(udp_socket);
        return -1;
    }
       
	//3,设置接广播地址
    bcast_addr.sin_family = AF_INET;
    bcast_addr.sin_addr.s_addr = inet_addr(bcast_ip);
    bcast_addr.sin_port = htons(bcast_port);
    
    //循环不停的发广播
    while(1)
    {
        //4,发送广播消息
        if(-1 == sendto(udp_socket, CLT_TOKEN, strlen(CLT_TOKEN),
            0, (struct sockaddr *)&bcast_addr, bcast_addr_len))
        {
            perror("发送广播失败!");
			close(udp_socket);
        	return -1;
        }
	    //select监听是否有消息可读
	    FD_ZERO(&rset);
		FD_SET(udp_socket, &rset);
		while( (-1 == (ret = select(udp_socket+1, &rset,
				NULL, NULL, &timevalue))) && (EINTR == errno) );
		if(-1 == ret)
		{
			perror("监测可读消息失败!");
			continue;
		}
		else if(0 == ret)
		{
			printf("服务器未找到, 请稍后...\n");
			continue;
		}
		
	    //5,接收回应消息
		memset(pSvr_addr, 0, svr_len);
		memset(recv_buf, 0, sizeof(recv_buf));
		if(-1 == recvfrom(udp_socket, recv_buf, sizeof(recv_buf), 0,
			(struct sockaddr *)pSvr_addr, &svr_len))
		{
			perror("UDP接收服务器回应失败");
		}
		else
		{
			//6,接收到消息,验证是不是服务器,是就退出循环
			if(0 == strcmp(SVR_TOKEN, recv_buf))
			{
				printf("已找到服务器 %s(%d)\n",
					inet_ntoa(pSvr_addr->sin_addr),
					ntohs(pSvr_addr->sin_port));
			}
			break;
		}       
    }

	//7,关闭套接字
	close(udp_socket);
	
	return 0;
}



