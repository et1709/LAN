#include"client.h"



int main(void)
{
	char *bcast_ip = BCAST;
	unsigned short bcast_port = SVR_PORT;
	int tcp_sockfd;
	
    //1,���ӷ�����    
	if(-1 == (tcp_sockfd = connect_server(bcast_ip, bcast_port)))
	{
		return -1;
	}
	
	//3, ����ѡ��
	function(tcp_sockfd);
	
}



