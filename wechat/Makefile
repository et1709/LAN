all:wechat

wechat:server.c server_tcp.c server_udp.c uinsock.c handle_request.c database.c
	gcc -I /home/luo/install/sqlite3/include -L /home/luo/install/sqlite3/lib $^ -o wechat -lpthread -lsqlite3
