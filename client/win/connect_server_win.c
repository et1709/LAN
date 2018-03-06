#include"connect_server.h"

int global_tcp_sockfd;

/*
*����:    ����UDP���ҷ�����,����TCP���ӷ�����
*����ֵ:  �ɹ�����TCP�����׽���,ʧ�ܷ���-1
*/
int connect_server(char *ip, unsigned short port)
{     
	int rt;
    struct sockaddr_in svr_addr;
	//1:��UDP���ҷ�����
	rt = udp_broadcast(&svr_addr, ip, port);
	if(-1 == rt)
	{
		return -1;
	}

	//2,��TCP���ӷ�����
	global_tcp_sockfd = tcp_connect(&svr_addr);
	if(-1 == global_tcp_sockfd)
	{
		return -1;
	}
	
	return global_tcp_sockfd;
}

//SIGPIPE�źŻص�����
void sig_callback(int sig)
{
	printf("��������ʧ!");
	close(global_tcp_sockfd);
}

//��TCP���ӷ�����,�ɹ�����������������ʧ�ܷ���-1
int tcp_connect(struct sockaddr_in* pSvr_addr)
{
	int tcp_sockfd;
	int cnt;
	char recv_buf[128];
	int recv_buf_len = sizeof(recv_buf);
	struct sockaddr_in svr_addr = *pSvr_addr;
	int svr_addr_len = sizeof(struct sockaddr_in);

	if(SIG_ERR == signal(SIGPIPE, sig_callback))
	{
		perror("ע�� SIGPIPE �ź�ʧ��!");
		return -1;
	}
	
	// 1. ����TCP�׽���
	tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == tcp_sockfd)
	{
		perror("����TCP�׽���ʧ��!");
		return -1;
	}

	// 2. ��������
	if(-1 == connect(tcp_sockfd, (struct sockaddr *)&svr_addr, 
		svr_addr_len))
	{
		perror("TCP���ӷ�����ʧ��!");
		goto out;
	}

	while(1)
	{
		while((-1 == (cnt = send(tcp_sockfd, CLT_TOKEN, sizeof(CLT_TOKEN), 0))) 
			   && (EINTR == errno));
		if(-1 == cnt)
		{
			perror("TCP������Ϣ��������ʧ��!");
			sleep(1);
			continue;
		}
		else if(cnt > 0)
		{
			printf("TCP�ɹ�������Ϣ%s��������\n", CLT_TOKEN);
			break;
		}
	}

	memset(recv_buf, 0, recv_buf_len);
	while(1)
	{	
		while( (-1 == (cnt = recv(tcp_sockfd, recv_buf, recv_buf_len, 0))) 
				&& (EINTR == errno));
		if(-1 == cnt)
		{
			perror("TCP���շ�������Ϣʧ��!");
			
		}
		else if(cnt > 0)
		{
			printf("���յ�TCP��������Ϣ:%s\n", recv_buf);
			printf("TCP�ɹ������Ϸ����� %s(%d)\n",
					inet_ntoa(svr_addr.sin_addr),
					ntohs(svr_addr.sin_port));
			break;
		}		
	}

	return tcp_sockfd;
	
out:
	close(tcp_sockfd);
	return -1;
	
}

//��UDP���ҷ�����
int udp_broadcast(struct sockaddr_in* pSvr_addr, char *ip, unsigned short port)
{
    int udp_socket;
    int reuse = 1;
	fd_set rset;
	char *bcast_ip = ip;
	unsigned short bcast_port = port;
	
	printf("bcast_ip = %s\n", bcast_ip);
	printf("bcast_port = %d\n", bcast_port);
	
	struct timeval timevalue = {2, 0};
	int ret;
    struct sockaddr_in bcast_addr;
    int bcast_addr_len = sizeof(struct sockaddr_in);
	int svr_len = sizeof(struct sockaddr_in);
	char recv_buf[128];
	
	//1.����UDP�׽���
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == udp_socket)
    {
        perror("����UDP�׽���ʧ��!");
        return -1;
    }
    
	//2,���ù㲥ѡ��
    if(-1 == setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, 
        &reuse, sizeof(reuse)))
    {
        perror("���õ�ַ������ʧ��!");
        close(udp_socket);
        return -1;
    }
       
	//3,���ýӹ㲥��ַ
    bcast_addr.sin_family = AF_INET;
    bcast_addr.sin_addr.s_addr = inet_addr(bcast_ip);
    bcast_addr.sin_port = htons(bcast_port);
    
    //ѭ����ͣ�ķ��㲥
    while(1)
    {
        //4,���͹㲥��Ϣ
        if(-1 == sendto(udp_socket, CLT_TOKEN, strlen(CLT_TOKEN),
            0, (struct sockaddr *)&bcast_addr, bcast_addr_len))
        {
            perror("���͹㲥ʧ��!");
			close(udp_socket);
        	return -1;
        }
	    //select�����Ƿ�����Ϣ�ɶ�
	    FD_ZERO(&rset);
		FD_SET(udp_socket, &rset);
		while( (-1 == (ret = select(udp_socket+1, &rset,
				NULL, NULL, &timevalue))) && (EINTR == errno) );
		if(-1 == ret)
		{
			perror("���ɶ���Ϣʧ��!");
			sleep(2);
			continue;
		}
		else if(0 == ret)
		{
			printf("������δ�ҵ�, ���Ժ�...\n");
			sleep(2);
			continue;
		}
		
	    //5,���ջ�Ӧ��Ϣ
		memset(pSvr_addr, 0, svr_len);
		memset(recv_buf, 0, sizeof(recv_buf));
		if(-1 == recvfrom(udp_socket, recv_buf, sizeof(recv_buf), 0,
			(struct sockaddr *)pSvr_addr, &svr_len))
		{
			perror("UDP���շ�������Ӧʧ��");
		}
		else
		{
			//6,���յ���Ϣ,��֤�ǲ��Ƿ�����,�Ǿ��˳�ѭ��
			if(0 == strcmp(SVR_TOKEN, recv_buf))
			{
				printf("���ҵ������� %s(%d)\n",
					inet_ntoa(pSvr_addr->sin_addr),
					ntohs(pSvr_addr->sin_port));
			}
			break;
		}       
    }

	//7,�ر��׽���
	close(udp_socket);
	
	return 0;
}



