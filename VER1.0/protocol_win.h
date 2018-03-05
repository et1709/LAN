#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

/*
*�����б�
*friend_id:        �����˺��б�
*friend_nickname:  �����ǳ��б�,������˺��б�һһ��Ӧ
*/
struct lis
{
	int friend_id[20];
	char friend_nickname[20][10];
};

/*
*order:            1:ע��,  2:��½, 3:����, 4:���Һ���, 5:Ⱥ��
*mine_id:   	   �Լ����˺�
*friend_id:        ���ѵ��˺�
*nickname:         �Լ����ǳ�
*friend_nickname   ���ѵ��ǳ�
*password          ����
*friend_list       �����б�
*information       ��Ϣ����
*send_msg_time     ������Ϣʱ��
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
