#include "server.h"

struct info infos[MAXBACKLOG];

int main()
{	
	struct TcpInit tcp_init;
	
	udp_server_init();//UDP 服务器初始化
	
	tcp_server_init(&tcp_init);

	while(1)
	{
		int index;
		
		//等待客户端连接
		tcp_init.tcp_connfd = uin_accept(tcp_init.tcp_socket, &(tcp_init.tcp_cltaddr));
		if(tcp_init.tcp_connfd == -1)
		{
			printf("连接失败\n");
			continue;
		}
		
		//查找第一个可用的位置
		index = get_first(); //不考虑满的情况
		infos[index].tcp_connfd = tcp_init.tcp_connfd;
		infos[index].tcp_cltaddr = tcp_init.tcp_cltaddr;
		
		//创建线程
		pthread_create(&infos[index].thread, NULL,
					tcp_server_handle, (void *)index);
	}
	
	if(tcp_server_close(&tcp_init) == 0)  //TCP 服务器关闭连接
	{
		printf("TCP 服务器关闭成功\n");
	}
	
	return 0;
}
