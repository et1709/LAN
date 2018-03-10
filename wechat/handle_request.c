#include "server.h"


/*-------------------------------------------------------
|	函数名：handle_request
|	功能  ：对客户端发来的数据包进行解析，处理不同请求
|	输入  ：数据包的地址packet, 线程下标index
|	输出  ：
|	说明  ：根据order的数值判断
-------------------------------------------------------*/
void handle_request(AGREEMENT *packet, int index)
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
			addFriend_req(packet, index);	//添加好友
			break;

		case 7:
			friendList_req(packet, index);	//查看好友列表
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
	int id_len;
	int name_len;
	int passwd_len;
	int age_len;
	int sex_len;
	char err_buf[20] = "Length discrepancy";

	id_len = strlen(packet->mine_id);
	name_len = strlen(packet->nickname);
	passwd_len = strlen(packet->password);
	age_len = strlen(packet->age);
	sex_len = strlen(packet->sex);

	strcpy(id, packet->mine_id);
	strcpy(nickname, packet->nickname);
	printf("Packet nickname is : %s\n", packet->nickname);
	strcpy(password, packet->password);
	printf("Packet password is : %s\n", packet->password);
	strcpy(age, packet->age);
	printf("Packet age is : %s\n", packet->age);
	strcpy(sex, packet->sex);
	printf("Packet sex is : %s\n", packet->sex);

	if(name_len > 0 && name_len <= 10 && passwd_len >= 3 && passwd_len <=5
				&& age_len >= 1 && age_len <=3 && sex_len == 1)
	{
		//添加数据进入数据库，进行注册
		strcpy(packet->mine_id, get_next_qqNum());		//从数据库中获取分配的账号
		if(add_to_database(packet->mine_id, packet->nickname, packet->password, packet->age, packet->sex) == 0) //注册成功
		{
			register_reply.order = 11;
			printf("Register succeed!\n");
			strcpy(register_reply.mine_id, packet->mine_id);
			//发送给客户端“注册成功”数据包
			r_send(infos[index].tcp_connfd, (void *)&register_reply, sizeof(AGREEMENT));
		}
		else
		{
			goto _register_fail;
		}
	}
	else
	{
		register_reply.order = 10;
		printf("Register failed!\n");
		//发送给客户端“注册失败”数据包
		strcpy(register_reply.information, err_buf);
		if(r_send(infos[index].tcp_connfd, (void *)&register_reply, sizeof(AGREEMENT)) >0)
		{
			printf("register_reply.order: %d\n",register_reply.order);
			printf("register_reply.information: %s\n",register_reply.information);
			printf("Send succeed\n");
		}
	}

	return;

_register_fail:
	register_reply.order = 10;
	printf("Register failed!\n");
	//发送给客户端“注册失败”数据包
	r_send(infos[index].tcp_connfd, (void *)&register_reply, sizeof(AGREEMENT));
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
			strcpy(packet->friend_nickname, packet->nickname);
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
	User_Info *tmp_Info;
	int temp;
	int i;
	
	printf("go to add friend\n");
	tmp_Info = check_user_table_Info(packet->friend_id);

	printf("tmp_Info address is %p\n", tmp_Info);
	//printf("tmp_Info: %s %s\n",tmp_Info->qqIdStr, tmp_Info->qqNameStr);
	printf("Friend id is %s\n", packet->friend_id);
	printf("My id is %s\n", packet->mine_id);
	//在数据库中查询是否有此账号
	if(tmp_Info == NULL)
	{
		printf("go to database friend\n");
		addfriend_reply.order = 60;
		//发送给客户端“添加好友失败”数据包
		r_send(infos[index].tcp_connfd, (void *)&addfriend_reply, sizeof(AGREEMENT));
	}
	else
	{
		for(i = 0; i < cliNum; i++)
		{
			if(infos[index].tcp_connfd == cliInfos[i].client_connfd)
			{
				//添加好友信息到添加方的个人数据库中
				temp = add_to_qqlist_table(cliInfos[i].id, tmp_Info->qqIdStr,
				tmp_Info->qqNameStr, tmp_Info->AgeStr, tmp_Info->SexStr);

				printf("My id is %s\n", packet->nickname);
				printf("Fid is %s\n", tmp_Info->qqIdStr);
				printf("Fname is %s\n", tmp_Info->qqNameStr);
				printf("Fage is %s\n", tmp_Info->AgeStr);
				printf("Fsex is %s\n", tmp_Info->SexStr);
				if(temp == 0)
				{
					printf("Add friend succeed!\n");
					addfriend_reply.order = 66;
					//发送给客户端“添加好友成功”数据包
					r_send(infos[index].tcp_connfd, (void *)&addfriend_reply, sizeof(AGREEMENT));
				}
				else
				{
					printf("Add friend failed!\n");
				}
			}
		}
	}
}


/*-------------------------------------------------------
|	函数名：friendList_req
|	功能  ：7.查看好友列表
|	输入  ：数据包的地址packet, 线程下标index
|	输出  ：
|	说明  ：
-------------------------------------------------------*/
void login_req(AGREEMENT *packet, int index)