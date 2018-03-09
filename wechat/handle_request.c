#include "server.h"


/*-------------------------------------------------------
|	函数名：handle_request
|	功能  ：对客户端发来的数据包进行解析，处理不同请求
|	输入  ：数据包的地址packet, 线程下标index
|	输出  ：
|	说明  ：根据order的数值判断
-------------------------------------------------------*/
struct sockaddr_in handle_request(AGREEMENT *packet, int index)
{
	switch(packet->order)
	{
		case 1:
			register_req(packet, index);	//注册
			break;

		case 2:
			login_req(packet, index);		//登录
			break;

		case 3:
			singleChat_req(packet, index);	//单聊
			break;

		case 4:
			//search_req();					//查找好友
			break;

		case 5:
			//groupChat_req()				//群聊
			break;

		case 6:
			//addFriend_req();				//添加好友
			break;
	};
}


/*-------------------------------------------------------
|	函数名：register_req
|	功能  ：1.注册
|	输入  ：数据包的地址packet, 线程下标index
|	输出  ：
|	说明  ：
-------------------------------------------------------*/
void register_req(AGREEMENT *packet, int index)
{
	AGREEMENT register_reply;
	//char tmp_cmp[5];     //比较id大小的数组

	char id[ID_LEN];
	char nickname[NICKNAME_LEN];
	char password[PASSWORD_LEN];
	char age[AGE_LEN];
	char sex[SEX_LEN];

	strcpy(id, packet->mine_id);
	strcpy(nickname, packet->nickname);
	printf("Nickname is : %s\n", packet->nickname);
	strcpy(password, packet->password);
	strcpy(age, packet->age);
	printf("Age is : %s\n", age);
	printf("Packet age is : %s\n", packet->age);
	strcpy(sex, packet->sex);
	printf("Sex is : %s\n", sex);
	printf("Packet sex is : %s\n", packet->sex);

	//添加数据进入数据库，进行注册
	if(add_to_database(id, nickname, password, age, sex) == 0) //注册成功
	{
		register_reply.order = 11;
		printf("Register succeed!\n");
		strcpy(register_reply.mine_id, packet->mine_id);
		//发送给客户端“注册成功”数据包
		r_send(infos[index].tcp_connfd, (void *)&register_reply, sizeof(AGREEMENT));
	}
	else
	{
		register_reply.order = 10;
		printf("Register failed!\n");
		//发送给客户端“注册成功”数据包
		r_send(infos[index].tcp_connfd, (void *)&register_reply, sizeof(AGREEMENT));
	}
}


/*-------------------------------------------------------
|	函数名：login_req
|	功能  ：2.登录
|	输入  ：数据包的地址packet, 线程下标index
|	输出  ：
|	说明  ：
-------------------------------------------------------*/
void login_req(AGREEMENT *packet, int index)
{
	char tmp_nickname[NICKNAME_LEN];
	AGREEMENT login_reply;
	int i;

	//在数据库中查询账号，密码是否对应
	if(strcmp(packet->password, search_user_table_Passwd(packet->mine_id)) == 0) //密码相匹配
	{
		login_reply.order = 22;
		printf("Login succeed!\n");

		if(strcpy(tmp_nickname, search_user_table_qqName(packet->mine_id)) == 0)
		{
			printf("Copy nickname succeed!\n");
		}
		//发送给客户端“登录成功”数据包
		r_send(infos[index].tcp_connfd, (void *)&login_reply, sizeof(AGREEMENT));


		//将IP地址、账号和昵称写入客户缓存区结构体中
		cliInfos[cliNum].client_connfd = infos[index].tcp_connfd;
		strcpy(cliInfos[cliNum].id, packet->mine_id);
		strcpy(cliInfos[cliNum].nickname,tmp_nickname);

		pthread_mutex_lock(&mutex);	//加锁
		/*---------- 临界区开始 -------------*/
		cliNum++;
		/*---------- 临界区结束 -------------*/
		pthread_mutex_unlock(&mutex);//解锁

		for(i = 0; i < cliNum; i++)
		{
			printf("IP:%d ID:%s NN:%s\n", cliInfos[i].client_connfd, 
									cliInfos[i].id, cliInfos[i].nickname);
		}
	}
	else	//密码不匹配											
	{
		login_reply.order = 20;
		printf("Login failed!\n");
		//发送给客户端“登录失败”数据包
		r_send(infos[index].tcp_connfd, (void *)&login_reply, sizeof(AGREEMENT));
	}
}


/*-------------------------------------------------------
|	函数名：singleChat_req
|	功能  ：3.单聊
|	输入  ：数据包的地址packet, 线程下标index
|	输出  ：
|	说明  ：
-------------------------------------------------------*/
void singleChat_req(AGREEMENT *packet, int index)
{
	int i;
	int send_status;

	printf("go to chat\n");
	for(i = 0; i < cliNum; i++)
	{
		printf("IP:%d ID:%s NN:%s\n", cliInfos[i].client_connfd, 
									cliInfos[i].id, cliInfos[i].nickname);
		printf("receive information:%s\n", packet->information);
		printf("Friend id is %s\n", packet->friend_id);

		if(strcmp(packet->friend_id, cliInfos[i].id) == 0)		//如果数据包中好友id已在客户缓存区中
		{
			packet->order = 33;
			printf("Friend id is %s match client_buf id", packet->friend_id);
			printf("client_buf id is %s\n", cliInfos[i].id);
			r_send(cliInfos[i].client_connfd, (void *)packet, sizeof(AGREEMENT));
								
			if(send_status == -1)
			{
				printf("Transpond failed!\n");
			}
			else
			{
				printf("Transpond succeed!\n");
			}
		}
	}
}


/*-------------------------------------------------------
|	函数名：addFriend_req
|	功能  ：6.添加好友
|	输入  ：数据包的地址packet, 线程下标index
|	输出  ：
|	说明  ：
-------------------------------------------------------*/
void addFriend_req(AGREEMENT *packet, int index)
{
	AGREEMENT addfriend_reply;

	//在数据库中查询是否有此账号
	if( 1==0)
	{
		//add_to_qqlist_table(char *userName, char *qqID, char *qqName, char *p_age, char *q_sex);
	}

}