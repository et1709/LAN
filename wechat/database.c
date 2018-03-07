#include <stdio.h>
#include <unistd.h>
#include "sqlite3.h"
#include "string.h"

sqlite3 *db;
typedef struct {
	
	char *parmt[5];
} callback;
callback backupdat;

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: file_path: 需要判断的文件名
*******************************************************************************/
int is_file_exist(const char *file_path)
{
	if (file_path == NULL) 
		return -1;
	else if (access(file_path, F_OK) == 0) 	
		return 0;
	
	return -1;
}



/*******************************************************************************
 * 名称: 
 * 功能: 在 database.db
 * 形参: 无
 * 返回: 无
 * 说明: 返回 0：成功 -1：失败
*******************************************************************************/
int creat_database(void)
{
	// 定义 sqlite3 指针
	char *pErrMsg = 0;
	char *dbname = "database.db";
	int err;
	const char *table1 = "create table user_table(qqID TEXT, qqNAME TEXT, PASSW TEXT, userAGE TEXT, userSEX TEXT);";
	
	if (-1 == is_file_exist("database.db")) {
		
		// 创建一个数据库
		if (SQLITE_OK != (err = sqlite3_open(dbname, &db))) {
		
			printf("创建数据库 %s 失败(%s)\n", dbname, sqlite3_errstr(err));
			return -1;
		}
		// 执行建表SQL, 创建一个table
		sqlite3_exec(db, table1, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		
		// 关闭数据库
		sqlite3_close(db);
	}
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 在 userName 数据库下创建 qqlist_table, mygroup_table, othergroup_table, 
		 friendchat_table 和 groupchat_table
 * 形参: 无
 * 返回: 无
 * 说明: 返回 0：成功 -1：失败 (用户注册的时候用到)
*******************************************************************************/
int creat_userbase(char *userName)
{
	// 定义 sqlite3 指针
	char *pErrMsg = 0;
	char str[100];
	int err;
	const char *table1 = "create table qqlist_table(qqID TEXT, qqNMAE TEXT, userAGE TEXT, userSEX TEXT);";
	const char *table2 = "create table mygroup_table(qqID TEXT, qqNAME TEXT, popNUM TEXT);";
	const char *table3 = "create table othergroup_table(qqID TEXT, qqNAME TEXT, popNUM TEXT);";
	
	memset(&str, 0, sizeof(str));
	strcpy(str, userName);
	strcat(str, ".db"); 
	
	if (-1 == is_file_exist(str)) {
		
		// 创建一个数据库
		if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
		
			printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
			return -1;
		}
		// 执行建表SQL, 创建一个table
		sqlite3_exec(db, table1, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建用户数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		sqlite3_exec(db, table2, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建用户数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		sqlite3_exec(db, table3, NULL, NULL, &pErrMsg);
		if (err != SQLITE_OK) {
			
			fprintf(stderr, "创建用户数据 table 失败: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);
		}
		
		// 关闭数据库
		sqlite3_close(db);
	}
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: p_qs：账号 p_qn：昵称 q_paswd：密码 q_sex：性别
*******************************************************************************/
int add_to_database(char *qqID, char *qqName, char *qqPaswd, char *p_age, char *q_sex)
{
	int err;
	char sql[100];
	char *pErrMsg = 0;
	
	// 打开数据库
	if (SQLITE_OK != (err = sqlite3_open("database.db", &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", "database.db", sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "insert into user_table values('");
	strcat(sql, qqID); 
	strcat(sql, "', '");
    strcat(sql, qqName); 	// 字符串拼接
	strcat(sql, "', '");
	strcat(sql, qqPaswd); 	// 字符串拼接
	strcat(sql, "', '");
	strcat(sql, p_age); 	// 字符串拼接
	strcat(sql, "', '");
	strcat(sql, q_sex); 	// 字符串拼接
	strcat(sql, "')");
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &pErrMsg)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	// 新用户注册之后 创建一个用户数据库
	creat_userbase(qqName);
	
	// 关闭数据库
	sqlite3_close(db);
	
	return 0;
}



/*******************************************************************************
 * 名称: 
 * 功能: 添加好友的时候创建一个与好友的聊天记录 table
 * 形参: 无
 * 返回: 无
 * 说明: chatName：朋友的昵称
*******************************************************************************/
int create_friendchat_table(char *chatName)
{
	int err;
	char sql[100];
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "create table ");
	strcat(sql, chatName); 
	strcat(sql, "_chatTable(qqID TEXT, qqName TEXT, qqChat TEXT, qqTime TEXT)");  
	// 创建一个数据库
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		return -1;
	}
	
	return 0;	// OK
}

/*******************************************************************************
 * 名称: 
 * 功能: 添加数据到 userName 用户数据库的 qqlist_table 列表
 * 形参: 无
 * 返回: 无
 * 说明: 无 
*******************************************************************************/
int add_to_qqlist_table(char *userName, char *qqID, char *qqName, char *p_age, char *q_sex)
{
	int err;
	char sql[100];
	char str[100];
	
	memset(&str, 0, sizeof(str));
	strcpy(str, userName);
	strcat(str, ".db"); 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "insert into qqlist_table values('");
	strcat(sql, qqID); 
	strcat(sql, "', '");
    strcat(sql, qqName); 	// 字符串拼接
	strcat(sql, "', '");
	strcat(sql, p_age); 	// 字符串拼接
	strcat(sql, "', '");
	strcat(sql, q_sex); 	// 字符串拼接
	strcat(sql, "')");
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	create_friendchat_table(qqName);

	// 关闭数据库
	sqlite3_close(db);
	
	return 0;	// OK
}

/*******************************************************************************
 * 名称: 
 * 功能: 添加群的时候创建一个与群的聊天记录 table
 * 形参: 无
 * 返回: 无
 * 说明: listName: 创建 table 的名字
*******************************************************************************/
int create_group_list_table(char *listName)
{
	int err;
	char sql[100];
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "create table ");
	strcat(sql, listName); 
	strcat(sql, "_listTable(qqID TEXT, qqName TEXT)");  
	// 创建一个数据库
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		return -1;
	}
	
	return 0;	// OK
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
int create_groupchat_table(char *chatName)
{
	int err;
	char sql[100];
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "create table ");
	strcat(sql, chatName); 
	strcat(sql, "_chatTable(qqID TEXT, qqName TEXT, qqChat TEXT,aaTime TEXT)");  
	// 创建一个数据库
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		return -1;
	}
	
	return 0;	// OK
}

/*******************************************************************************
 * 名称: 
 * 功能: 添加数据到 userName 用户数据库的 mygroup_table 列表
 * 形参: 无
 * 返回: 无
 * 说明: p_gID：群号  p_gName：群昵称 p_popNum：群成员数量
*******************************************************************************/
int add_to_mygroup_table(char *userName, char *qqID, char *qqName, char *popNum)
{
	int err;
	char sql[100];
	char str[100];
	
	memset(&str, 0, sizeof(str));
	strcpy(str, userName);
	strcat(str, ".db"); 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "insert into mygroup_table values('");
	strcat(sql, qqID); 			// 字符串拼接
	strcat(sql, "', '");
	strcat(sql, qqName); 		// 字符串拼接
	strcat(sql, "', '");	
    strcat(sql, popNum); 		// 字符串拼接
	strcat(sql, "')");
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	create_group_list_table(qqName);
	create_groupchat_table(qqName);
	
	// 关闭数据库
	sqlite3_close(db);	
	
	return 0;	// OK
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
int add_to_othergroup_table(char *userName, char *qqID, char *qqName, char *popNum)
{
	int err;
	char sql[100];
	char str[100];
	
	memset(&str, 0, sizeof(str));
	strcpy(str, userName);
	strcat(str, ".db"); 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "insert into 'othergroup_table' values('");
	strcat(sql, qqID); 			// 字符串拼接
	strcat(sql, "', '");
	strcat(sql, qqName); 		// 字符串拼接
	strcat(sql, "', '");	
    strcat(sql, popNum); 		// 字符串拼接
	strcat(sql, "')");
	
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	create_group_list_table(qqName);	
	create_groupchat_table(qqName);

	// 关闭数据库
	sqlite3_close(db);
	
	return 0;	// OK
}

/*******************************************************************************
 * 名称: 
 * 功能: 添加数据到 userName 用户数据库的 friendchat_table 列表
 * 形参: 无
 * 返回: 无
 * 说明: 无 
*******************************************************************************/
int add_to_friendchat_table(char *userName, char *qqName, char *qqID, char *chatText, char *time)
{
	int err;
	char sql[100];
	char str[100];
	
	memset(&str, 0, sizeof(str));
	strcpy(str, userName);
	strcat(str, ".db"); 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("exit\n");
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "insert into ");
	strcat(sql, qqName); 		// 字符串拼接
	strcat(sql, "_chatTable values('");
	strcat(sql, qqName); 		// 字符串拼接
	strcat(sql, "', '");	
	strcat(sql, qqID); 			// 字符串拼接
	strcat(sql, "', '");		
    strcat(sql, chatText); 		// 字符串拼接
	strcat(sql, "', '");	
    strcat(sql, time); 			// 字符串拼接
	strcat(sql, "')");
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	// 关闭数据库
	sqlite3_close(db);
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无 
*******************************************************************************/
int add_to_grouplist_table(char *userName, char *groupName, char *qqID, char *qqName)
{
	int err;
	char sql[100];
	char str[100];
	
	memset(&str, 0, sizeof(str));
	strcpy(str, userName);
	strcat(str, ".db"); 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	sprintf(sql, "insert into %s_listTable values('%s', '%s');", groupName, qqID, qqName);
	printf("str is: %s\n", sql);
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	// 关闭数据库
	sqlite3_close(db);
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 添加数据到 userName 用户数据库的 friendchat_table 列表
 * 形参: 无
 * 返回: 无
 * 说明: 无 
*******************************************************************************/
int add_to_groupchat_table(char *userName, char *groupName, char *qqName, char *qqID, char *chatText, char *time)
{
	int err;
	char sql[100];
	char str[100];
	
	memset(&str, 0, sizeof(str));
	strcpy(str, userName);
	strcat(str, ".db"); 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	strcpy(sql, "insert into ");
	strcat(sql, groupName); 	// 字符串拼接
	strcat(sql, "_chatTable values('");
	strcat(sql, qqName); 		// 字符串拼接
	strcat(sql, "', '");	
	strcat(sql, qqID); 			// 字符串拼接
	strcat(sql, "', '");		
    strcat(sql, chatText); 		// 字符串拼接
	strcat(sql, "', '");	
    strcat(sql, time); 			// 字符串拼接
	strcat(sql, "')");
	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, NULL)) {

		// 关闭数据库
		sqlite3_close(db);
		return -1;
	}
	
	// 关闭数据库
	sqlite3_close(db);
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
int select_callback(void *data, int col_count, char **col_values, char **col_Name)
{
	int i;
	
	// 每条记录回调一次该函数,有多少条就回调多少次
	for (i=0; i<col_count; i++) {
		
		backupdat.parmt[i] = col_values[i];
	}

	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
callback *read_table(char *userName, char *tabName, char *qqID)
{
	int err;
	char sql[100];
	char str[100];
	char *pErrMsg = 0;
	
	memset(&str, 0, sizeof(str));
	strcpy(str, userName);
	strcat(str, ".db"); 
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return NULL;
	}

	sprintf(sql, "select * from %s where qqID = '%s';", tabName, qqID);
    err = sqlite3_exec(db, sql, select_callback, 0, &pErrMsg);
    if (err != SQLITE_OK) {
    
        // 关闭数据库
		sqlite3_close(db);
        return NULL;
    }
	
	// 关闭数据库
	sqlite3_close(db);
	
	return &backupdat;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
int read_specify(char *q_tab, char *q_qs) 
{
	char *pErrMsg = 0;
	char sq5[100];
	
	memset(&sq5, 0, sizeof(sq5));
	strcpy(sq5, "select * from ");
	strcat(sq5, q_tab); 
	strcat(sq5, "where qs='"); 
	strcat(sq5, q_qs);
	strcat(sq5, "'");
	if (-1 == sqlite3_exec(db, sq5, NULL, NULL, &pErrMsg)) {
		
		return -1;
	}
	
	return 0;
}

/*******************************************************************************
 * 名称: 
 * 功能: 
 * 形参: 无
 * 返回: 无
 * 说明: 无
*******************************************************************************/
int main(void)
{
	callback *str;
	
	// (创建一个数据库存放所以用户的注册信息) database.db
	creat_database();
	
	// 想 database.db 数据库添加 用户信息同时给注册的用户创建一个用户数据库
	// liyan.db(qqlist_table, mygroup_table, othergroup_table, friendchat_table, groupchat_table)
	add_to_database("1378139998", "liyan", "123456781", "21", "F");	
	
	// 向用户数据库的 好友列表 添加好友信息同时创建一个和该好友的聊天记录 table
	// qqlist_table(xiaoqi|1378139999|peiqi|123455441|21|F) peiqi_chatTable
	add_to_qqlist_table("liyan", "1378139997", "yujuan", "18", "F");
	add_to_qqlist_table("liyan", "1378139998", "peiqi", "21", "F");
	add_to_qqlist_table("liyan", "1378139999", "xiaoqing", "21", "F");
	
	// 向该用户自己的 mygroup_table 添加群信息同时创建一个群成员 table 和 群聊天记录 table
	// mygroup_table(294583762|qq_linux|8) qq_linux_listTable qq_linux_chatTable
	add_to_mygroup_table("liyan", "294583762", "qq_linux", "8");
	
	// othergroup_table(xiaoqi|154583797|qq_window|9) qq_window_listTable qq_window_chatTable
	add_to_othergroup_table("liyan", "154583797", "qq_window", "9");
	
	// 向该用户的(谁)的聊天记录 table 添加数据
	add_to_friendchat_table("liyan", "peiqi", "1328743347", "ni hao!", "9:00");
	
	// 记录该用户的(群)里的(谁) 的聊天信息
	add_to_groupchat_table("liyan", "qq_linux", "peiqi", "1328743347", "ni hao!", "9:00");
	
	// 向 liyan 的 q 群 qq_linux 添加成员
	add_to_grouplist_table("liyan", "qq_linux", "1328743347", "peiqi");
	
	// 
	
	
	
	// 读取 liyan 的 qqlist_table 下的 1378139997 这个好友的信息
	str = read_table("liyan", "qqlist_table", "1378139997");
	printf("qqID: %s\n", str->parmt[0]);
	printf("qqNAME: %s\n", str->parmt[1]);
	printf("userAGE: %s\n", str->parmt[2]);
	printf("userSEX: %s\n", str->parmt[3]);
	
	str = read_table("liyan", "mygroup_table", "294583762");
	printf("qqID: %s\n", str->parmt[0]);
	printf("qqNAME: %s\n", str->parmt[1]);
	printf("popNUM: %s\n", str->parmt[2]);
	
	str = read_table("liyan", "othergroup_table", "154583797");
	printf("qqID: %s\n", str->parmt[0]);
	printf("qqNAME: %s\n", str->parmt[1]);
	printf("popNUM: %s\n", str->parmt[2]);
	
	// str = read_table("liyan", "qq_linux_listTable", "154583797");
	// printf("qqID: %s\n", str->parmt[0]);
	// printf("qqNAME: %s\n", str->parmt[1]);
	
_out:
	// 关闭数据库
	// sqlite3_close(db);
 
	return 0;
}

