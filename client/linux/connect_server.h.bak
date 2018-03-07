#ifndef _CONNECT_SERVER_H_
#define _CONNECT_SERVER_H_

#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>	 
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>




#define SVR_TOKEN  "Hello client"
#define CLT_TOKEN  "Hello server"

int connect_server(char *, unsigned short );
int udp_broadcast(struct sockaddr_in* , char *, unsigned short );
//SIGPIPE信号回调函数
void sig_callback(int sig);
//用TCP连接服务器,成功返回连接描述符，失败返回-1
int tcp_connect(struct sockaddr_in* pSvr_addr);





#endif


