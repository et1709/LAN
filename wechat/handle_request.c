#include "server.h"

struct sockaddr_in handle_request(AGREEMENT *packet, int index)
{
	switch(packet->order)
	{
		case 1:
			//register_req();   //注册
			break;

		case 2:
			login_req(packet, index);      //登录
			break;

		case 3:
			singleChat_req(packet); //单聊
			break;

		case 4:
			//search_req();     //查找好友
			break;

		case 5:
			//groupChat_req()   //群聊
			break;
	};
}


//1.注册
void register_req(AGREEMENT *packet, int index)
{
	AGREEMENT register_reply;
	char tmp_cmp[5];     //比较id大小的数组

	char id[6] = packet->mine_id;
	char nickname[11] = packet->nickname;
	char password[6] = packet->password;
	char age[4] = packet->age;
	char sex = pacjet->sex;

	//添加数据进入数据库，进行注册
	if(add_to_database(id, nickname, password, age, sex) == 0) //注册成功
	{
		register_reply.order = 11;
		//发送给客户端“注册成功”数据包
		r_send(infos[index].tcp_connfd, (void *)&re_reply, sizeof(AGREEMENT));
	}
	else
	{
		register_reply.order = 10;
		//发送给客户端“注册成功”数据包
		r_send(infos[index].tcp_connfd, (void *)&re_reply, sizeof(AGREEMENT));
	}
}

//2.登录
void login_req(AGREEMENT *packet, int index)
{
	int tmp_id;
	char tmp_nickname[20];
	AGREEMENT login_reply;
	//打开数据库
	//在数据库中查询账号，密码是否对应
	login_reply.order = 22;

	//返回的账号和昵称写入结构体中
	//cliInfos[cliNum].id = tmp_id;
	//strcpy(cliInfos[cliNum].nickname,tmp_nickname);

	//发送给客户端“登录成功”数据包
	r_send(infos[index].tcp_connfd, (void *)&login_reply, sizeof(AGREEMENT));
	//memset(&login_reply, 0 ,sizeof (AGREEMENT));//清空缓冲区

	pthread_mutex_lock(&mutex);//加锁
	/*---------- 临界区开始 -------------*/
	cliNum++;
	/*---------- 临界区结束 -------------*/
	pthread_mutex_unlock(&mutex);//解锁
}


//3.单聊
struct sockaddr_in singleChat_req(AGREEMENT *packet)
{
	int i;
	struct sockaddr_in target_cltaddr;

	for(i = 0; i < cliNum; i++)
	{
		//if(packet->friend_id == cliInfos[i].id)       //如果数据包中好友id已在客户缓存区中
		//{
			target_cltaddr = cliInfos[i].tcp_cltaddr; //将查找到的好友IP地址赋给目标IP地址
		//}
	}

	return target_cltaddr;                            //返回目标IP地址
}