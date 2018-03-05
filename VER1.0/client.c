#include"client.h"



int main(void)
{
	char *bcast_ip = BCAST;
	unsigned short bcast_port = SVR_PORT;
	
    //1,连接服务器
    connect_server(bcast_ip, bcast_port);
    
		
}


