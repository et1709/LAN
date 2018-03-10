#include "server.h"


/*-------------------------------------------------------
|	��������handle_request
|	����  ���Կͻ��˷��������ݰ����н���������ͬ����
|	����  �����ݰ��ĵ�ַpacket, �߳��±�index
|	���  ��
|	˵��  ������order����ֵ�ж�
-------------------------------------------------------*/
void handle_request(AGREEMENT *packet, int index)
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
			addFriend_req(packet, index);	//��Ӻ���
			break;

		case 7:
			friendList_req(packet, index);	//�鿴�����б�
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
		//������ݽ������ݿ⣬����ע��
		strcpy(packet->mine_id, get_next_qqNum());		//�����ݿ��л�ȡ������˺�
		if(add_to_database(packet->mine_id, packet->nickname, packet->password, packet->age, packet->sex) == 0) //ע��ɹ�
		{
			register_reply.order = 11;
			printf("Register succeed!\n");
			strcpy(register_reply.mine_id, packet->mine_id);
			//���͸��ͻ��ˡ�ע��ɹ������ݰ�
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
		//���͸��ͻ��ˡ�ע��ʧ�ܡ����ݰ�
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
	//���͸��ͻ��ˡ�ע��ʧ�ܡ����ݰ�
	r_send(infos[index].tcp_connfd, (void *)&register_reply, sizeof(AGREEMENT));
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
|	��������addFriend_req
|	����  ��6.��Ӻ���
|	����  �����ݰ��ĵ�ַpacket, �߳��±�index
|	���  ��
|	˵��  ��
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
	//�����ݿ��в�ѯ�Ƿ��д��˺�
	if(tmp_Info == NULL)
	{
		printf("go to database friend\n");
		addfriend_reply.order = 60;
		//���͸��ͻ��ˡ���Ӻ���ʧ�ܡ����ݰ�
		r_send(infos[index].tcp_connfd, (void *)&addfriend_reply, sizeof(AGREEMENT));
	}
	else
	{
		for(i = 0; i < cliNum; i++)
		{
			if(infos[index].tcp_connfd == cliInfos[i].client_connfd)
			{
				//��Ӻ�����Ϣ����ӷ��ĸ������ݿ���
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
					//���͸��ͻ��ˡ���Ӻ��ѳɹ������ݰ�
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
|	��������friendList_req
|	����  ��7.�鿴�����б�
|	����  �����ݰ��ĵ�ַpacket, �߳��±�index
|	���  ��
|	˵��  ��
-------------------------------------------------------*/
void login_req(AGREEMENT *packet, int index)