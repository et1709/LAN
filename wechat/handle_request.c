#include "server.h"

struct sockaddr_in handle_request(AGREEMENT *packet, int index)
{
	switch(packet->order)
	{
		case 1:
			//register_req();   //ע��
			break;

		case 2:
			login_req(packet, index);      //��¼
			break;

		case 3:
			singleChat_req(packet); //����
			break;

		case 4:
			//search_req();     //���Һ���
			break;

		case 5:
			//groupChat_req()   //Ⱥ��
			break;
	};
}


//1.ע��
void register_req(AGREEMENT *packet, int index)
{
	AGREEMENT register_reply;
	char tmp_cmp[5];     //�Ƚ�id��С������

	char id[6] = packet->mine_id;
	char nickname[11] = packet->nickname;
	char password[6] = packet->password;
	char age[4] = packet->age;
	char sex = pacjet->sex;

	//������ݽ������ݿ⣬����ע��
	if(add_to_database(id, nickname, password, age, sex) == 0) //ע��ɹ�
	{
		register_reply.order = 11;
		//���͸��ͻ��ˡ�ע��ɹ������ݰ�
		r_send(infos[index].tcp_connfd, (void *)&re_reply, sizeof(AGREEMENT));
	}
	else
	{
		register_reply.order = 10;
		//���͸��ͻ��ˡ�ע��ɹ������ݰ�
		r_send(infos[index].tcp_connfd, (void *)&re_reply, sizeof(AGREEMENT));
	}
}

//2.��¼
void login_req(AGREEMENT *packet, int index)
{
	int tmp_id;
	char tmp_nickname[20];
	AGREEMENT login_reply;
	//�����ݿ�
	//�����ݿ��в�ѯ�˺ţ������Ƿ��Ӧ
	login_reply.order = 22;

	//���ص��˺ź��ǳ�д��ṹ����
	//cliInfos[cliNum].id = tmp_id;
	//strcpy(cliInfos[cliNum].nickname,tmp_nickname);

	//���͸��ͻ��ˡ���¼�ɹ������ݰ�
	r_send(infos[index].tcp_connfd, (void *)&login_reply, sizeof(AGREEMENT));
	//memset(&login_reply, 0 ,sizeof (AGREEMENT));//��ջ�����

	pthread_mutex_lock(&mutex);//����
	/*---------- �ٽ�����ʼ -------------*/
	cliNum++;
	/*---------- �ٽ������� -------------*/
	pthread_mutex_unlock(&mutex);//����
}


//3.����
struct sockaddr_in singleChat_req(AGREEMENT *packet)
{
	int i;
	struct sockaddr_in target_cltaddr;

	for(i = 0; i < cliNum; i++)
	{
		//if(packet->friend_id == cliInfos[i].id)       //������ݰ��к���id���ڿͻ���������
		//{
			target_cltaddr = cliInfos[i].tcp_cltaddr; //�����ҵ��ĺ���IP��ַ����Ŀ��IP��ַ
		//}
	}

	return target_cltaddr;                            //����Ŀ��IP��ַ
}