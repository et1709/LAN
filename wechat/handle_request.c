#include "server.h"


/*-------------------------------------------------------
|	��������handle_request
|	����  ���Կͻ��˷��������ݰ����н���������ͬ����
|	����  �����ݰ��ĵ�ַpacket, �߳��±�index
|	���  ��
|	˵��  ������order����ֵ�ж�
-------------------------------------------------------*/
struct sockaddr_in handle_request(AGREEMENT *packet, int index)
{
	switch(packet->order)
	{
		case 1:
			register_req(packet, index);	//ע��
			break;

		case 2:
			login_req(packet, index);		//��¼
			break;

		case 3:
			singleChat_req(packet, index);	//����
			break;

		case 4:
			//search_req();					//���Һ���
			break;

		case 5:
			//groupChat_req()				//Ⱥ��
			break;

		case 6:
			//addFriend_req();				//��Ӻ���
			break;
	};
}


/*-------------------------------------------------------
|	��������register_req
|	����  ��1.ע��
|	����  �����ݰ��ĵ�ַpacket, �߳��±�index
|	���  ��
|	˵��  ��
-------------------------------------------------------*/
void register_req(AGREEMENT *packet, int index)
{
	AGREEMENT register_reply;
	//char tmp_cmp[5];     //�Ƚ�id��С������

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

	//������ݽ������ݿ⣬����ע��
	if(add_to_database(id, nickname, password, age, sex) == 0) //ע��ɹ�
	{
		register_reply.order = 11;
		printf("Register succeed!\n");
		strcpy(register_reply.mine_id, packet->mine_id);
		//���͸��ͻ��ˡ�ע��ɹ������ݰ�
		r_send(infos[index].tcp_connfd, (void *)&register_reply, sizeof(AGREEMENT));
	}
	else
	{
		register_reply.order = 10;
		printf("Register failed!\n");
		//���͸��ͻ��ˡ�ע��ɹ������ݰ�
		r_send(infos[index].tcp_connfd, (void *)&register_reply, sizeof(AGREEMENT));
	}
}


/*-------------------------------------------------------
|	��������login_req
|	����  ��2.��¼
|	����  �����ݰ��ĵ�ַpacket, �߳��±�index
|	���  ��
|	˵��  ��
-------------------------------------------------------*/
void login_req(AGREEMENT *packet, int index)
{
	char tmp_nickname[NICKNAME_LEN];
	AGREEMENT login_reply;
	int i;

	//�����ݿ��в�ѯ�˺ţ������Ƿ��Ӧ
	if(strcmp(packet->password, search_user_table_Passwd(packet->mine_id)) == 0) //������ƥ��
	{
		login_reply.order = 22;
		printf("Login succeed!\n");

		if(strcpy(tmp_nickname, search_user_table_qqName(packet->mine_id)) == 0)
		{
			printf("Copy nickname succeed!\n");
		}
		//���͸��ͻ��ˡ���¼�ɹ������ݰ�
		r_send(infos[index].tcp_connfd, (void *)&login_reply, sizeof(AGREEMENT));


		//��IP��ַ���˺ź��ǳ�д��ͻ��������ṹ����
		cliInfos[cliNum].client_connfd = infos[index].tcp_connfd;
		strcpy(cliInfos[cliNum].id, packet->mine_id);
		strcpy(cliInfos[cliNum].nickname,tmp_nickname);

		pthread_mutex_lock(&mutex);	//����
		/*---------- �ٽ�����ʼ -------------*/
		cliNum++;
		/*---------- �ٽ������� -------------*/
		pthread_mutex_unlock(&mutex);//����

		for(i = 0; i < cliNum; i++)
		{
			printf("IP:%d ID:%s NN:%s\n", cliInfos[i].client_connfd, 
									cliInfos[i].id, cliInfos[i].nickname);
		}
	}
	else	//���벻ƥ��											
	{
		login_reply.order = 20;
		printf("Login failed!\n");
		//���͸��ͻ��ˡ���¼ʧ�ܡ����ݰ�
		r_send(infos[index].tcp_connfd, (void *)&login_reply, sizeof(AGREEMENT));
	}
}


/*-------------------------------------------------------
|	��������singleChat_req
|	����  ��3.����
|	����  �����ݰ��ĵ�ַpacket, �߳��±�index
|	���  ��
|	˵��  ��
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

		if(strcmp(packet->friend_id, cliInfos[i].id) == 0)		//������ݰ��к���id���ڿͻ���������
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
|	��������addFriend_req
|	����  ��6.��Ӻ���
|	����  �����ݰ��ĵ�ַpacket, �߳��±�index
|	���  ��
|	˵��  ��
-------------------------------------------------------*/
void addFriend_req(AGREEMENT *packet, int index)
{
	AGREEMENT addfriend_reply;

	//�����ݿ��в�ѯ�Ƿ��д��˺�
	if( 1==0)
	{
		//add_to_qqlist_table(char *userName, char *qqID, char *qqName, char *p_age, char *q_sex);
	}

}