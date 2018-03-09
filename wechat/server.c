#include "server.h"

struct info infos[MAXBACKLOG];
struct ClientInfo cliInfos[MAXBACKLOG];			//已登录的客户信息
int cliNum = 0;									//客户端数
pthread_mutex_t mutex;							//互斥锁;


int main()
{	
	struct TcpInit tcp_init;
		
	tcp_server_init(&tcp_init);					//TCP 服务器初始化

	creat_database();							//创建数据库

	while(1)
	{
		int index;

		udp_server_init(index);					//UDP 服务器初始化,循环监听

		//等待客户端连接
		tcp_init.tcp_connfd = uin_accept(tcp_init.tcp_socket, &(tcp_init.tcp_cltaddr));
		if(tcp_init.tcp_connfd == -1)
		{
			printf("客户端连接失败\n");
			continue;
		}
		
		//查找第一个可用的位置
		index = get_first(); //不考虑满的情况
		infos[index].tcp_connfd = tcp_init.tcp_connfd;
		infos[index].tcp_cltaddr = tcp_init.tcp_cltaddr;
		
		//处理客户端请求线程
		pthread_create(&infos[index].thread, NULL,tcp_server_handle, (void *)index);
	}
	
	if(tcp_server_close(&tcp_init) == 0)  //TCP 服务器关闭连接
	{
		printf("TCP 服务器关闭成功\n");
	}
	
	return 0;
}
