#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#define FRIEND_NUMBER 20

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
*order:            �ͻ�����������: 1:ע��,  2:��½, 3:����, 
*								   4:���Һ���, 5:Ⱥ��, 6:��Ӻ���
*                  ��������Ӧ����: 11:ע��ɹ�,10:ע��ʧ��. 22:��½�ɹ�,20:��½ʧ��
*                                  33:�յ�������Ϣ, 44:���ҵ����� 40:���Һ���ʧ��
*								   
*mine_id:   	   �Լ����˺�
*friend_id:        ���ѵ��˺�
*nickname:         �Լ����ǳ�
*friend_nickname   ���ѵ��ǳ�
*password          ����
*friend_list       �����б�
*information       ������Ϣ���ݺͻ�Ӧ��������
*send_msg_time     ������Ϣʱ��
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
