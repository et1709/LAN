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
	char friend_id[6];
	char friend_nickname[20][11];
};

/*
*order:            �ͻ�����������: 1:ע��,  2:��½, 3:����, 
*								   4:���Һ���, 5:Ⱥ��, 6:��Ӻ���
*                  ��������Ӧ����: 11:ע��ɹ�,10:ע��ʧ��. 22:��½�ɹ�,20:��½ʧ��
*                                  33:�յ�������Ϣ, 44:���ҵ����� 40:���Һ���ʧ��
*								   
*mine_id:   	   
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
	char mine_id[6];			  //�Լ����˺�
	char friend_id[6];
	char herd_id[5];
	char nickname[11];
	char friend_nickname[11];
	char password[6];
	struct lis friend_list;
	char information[100];
	char send_msg_time[20];
	char age[4];                  //�û����������� 
	char sex[2];                  //�û��������Ա� 	
}AGREEMENT;


#endif
