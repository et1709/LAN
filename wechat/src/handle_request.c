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
	cliInfos[cliNum].id = tmp_id;
	strcpy(cliInfos[cliNum].nickname,tmp_nickname);

	//���͸��ͻ��ˡ���¼�ɹ������ݰ�
	r_send(infos[index].tcp_connfd, (void *)&login_reply, sizeof(AGREEMENT));

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
		if(packet->friend_id == cliInfos[i].id)       //������ݰ��к���id���ڿͻ���������
		{
			target_cltaddr = cliInfos[i].tcp_cltaddr; //�����ҵ��ĺ���IP��ַ����Ŀ��IP��ַ
		}
	}

	return target_cltaddr;                            //����Ŀ��IP��ַ
}