#include"client.h"



int main(void)
{
	char *bcast_ip = BCAST;
	unsigned short bcast_port = SVR_PORT;
	
    //1,���ӷ�����
    connect_server(bcast_ip, bcast_port);
    
		
}


