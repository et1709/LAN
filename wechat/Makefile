all:wechat

wechat:server.c server_tcp.c server_udp.c uinsock.c handle_request.c
	gcc $^ -o wechat -lpthread
