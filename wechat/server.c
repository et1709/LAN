#include "server.h"

struct info infos[MAXBACKLOG];

int main()
{	
	int shmid;
	char *shmadd;
	char shmadd_buffer[SIZE_SHMADD];
	char buf_w[128] = "";
	struct TcpInit tcp_init;
	
	/********共享内存*********/
	shmid = shmid_create();
	//把共享内存区对象映射到调用进程的地址空间
	shmadd = shmat(shmid, NULL, 0);
	
	tcp_server_init(&tcp_init);//TCPP 服务器初始化

	while(1)
	{
		udp_server_init();     //UDP 服务器初始化,循环监听
		
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
		
		//将新上线的客户发送给在线的客户端
		//memset(buf_w, 0 ,sizeof buf_w);//清空缓冲区
		//sprintf(buf_w, "%s(%d)", inet_ntoa(infos[index].tcp_cltaddr.sin_addr)
		//							,ntohs(infos[index].tcp_cltaddr.sin_port));
		//strcat(buf_w, " 已上线...\n");
		//for(index = 0; index < MAXBACKLOG; index++)
		{
		//	send(tcp_init.tcp_connfd, buf_w, strlen(buf_w), 0);
		}
		
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


//创建共享存储区
int shmid_create()
{
	int shmid;
	
	if((shmid = shmget(IPC_PRIVATE, SIZE_SHMADD, 0777)) < 0)//创建一个共享内存对象
	{
		perror;
	}
	else
	{
		printf("Shmid success!\n");
	}
	
	return shmid;
}
