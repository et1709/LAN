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

	//2,��ʾע���½�˵�
	login_menu();

	//3, ����ѡ��
	choose_function();
}

//����ѡ��
void choose_function(void)
{
	int num;
	while(1)
	{		
		while(scanf(&num) <= 0)
		{
			printf("�������, ����������!\n");
		}
		if(num > 3 || num < 0)
		{
			printf("��Ǹ,û�����ѡ��, ����������!\n");
		}
		else {
			break;
		}
	}
	switch(num)
	{
		case 0: 
			printf("��лʹ��, 88~~\n");
			break;
		case 1:
			//ע���˺�
			
			break;
		case 1:
			//��½
			
			break;
	}


	
}

