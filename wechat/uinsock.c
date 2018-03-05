#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "uinsock.h"

#define MAXBACKLOG 50

static int ignore_sigpipe(void);


//安装捕捉SIGPIPE信号的信号处理函数
//创建一个绑定在port端口的TCP套接字并）,将套接字设为被动的（监听状态）
//允许服务器用同一端口号立即重启
//返回套接字的文件描述符
int uin_open_tcp(unsigned short port)
{
	int ret = 0;
	struct sockaddr_in server;
	int sock;
	int one = 1;
	// 忽略 SIGPIPE 信号
	if (-1 == ignore_sigpipe())
	{
		printf("in_ignore_sigpipe failed\n");
		return -1;
	}

	if (-1 == (sock = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("Failed to socket");
		return -1;
	}

	if (-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)))
	{
		perror("Failed setsockopt");
		ret = -1;
		goto _out;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (-1 == bind(sock, (struct sockaddr*)&server, sizeof(server)))
	{
		perror("Failed to bind");
		ret = -1;
		goto _out;
	}

	if (-1 == listen(sock, MAXBACKLOG))
	{
		perror("Failed to listen");
		ret = -1;
		goto _out;
	}

_out:
	if (-1 == ret)
	{
		while((-1 == close(sock)) && (EINTR == errno));
		return -1;
	}

	return sock;
}

//在fd上等待连接
//被信号中断重启accept
//accept可能会因为与资源不足有关的原因被信号终断返回错误;
//或者在完成三次握手之后,客户机断开连接,accept也可能被信号终断返回错误.
//这些情况下不能简单地退出,因为问题可能是暂时的
//cltaddr返回连接上的客户端地址
//返回一个通信文件描述符
int uin_accept(int fd, struct sockaddr_in *addr)
{
	int sock = -1;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	while ((-1 == (sock = accept(fd, (struct sockaddr*)addr, &addrlen))) && (EINTR == errno));

	if (-1 == sock)
	{
		perror("Failed to accept");
		return -1;
	}

	return sock;
}

//安装捕捉SIGPIPE信号的信号处理函数
//在地址ip和端口port上初始化一个对服务器的连接
//根据输入的服务器IP和端口号发起连接,等待TCP三次握手完成连接
//connect调用会被信号中断.但是不能被重启,因为网络子系统已经初始化了三次握手,
//这时连接请求的完成异步于程序的执行.
//应用程序必须调用select来检测描述符是否已准备好写操作.
//返回一个通信文件描述符
int uin_connect(const char *ip, unsigned short port)
{
	int ret;
	int sock;
	in_addr_t inaddr;
	struct sockaddr_in server;
	fd_set sockset;

	if (-1 == ignore_sigpipe())
	{
		printf("in_ignore_sigpipe failed\n");
		return -1;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (-1 == sock)
	{
		perror("Failed to socket");
		return -1;
	}

	inaddr = inet_addr(ip);

	if (INADDR_NONE == inaddr)
	{
		perror("Failed to inet_addr");
		ret = -1;
		goto _out;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inaddr;

	//connect已触发了三路握手，所以connect被信号中断以后不能直接重启，
	//必须等待三路握手结果
	ret = connect( sock , (struct sockaddr*)&server , sizeof(server) );
	if ((-1 == ret)	&& ((EINTR == errno) || (EALREADY == errno)))
	{
		FD_ZERO(&sockset);      // 清空描述符集
		FD_SET(sock, &sockset); // 设置 sock 到描述符集
		// 使用 select 函数实现监测 sock 可写才返回
		while((-1 == (ret = select(sock + 1, NULL, &sockset, NULL, NULL)))
				&& (EINTR == errno))//三路握手不成功，select会失败返回
		{
			// 如果 select 被信号中断，那么应该清空描述符集，
			// 并且重新设置 sock 到描述符集
			FD_ZERO(&sockset);
			FD_SET(sock, &sockset);
		}

		if (-1 == ret)
		{
			perror("Failed to connect");
			goto _out;
		}
	}

_out:
	if (-1 == ret)
	{
		while((-1 == close(sock)) && (EINTR == errno));
		return -1;

	}
	return sock;
}


//创建一个UDP套接字，如果port大于0，将套接字绑定到port
//返回套接字的文件描述符
int uin_open_udp(unsigned short port)
{
	int ret = 0;
	int sock;
	struct sockaddr_in server;
	int one = 1;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (-1 == sock)
	{
		perror("Failed to socket");
		return -1;
	}
	// 一般服务器才指定端口
	if (port > 0)
	{
		int reuse = 1;
		// 设置地址重用
		if (-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
							 &reuse, sizeof(reuse)))
		{
			perror("setsockopt failed!");
			ret = -1;
			goto _out;
		}

		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(port);
		// 绑定地址
		if (-1 == bind(sock, (struct sockaddr*)&server, sizeof(server)))
		{
			perror("Failed bind");
			ret = -1;
			goto _out;
		}
	}

_out:
	if (-1 == ret)
	{
		while((-1 == close(sock)) && (EINTR == errno));
		return -1;
	}

	return sock;
}


//用套接字fd向addr指定的接收者发送buf中count个字节
//信号终断时重启sendto
//返回实际发送的数据字节数
int uin_sendto(int fd, void *buf, int count, struct sockaddr_in *addr)
{
	int cnt  = 0;

	while(1)
	{
		cnt = sendto(fd, buf, count, 0, 
				(struct sockaddr*)addr,sizeof(struct sockaddr_in));
		if( (-1 == cnt) && (EINTR == errno) )
		{
			continue;
		}
		if (-1 == cnt)
		{
			perror("Failed to sendto");
			return -1;
		}
		else
		{
			break;
		}
	}
	return cnt;
}

//用套接字fd向ip和port指定的接收者发送buf中count个字节
//信号终断时重启sendto
//返回实际发送的字节数
int uin_sendtohost(int fd, void *buf, int count, char *ip, unsigned short port)
{
	struct sockaddr_in server;
	in_addr_t inaddr;

	inaddr = inet_addr(ip);

	if (INADDR_NONE == inaddr)
	{
		perror("Failed to inet_addr");
		return -1;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inaddr;

	return uin_sendto(fd, buf, count, &server);
}


//在套接字fd上等待最多size个字节数据
//被信号终断重启recvfrom
//返回接收到的数据字节数
int uin_recvfrom(int fd, void *buf, int size, struct sockaddr_in *addr)
{
	int cnt = 0;
	socklen_t len = sizeof( struct sockaddr_in);

	while(1)
	{
		cnt = recvfrom(fd, buf, size, 0, 
				(struct sockaddr*)addr, &len);
		if( (-1 == cnt)	&& (EINTR == errno))
		{
			continue;
		}

		if (-1 == cnt)
		{
			perror("Failed to recvfrom");
			return -1;
		}
		else
		{
			break;
		}
	}
	return cnt;
}


//在通信套接字fd上等待最多size个字数据
//被信号终断重启recv
//返回实际接收的数据字节数
int r_recv(int fd, void *buf, int size)
{
	int ret = 0;

	while((-1 == (ret = recv(fd, buf, size, 0))) && (EINTR == errno));

	return ret;
}

//用通信套接字fd发送buf中的count字节
//被信号中断则重启send
//返回实际发送的数据字节数
int r_send(int fd, const void* msg, int count)
{
	int ret = 0;

	while((-1 == (ret = send(fd, msg, count, 0))) && (EINTR == errno));

	return ret;
}

//用通信套接字fd发送buf中的count字节
//如果发送操作没有出错而count字节未发完，则继续发送余下数据直至count字节全部发完
//被信号中断则重启send
//返回已发送的数据字节数
int n_send(int fd, const void* msg, int count)
{
	char *pmsg = (char*)msg;  // 发送数据的起始地址
	int sending = count;      // 要发送的数据长度
	int sended = 0;           // 某次已发送成功的字节数
	int total = 0;            // 总共发送的字节数

	while (sending > 0)
	{
		sended = r_send(fd, pmsg, sending);

		if (-1 == sended)
		{
			perror("Failed to r_send");
			return -1;  // 有一次发送失败，则认为全部失败
		}

		pmsg += sended;
		sending -= sended;

		total += sended;
	}

	return total;
}


static int ignore_sigpipe(void)
{
	if (SIG_ERR == signal(SIGPIPE, SIG_IGN))
	{
		perror("Failed to signal");
		return -1;
	}

	return 0;
}

