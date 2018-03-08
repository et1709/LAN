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
	sprintf(sql, "create table %s_chatTable(qqID TEXT, qqName TEXT, qqChat TEXT, qqTime TEXT)", chatName);
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
	sprintf(str, "%s.db", userName);
	// 创建一个数据库
	if (SQLITE_OK != (err = sqlite3_open(str, &db))) {
	
		printf("创建数据库 %s 失败(%s)\n", str, sqlite3_errstr(err));
		return -1;
	}
	
	memset(&sql, 0, sizeof(sql));
	sprintf(sql, "insert into qqlist_table values('%s', '%s', '%s', '%s')", qqID, qqName, p_age, q_sex);
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
