#include"client.h"



int main(void)
{
	char *bcast_ip = BCAST;
	unsigned short bcast_port = SVR_PORT;
	int tcp_sockfd;
	
    //1,连接服务器    
	if(-1 == (tcp_sockfd = connect_server(bcast_ip, bcast_port)))
	{
		return -1;
	}
	
	//3, 功能选择
	function(tcp_sockfd);
	
}



