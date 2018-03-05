#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

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
*order:            1:注册,  2:登陆, 3:单聊, 4:查找好友, 5:群聊
*mine_id:   	   自己的账号
*friend_id:        好友的账号
*nickname:         自己的昵称
*friend_nickname   好友的昵称
*password          密码
*friend_list       好友列表
*information       消息内容
*send_msg_time     发送消息时间
*/
#typedef struct agr{
	int order;
	int mine_id;
	int friend_id;
	char nickname[20];
	char friend_nickname[20];
	char password[20];
	struct lis friend_list;
	char information[100];
	char send_msg_time[20];
}Agreement;


#endif
