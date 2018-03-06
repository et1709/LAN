#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#define FRIEND_NUMBER 20

/*
*好友列表
*friend_id:        好友账号列表
*friend_nickname:  好友昵称列表,与好友账号列表一一对应
*/
struct lis
{
	int friend_id[20];
	char friend_nickname[20][10];
};

/*
*order:            客户端请求命令: 1:注册,  2:登陆, 3:单聊, 
*								   4:查找好友, 5:群聊, 6:添加好友
*                  服务器回应命令: 11:注册成功,10:注册失败. 22:登陆成功,20:登陆失败
*                                  33:收到单聊信息, 44:查找到好友 40:查找好友失败
*								   
*mine_id:   	   自己的账号
*friend_id:        好友的账号
*nickname:         自己的昵称
*friend_nickname   好友的昵称
*password          密码
*friend_list       好友列表
*information       发送消息内容和回应命令内容
*send_msg_time     发送消息时间
*/
typedef struct agr{
	int order;
	int mine_id;
	int friend_id;
	char nickname[20];
	char friend_nickname[20];
	char password[20];
	struct lis friend_list;
	char information[100];
	char send_msg_time[20];
}AGREEMENT;


#endif
