all:wechat

wechat:server.c server_tcp.c server_udp.c uinsock.c handle_request.c database.c
	gcc -I /usr/local/include -L /usr/local/lib $^ -o wechat -lpthread -lsqlite3
