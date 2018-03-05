
#ifndef   _UIN_SOCK_H
#define   _UIN_SOCK_H

#include <netinet/in.h>

#ifndef NDEBUG
    #define PTrace printf
#else
    #define PTrace
#endif

/*
	TCP
int uin_open_tcp(unsigned short port);
int uin_accept(int fd, struct sockaddr_in *addr);
int uin_connect(const char *cp, unsigned short port);
int r_recv(int fd, void* buf, int size);
int r_send(int fd, const void* msg, int count);
int n_send(int fd, const void* msg, int count);

	UDP
int uin_open_udp(unsigned short port);
int uin_sendtohost(int fd, void *buf, int count, char *cp, unsigned short port);
int uin_sendto(int fd, void *buf, int count, struct sockaddr_in *addr);
int uin_recvfrom(int fd, void *buf, int size, struct sockaddr_in *addr);
*/



//UICI UDP接口
//创建一个UDP套接字，如果port大于0，将套接字绑定到port

//返回套接字的文件描述符
int uin_open_udp(unsigned short port);

//在套接字fd上等待最多size个字节数据
//被信号终断重启recvfrom
//返回接收到的数据字节数
int uin_recvfrom(int fd, void *buf, int size, struct sockaddr_in *addr);

//用套接字fd向addr指定的接收者发送buf中count个字节
//信号终断时重启sendto
//返回实际发送的数据字节数
int uin_sendto(int fd, void *buf, int count, struct sockaddr_in *addr);

//用套接字fd向ip和port指定的接收者发送buf中count个字节
//信号终断时重启sendto
//返回实际发送的字节数
int uin_sendtohost(int fd, void *buf, int count, char *ip, unsigned short port);


//UICI TCP接口

//安装捕捉SIGPIPE信号的信号处理函数
//创建一个绑定在port端口的TCP套接字并）,将套接字设为被动的（监听状态）
//允许服务器用同一端口号立即重启
//返回套接字的文件描述符
int uin_open_tcp(unsigned short port);

//在fd上等待连接
//被信号中断重启accept
//accept可能会因为与资源不足有关的原因被信号终断返回错误;
//或者在完成三次握手之后,客户机断开连接,accept也可能被信号终断返回错误.
//这些情况下不能简单地退出,因为问题可能是暂时的
//cltaddr返回连接上的客户端地址
//返回一个通信文件描述符
int uin_accept(int fd , struct sockaddr_in *cltaddr);

//安装捕捉SIGPIPE信号的信号处理函数
//在地址ip和端口port上初始化一个对服务器的连接
//根据输入的服务器IP和端口号发起连接,等待TCP三次握手完成连接
//connect调用会被信号中断.但是不能被重启,因为网络子系统已经初始化了三次握手,
//这时连接请求的完成异步于程序的执行.
//应用程序必须调用select来检测描述符是否已准备好写操作.
//返回一个通信文件描述符
int uin_connect(const char *ip, unsigned short port);


//对send/recv的封装

//用通信套接字fd发送buf中的count字节
//被信号中断则重启send
//返回实际发送的数据字节数
int r_send(int fd, const void* msg, int count);

//用通信套接字fd发送buf中的count字节
//如果发送操作没有出错而count字节未发完，则继续发送余下数据直至count字节全部发完
//被信号中断则重启send
//返回已发送的数据字节数
int n_send(int fd, const void* msg, int count);

//在通信套接字fd上等待最多size个字数据
//被信号终断重启recv
//返回实际接收的数据字节数
int r_recv(int fd, void* msg, int size);

//在通信套接字fd上等待count个字数据
//如果发送操作没有出错而接收未达到count字节，则继续接直至count字节全部收到
//被信号终断重启recv
//返回已接收的数据字节数
int n_recv(int fd, void* buf, int count);


#endif

