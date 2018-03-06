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
	while(1);
	//2,显示注册登陆菜单
	//login_menu();

	//3, 功能选择
	//choose_function();
}

//功能选择
void choose_function(void)
{
	int num;
	while(1)
	{		
		while(scanf(&num) <= 0)
		{
			printf("输入错误, 请重新输入!\n");
		}
		if(num > 3 || num < 0)
		{
			printf("抱歉,没有这个选项, 请重新输入!\n");
		}
		else {
			break;
		}
	}
	switch(num)
	{
		case 0: 
			printf("感谢使用, 88~~\n");
			break;
		case 1:
			//注册账号
			
			break;
		case 2:
			//登陆
			
			break;
	}


	
}

